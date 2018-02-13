/*
 *  mpq-extract.c -- functions for extract files from a given mpq archive.
 *
 *  Copyright (c) 2003-2008 Maik Broemme <mbroemme@plusserver.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  $Id: mpq-extract.c,v 1.18 2004/02/12 00:39:17 mbroemme Exp $
 */

/* generic includes. */
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* libmpq includes. */
#include "mpq.h"

/* mpq-tools configuration includes. */
#include "config.h"

#include "../common.h"

/* define new print functions for error. */
#define ERROR(...) fprintf(stderr, __VA_ARGS__);

/* define new print functions for notification. */
#define NOTICE(...) printf(__VA_ARGS__);



#define MPQ_HASH_TABLE_OFFSET	0
#define MPQ_HASH_NAME_A	1
#define MPQ_HASH_NAME_B	2
#define MPQ_HASH_FILE_KEY	3
unsigned long dwCryptTable[0x500];

// The encryption and hashing functions use a number table in their procedures. This table must be initialized before the functions are called the first time.
void InitializeCryptTable() {
    uint32_t seed   = 0x00100001;
    uint32_t index1 = 0;
    uint32_t index2 = 0;
    int   i;

    for (index1 = 0; index1 < 0x100; index1++)
    {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
        {
            uint32_t temp1, temp2;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed  = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);

            dwCryptTable[index2] = (temp1 | temp2);
        }
    }
}

unsigned long HashString(const char *lpszString, unsigned long dwHashType) {
    uint32_t seed1 = 0x7FED7FED;
    uint32_t seed2 = 0xEEEEEEEE;
    int    ch;

    while (*lpszString != 0) {
        ch = toupper(*lpszString++);

        seed1 = dwCryptTable[(dwHashType * 0x100) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}



struct {uint32_t hash_a; uint32_t hash_b; char *name;}
	fileTable[100000];
int fileTableSize;

char *normalizePath(char *s) {
	int i;
	for(i = 0; s[i]; i++) {
		s[i] = tolower(s[i]);
		if(s[i]=='\\') s[i]='/';
	}
	return s;
}

void load_filetable(char *ft) {
	char name[2048];
	FILE *f = fopen(ft, "r");
	if(!f) {
		printf("Failed to open %s\n", ft);
		exit(1);
	}

    InitializeCryptTable();

	while(fgets(name, 2048, f)) {
		name[strlen(name)-2]=0;
        fileTable[fileTableSize].hash_a = HashString(name, MPQ_HASH_NAME_A);
        fileTable[fileTableSize].hash_b = HashString(name, MPQ_HASH_NAME_B);
        fileTable[fileTableSize].name = normalizePath(strdup(name));
		/*printf("%8x, %8x: "
			,fileTable[fileTableSize].hash_a
			,fileTable[fileTableSize].hash_b);
		printf("%s\n", fileTable[fileTableSize].name);*/
		fileTableSize++;
	}

	fclose(f);
}



/* returns the filename for the given file in the archive.
 * if the real filename cannot be retrieved, a dummy name is returned
 * instead.
 */
static const char *get_filename (mpq_archive_s *mpq_archive, unsigned int file_number) {

	static char buf[32];
	const char *ret;

	ret = libmpq__file_name (mpq_archive, file_number);

	if(!ret) {
		int i;
		mpq_hash_s *h = mpq_archive->mpq_hash+mpq_archive->mpq_list->hash_table_indices[file_number-1];
		for(i = 0; i < fileTableSize; i++) {
			if(h->hash_a == fileTable[i].hash_a && h->hash_b == fileTable[i].hash_b) {
				ret = fileTable[i].name;
				break;
			}
		}
	}

	if (!ret) {
		unsigned int total_files;

		total_files = libmpq__archive_info(mpq_archive, LIBMPQ_ARCHIVE_FILES);

		if (file_number > total_files)
			return NULL;

		snprintf(buf, sizeof(buf), "file%06i.xxx", file_number);
		ret = buf;
	}

	return ret;
}

/* this function extract a single file from archive. */
int mpq_extract__extract_file(mpq_archive_s *mpq_archive, unsigned int file_number, int fd) {

	/* some common variables. */
	const char *filename;
	unsigned int i;
	unsigned char *in_buf;
	unsigned char *out_buf;
	unsigned char *temp_buf;
	unsigned int in_size;
	unsigned int out_size;
	unsigned int temp_size;
	int result = 0;
	int rb = 0;
	int tb = 0;

	/* open the file. */
	if ((result = libmpq__file_open(mpq_archive, file_number)) < 0) {

		/* something on open file failed. */
		return result;
	}

	/* get/show filename to extract. */
	if ((filename = get_filename(mpq_archive, file_number)) == NULL) {

		/* filename was not found. */
		return LIBMPQ_ERROR_EXIST;
	}


	/* loop through all blocks. */
	for (i = 1; i <= libmpq__file_info(mpq_archive, LIBMPQ_FILE_BLOCKS, file_number); i++) {

		/* check if file is encrypted. */
		if (libmpq__file_info(mpq_archive, LIBMPQ_FILE_ENCRYPTED, file_number) == 1) {

			/* get buffer sizes. */
			in_size   = libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_ENCRYPTED_SIZE, file_number, i);
			temp_size = libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_DECRYPTED_SIZE, file_number, i);

			/* allocate memory for the buffers. */
			if ((in_buf   = malloc(in_size))   == NULL ||
			    (temp_buf = malloc(temp_size)) == NULL) {

				/* memory allocation problem. */
				return LIBMPQ_ERROR_MALLOC;
			}

			/* cleanup. */
			memset(in_buf,   0, in_size);
			memset(temp_buf, 0, temp_size);

			/* seek in file. */
			if (lseek(mpq_archive->fd, libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_OFFSET, file_number, i), SEEK_SET) < 0) {

				/* something with seek in file failed. */
				return LIBMPQ_ERROR_LSEEK;
			}

			/* read block from file. */
			if ((rb = read(mpq_archive->fd, in_buf, in_size)) < 0) {

				/* free temporary buffer if used. */
				if (temp_buf != NULL) {

					/* free temporary buffer. */
					free(temp_buf);
				}

				/* free input buffer if used. */
				if (in_buf != NULL) {

					/* free input buffer. */
					free(in_buf);
				}

				/* something on reading block failed. */
				return LIBMPQ_ERROR_READ;
			}

			/* decrypt the block. */
			if ((tb = libmpq__block_decrypt(in_buf, in_size, temp_buf, temp_size, libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_SEED, file_number, i))) < 0) {

				/* free temporary buffer if used. */
				if (temp_buf != NULL) {

					/* free temporary buffer. */
					free(temp_buf);
				}

				/* free input buffer if used. */
				if (in_buf != NULL) {

					/* free input buffer. */
					free(in_buf);
				}

				/* something on decrypting block failed. */
				return tb;
			}

			/* free input buffer if used. */
			if (in_buf != NULL) {

				/* free input buffer. */
				free(in_buf);
			}
		}

		/* get buffer sizes. */
		in_size  = libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_COMPRESSED_SIZE, file_number, i);
		out_size = libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_UNCOMPRESSED_SIZE, file_number, i);

		/* allocate memory for the buffers. */
		if ((in_buf  = malloc(in_size))  == NULL ||
		    (out_buf = malloc(out_size)) == NULL) {

			/* check if file is encrypted. */
			if (libmpq__file_info(mpq_archive, LIBMPQ_FILE_ENCRYPTED, file_number) == 1) {

				/* free temporary buffer if used. */
				if (temp_buf != NULL) {

					/* free temporary buffer. */
					free(temp_buf);
				}
			}

			/* memory allocation problem. */
			return LIBMPQ_ERROR_MALLOC;
		}

		/* cleanup. */
		memset(in_buf,  0, in_size);
		memset(out_buf, 0, out_size);

		/* check if file is encrypted. */
		if (libmpq__file_info(mpq_archive, LIBMPQ_FILE_ENCRYPTED, file_number) == 1) {

			/* copy temporary buffer to input buffer. */
			memcpy(in_buf, temp_buf, in_size);

			/* free temporary buffer if used. */
			if (temp_buf != NULL) {

				/* free temporary buffer. */
				free(temp_buf);
			}
		} else {

			/* seek in file. */
			if (lseek(mpq_archive->fd, libmpq__block_info(mpq_archive, LIBMPQ_BLOCK_OFFSET, file_number, i), SEEK_SET) < 0) {

				/* something with seek in file failed. */
				return LIBMPQ_ERROR_LSEEK;
			}

			/* read block from file. */
			if ((rb = read(mpq_archive->fd, in_buf, in_size)) < 0) {

				/* free output buffer if used. */
				if (out_buf != NULL) {

					/* free output buffer. */
					free(out_buf);
				}

				/* free input buffer if used. */
				if (in_buf != NULL) {

					/* free input buffer. */
					free(in_buf);
				}

				/* something on reading block failed. */
				return LIBMPQ_ERROR_READ;
			}
		}

		/* check if file is compressed. */
		if (libmpq__file_info(mpq_archive, LIBMPQ_FILE_COMPRESSED, file_number) == 1) {

			/* decompress the block. */
			if ((tb = libmpq__block_decompress(in_buf, in_size, out_buf, out_size)) < 0) {

				/* free output buffer if used. */
				if (out_buf != NULL) {

					/* free output buffer. */
					free(out_buf);
				}

				/* free input buffer if used. */
				if (in_buf != NULL) {

					/* free input buffer. */
					free(in_buf);
				}

				/* something on decrypting block failed. */
				return tb;
			}
		}

		/* check if file is imploded. */
		if (libmpq__file_info(mpq_archive, LIBMPQ_FILE_IMPLODED, file_number) == 1) {

			/* explode the block. */
			if ((tb = libmpq__block_explode(in_buf, in_size, out_buf, out_size)) < 0) {

				/* free output buffer if used. */
				if (out_buf != NULL) {

					/* free output buffer. */
					free(out_buf);
				}

				/* free input buffer if used. */
				if (in_buf != NULL) {

					/* free input buffer. */
					free(in_buf);
				}

				/* something on decrypting block failed. */
				return tb;
			}
		}

		/* check if file is neither compressed nor imploded. */
		if (libmpq__file_info(mpq_archive, LIBMPQ_FILE_COPIED, file_number) == 1) {

			/* copy the block. */
			if ((tb = libmpq__block_copy(in_buf, in_size, out_buf, out_size)) < 0) {

				/* free output buffer if used. */
				if (out_buf != NULL) {

					/* free output buffer. */
					free(out_buf);
				}

				/* free input buffer if used. */
				if (in_buf != NULL) {

					/* free input buffer. */
					free(in_buf);
				}

				/* something on decrypting block failed. */
				return tb;
			}
		}

		/* write block to file. */
		write(fd, out_buf, out_size);

		/* free output buffer if used. */
		if (out_buf != NULL) {

			/* free output buffer. */
			free(out_buf);
		}

		/* free input buffer if used. */
		if (in_buf != NULL) {

			/* free input buffer. */
			free(in_buf);
		}
	}

	/* if no error was found, return zero. */
	return 0;
}

void mpqDecompileOld(char *OutDir, char *FileName, char *ListFile) {
    char Tmp[1024];
	mpq_archive_s *mpq_archive;
	char *filename;
	unsigned int i;
	int fd;

	libmpq__init();
	load_filetable(ListFile);

	mpq_archive = malloc(sizeof(mpq_archive_s));
	memset(mpq_archive, 0, sizeof(mpq_archive_s));

	if (libmpq__archive_open(mpq_archive, FileName) < 0) {
        printf("  cant load %s\n", FileName);
        abort();
	}
 
	for (i = 1; i <= libmpq__archive_info(mpq_archive, LIBMPQ_ARCHIVE_FILES); i++) {
        filename = (char*)get_filename(mpq_archive, i);
		unless (filename) {
            printf("  cant get filename for %d\n", i);
            abort();
		}

        printf("Extracting: %s\n", filename);
        sprintf(Tmp, "%s/%s", OutDir, filename);
        writeFile(0, 0, Tmp, Tmp); // just to create path
		if ((fd = open(Tmp, O_RDWR|O_CREAT|O_TRUNC, 0644)) < 0) {
            printf("  failed to create file\n");
            abort();
		}
		if (mpq_extract__extract_file(mpq_archive, i, fd) < 0) {
          printf("  extraction failed\n");
          abort();
		}

		close(fd);
	}


	libmpq__archive_close(mpq_archive);
	if (mpq_archive) free(mpq_archive);
	libmpq__shutdown();
}


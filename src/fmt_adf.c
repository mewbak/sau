//Credits go to Bernd Schmidt and Jim Cooper

#include "common.h"

static void write_log (const char *s,...)
{
    fprintf (stderr, "%s", s);
}

static u1 filemem[901120];

typedef struct afile {
    struct afile *sibling;
    u1 *data;
    u4 size;
    char name[32];
} afile;

typedef struct directory {
    struct directory *sibling;
    struct directory *subdirs;
    struct afile *files;
    char name[32];
} directory;

static int secdatasize, secdataoffset;

static u4 readlong (u1 *buffer, int pos)
{
    return ((*(buffer + pos) << 24) + (*(buffer + pos + 1) << 16)
          + (*(buffer + pos + 2) << 8) + *(buffer + pos + 3));
}

static afile *read_file (u1 *filebuf)
{
    afile *a = n(afile);
    int sizeleft;
    u1 *datapos;
    u4 numblocks, blockpos;

    /* BCPL strings... Yuk. */
    memset (a->name, 0, 32);
    strncpy (a->name, (char *) filebuf + 0x1B1, *(filebuf + 0x1B0));
    sizeleft = a->size = readlong (filebuf, 0x144);
    a->data = ns (u1, a->size);

    numblocks = readlong (filebuf, 0x8);
    blockpos = 0x134;
    datapos = a->data;
    while (numblocks) {
      u1 *databuf = filemem + 512 * readlong (filebuf, blockpos);
      int readsize = sizeleft > secdatasize ? secdatasize : sizeleft;
      memcpy (datapos, databuf + secdataoffset, readsize);
      datapos += readsize;
      sizeleft -= readsize;

      blockpos -= 4;
      numblocks--;
      if (!numblocks) {
          u4 nextflb = readlong (filebuf, 0x1F8);
          if (nextflb) {
            filebuf = filemem + 512 * nextflb;
            blockpos = 0x134;
            numblocks = readlong (filebuf, 0x8);
            if (!filebuf) {
                write_log ("Disk structure corrupted. Use DISKDOCTOR to correct it.\n");
                abort ();
            }
          }
      }
    }
    return a;
}

static directory *read_dir (u1 *dirbuf)
{
    directory *d = n (directory);
    u4 hashsize;
    u4 i;

    memset (d->name, 0, 32);
    strncpy (d->name, (char *) dirbuf + 0x1B1, *(dirbuf + 0x1B0));
    d->sibling = 0;
    d->subdirs = 0;
    d->files = 0;
    hashsize = readlong (dirbuf, 0xc);
    if (!hashsize)
      hashsize = 72;
    if (hashsize != 72)
      write_log ("Warning: Hash table with != 72 entries.\n");
    for (i = 0; i < hashsize; i++) {
      u4 subblock = readlong (dirbuf, 0x18 + 4 * i);

      while (subblock) {
          directory *subdir;
          afile *subfile;
          u1 *subbuf = filemem + 512 * subblock;
          int dirtype;

          dirtype = (s4) readlong (subbuf, 0x1FC);
          if (dirtype > 0) {
            subdir = read_dir (subbuf);
            subdir->sibling = d->subdirs;
            d->subdirs = subdir;
          } else if (dirtype < 0) {
            subfile = read_file (subbuf);
            subfile->sibling = d->files;
            d->files = subfile;
          } else {
            write_log ("Disk structure corrupted. Use DISKDOCTOR to correct it.\n");
            abort ();
          }
          subblock = readlong (subbuf, 0x1F0);
      }
    }
    return d;
}

static void writedir (char *Output, directory * dir) {
    char Tmp[1024];
    directory *subdir;
    afile *f;
    hd(dir->name, 2);
    if (!strcmp(dir->name, " ")) *dir->name = 0;
    sprintf(Tmp, "%s/%s", Output, dir->name);

    for (subdir = dir->subdirs; subdir; subdir = subdir->sibling)
      writedir (Tmp, subdir);
    for (f = dir->files; f; f = f->sibling) {
      printf("  Extracting: %s/%s\n", dir->name, f->name);
      sprintf(Tmp, "%s/%s/%s", Output, dir->name, f->name);
      writeFile(0, f->size, Tmp, f->data);
    }
}


static void adfDecompile(char *Output, char *Input) {
    directory *root;
    FILE *inf;

    inf = fopen (Input, "rb");
    if (inf == NULL) {
      write_log ("can't open file\n");
      abort();
    }
    fread (filemem, 1, 901120, inf);

    if (strncmp ((char *) filemem, "DOS\0", 4) == 0
      || strncmp ((char *) filemem, "DOS\2", 4) == 0) {
      secdatasize = 488;
      secdataoffset = 24;
    } else if (strncmp ((char *) filemem, "DOS\1", 4) == 0
             || strncmp ((char *) filemem, "DOS\3", 4) == 0) {
      secdatasize = 512;
      secdataoffset = 0;
    } else {
      write_log ("Not a DOS ADF.\n");
      abort();
    }
    root = read_dir (filemem + 880 * 512);
    writedir (Output, root);
}


int adfInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "adf";
  F->Description = "Amiga Disk File";
  F->Decompile = adfDecompile;
  return 1;
}


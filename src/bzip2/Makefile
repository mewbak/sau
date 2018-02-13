STATICLIB = libbz2.a

#
# Set to 1 if shared object needs to be installed
#
SHARED_MODE=0

#LOC = -DASMV
#LOC = -DDEBUG -g

PREFIX =
CC = $(PREFIX)gcc
CFLAGS = $(LOC) -O3 -Wall -DBZ_STRICT_ANSI
EXTRA_CFLAGS = -DNO_VIZ

AS = $(CC)
ASFLAGS = $(LOC) -Wall

LD = $(CC)
LDFLAGS = $(LOC)

AR = $(PREFIX)ar
ARFLAGS = rcs


STRIP = $(PREFIX)strip

CP = cp -fp
# If GNU install is available, replace $(CP) with install.
INSTALL = $(CP)
RM = rm -f

prefix = /usr/local
exec_prefix = $(prefix)

OBJS = blocksort.o  compress.o  decompress.o  randtable.o \
       bzlib.o      crctable.o  huffman.o

OBJA =

all: $(STATICLIB)

.c.o:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

.S.o:
	$(AS) $(ASFLAGS) -c -o $@ $<

$(STATICLIB): $(OBJS) $(OBJA)
	$(AR) $(ARFLAGS) $@ $(OBJS) $(OBJA)


# BINARY_PATH, INCLUDE_PATH and LIBRARY_PATH must be set.

.PHONY: install uninstall clean


clean:
	-$(RM) $(STATICLIB)
	-$(RM) *.o
	-$(RM) *.exe


blocksort.o: bzlib.h bzlib_private.h
compress.o: bzlib.h bzlib_private.h
decompress.o: bzlib.h bzlib_private.h
randtable.o: bzlib.h bzlib_private.h
bzlib.o: bzlib.h bzlib_private.h
crctable.o: bzlib.h bzlib_private.h
huffman.o: bzlib.h bzlib_private.h



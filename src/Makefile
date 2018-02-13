SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS)) fmt_mpq.o

PREFIX =
MODS = storm/libStorm.a libmpq/libmpq.a \
       zlib/libz.a bzip2/libbz2.a libpng/libpng.a   
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(CXX)
STRIP = $(PREFIX)strip
AR = $(PREFIX)ar
#CFLAGS = $CFLAGS $(LOC) -O3 -Wall

all: clear_inits inits.h sau

.c.o:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

.S.o:
	$(AS) $(ASFLAGS) -c -o $@ $<


storm/libStorm.a:
	cd storm && make

libmpq/libmpq.a:
	cd libmpq && make

zlib/libz.a:
	cd zlib && make

bzip2/libbz2.a:
	cd bzip2 && make

libpng/libpng.a:
	cd libpng && make


clear_inits:
	rm -f inits.h

inits.h:
	echo '// Auto-generated file.' > inits.h
	ls fmt_*.c fmt_*.cpp | sed 's/fmt_\([^.]*\).*/extern int \1Init(format*);/' >> inits.h
	echo 'static void *Inits[] = {' >> inits.h
	ls fmt_*.c fmt_*.cpp | sed 's/fmt_\([^.]*\).*/  \1Init,/' >> inits.h
	echo '  0};' >> inits.h

sau: $(OBJS) $(MODS)
	g++  $(OBJS) -o $@ $(MODS)

.PHONY: clean

clean:
	rm -f *.o
	cd storm && make clean
	cd libmpq && make clean
	cd zlib && make clean
	cd bzip2 && make clean
	cd libpng && make clean

sau.o: common.h inits.h

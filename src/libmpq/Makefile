

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


STATICLIB = libmpq.a
OBJS = common.o explode.o extract.o huffman.o mpq.o mpq-extract.o wave.o

all: $(STATICLIB)

.c.o:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

.S.o:
	$(AS) $(ASFLAGS) -c -o $@ $<

$(STATICLIB): $(OBJS) $(OBJA)
	$(AR) $(ARFLAGS) $@ $(OBJS) $(OBJA)

clean:
	-$(RM) $(STATICLIB)
	-$(RM) *.o
	-$(RM) *.exe


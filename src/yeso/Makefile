CFLAGS = -Wall -Werror -std=gnu99 -g -Os

ZESO_PROGS = zesomunch
LXLIB = -L. -lzeso -lX11 -lXext

all: $(ZESO_PROGS)

clean:
	$(RM) $(ZESO_PROGS) *.a *.o

libzeso.a: zeso.o yeso.o
	ar rcsDv $@ $^
zesomunch: zesomunch.o libzeso.a zeso.h
	$(CC) $(CFLAGS) -o $@ zesomunch.o $(LXLIB)

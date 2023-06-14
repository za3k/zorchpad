all: zeso_progs

CFLAGS = -Wall -Werror -std=gnu99 -g -Os

ZESO_PROGS = zesomunch
LXLIB = -L. -lzeso -lX11 -lXext

zeso_progs: $(ZESO_PROGS)

clean:
	$(RM) $(ZESO_PROGS) *.a *.o

libyeso.a: yeso.o
	ar rcsDv $@ $^
libzeso.a: zeso.o yeso.o
	ar rcsDv $@ $^
yeso-xlib: yeso.o yeso.h
	$(CC) $(CFLAGS) yeso.o -lX11 -lXext -o $@
zesomunch: zesomunch.o libzeso.a zeso.h
	$(CC) $(CFLAGS) -o $@ zesomunch.o $(LXLIB)

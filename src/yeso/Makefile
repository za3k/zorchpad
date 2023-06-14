all: yeso_progs

CFLAGS = -Wall -Werror -std=gnu99 -g -Os

YESO_PROGS = yesomunch
LXLIB = -L. -lyeso -lX11 -lXext

yeso_progs: $(YESO_PROGS)

clean:
	$(RM) $(YESO_PROGS) *.a *.o

libyeso.a: yeso.o
	ar rcsDv $@ $^
yeso-xlib: yeso.o yeso.h
	$(CC) $(CFLAGS) yeso.o -lX11 -lXext -o $@
yesomunch: yesomunch.o libyeso.a yeso.h
	$(CC) $(CFLAGS) -o $@ yesomunch.o $(LXLIB)

all: ae zi
clean:
	rm -f ae zi
zi: zi.c
	cc -Dindex=index_ptr -DBUF=4096 -DMODE=0666 -Os $< -o $@ -L../yeso -I ../yeso -lzeso -lX11 -lXext
ae: ae.c
	cc -Dindex=index_ptr -DBUF=4096 -DMODE=0666 -Os $< -o $@ -lcurses

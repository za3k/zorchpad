/*
 *	ae.c		Anthony's Editor  IOCCC '91
 *
 *	Public Domain 1991 by Anthony Howe.  All rights released.
 *
 *  Adapted for the Zorchpad emulator in 2023 by Zachary Vance.
 */

#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "zeso.h"

int done;
int row, col;
int index, page, epage;
char buf[BUF];
char *ebuf;
char *gap = buf;
char *egap;
char *filename;
static zwin w;

/*
 *	The following assertions must be maintained.
 *
 *	o  buf <= gap <= egap <= ebuf
 *		If gap == egap then the buffer is full.
 *
 *	o  point = ptr(index) and point < gap or egap <= point
 *
 *	o  page <= index < epage
 *
 *	o  0 <= index <= pos(ebuf) <= BUF
 *
 *
 *	Memory representation of the file:
 *
 *		low	buf  -->+----------+
 *				|  front   |
 *				| of file  |
 *			gap  -->+----------+<-- character not in file 
 *				|   hole   |
 *			egap -->+----------+<-- character in file
 *				|   back   |
 *				| of file  |
 *		high	ebuf -->+----------+<-- character not in file 
 *
 *
 *	point & gap
 *
 *	The Point is the current cursor position while the Gap is the 
 *	position where the last edit operation took place. The Gap is 
 *	ment to be the cursor but to avoid shuffling characters while 
 *	the cursor moves it is easier to just move a pointer and when 
 *	something serious has to be done then you move the Gap to the 
 *	Point. 
 */

int adjust();
int nextline();
int pos();
int prevline();
char *ptr();

void bottom();
void delete();
void display();
void down();
void file();
void insert();
void left();
void lnbegin();
void lnend();
void movegap();
void pgdown();
void pgup();
void redraw();
void right();
void quit();
void top();
void up();
void wleft();
void wright();


int x=0, y=0;
void clear() {
    zw_fill(w, WHITE);
}
void refresh() {
    zw_flip(w);
}
#define LINES HEIGHT_CHAR
#define COLS WIDTH_CHAR
void addch(char c, int invert) {
    zw_set_char(w, x, y, c, 0, invert);
    ++x;
    if (c == '\n' || x >= COLS) {
        x=0;
        if (++y >= LINES) {
            y = LINES-1; // Oops! no scrolling :P
        }
    }
}
void move(int ny, int nx) {
    x=nx;
    y=ny;
}
void mvaddstr(int y, int x, char *s) {
    move(y,x);
    while (*s!='\0') addch(*s++, 0);
}
void clrtobot() {
    while (y < LINES-1 || x < COLS-1) addch(0, 0); // Haha yuck
    addch(0, 0);
}
#include <stdio.h>
uint64_t getch() {
    uint64_t c = zw_get_key(w);
    return c;
}


uint64_t key[] = {
    'h', 'j', 'k', 'l',
    'H', 'J', 'K', 'L',
    '[', ']', 't', 'b',
    'i', 'x', 'W', 'R',
    'Q'
};

void (*func[])() = {
	left, down, up, right, 
	wleft, pgdown, pgup, wright,
	lnbegin, lnend, top, bottom, 
	insert, delete, file, redraw,
    quit, movegap
};

char * ptr(int offset) {
	if (offset < 0)
		return (buf);
	return (buf+offset + (buf+offset < gap ? 0 : egap-gap));
}

int pos(char *pointer) {
	return (pointer-buf - (pointer < egap ? 0 : egap-gap)); 
}

void top() {
	index = 0;
}

void bottom() {
	epage = index = pos(ebuf);
}

void quit() {
	done = 1;
}


void redraw() {
	clear();
	display();
}

void movegap() {
	char *p = ptr(index);
	while (p < gap)
		*--egap = *--gap;
	while (egap < p)
		*gap++ = *egap++;
	index = pos(egap);
}

int prevline(int offset) {
	char *p;
	while (buf < (p = ptr(--offset)) && *p != '\n') {}
	return (buf < p ? ++offset : 0);
}

int nextline(int offset) {
	char *p;
	while ((p = ptr(offset++)) < ebuf && *p != '\n')	
		;
	return (p < ebuf ? offset : pos(ebuf));
}

int adjust(int offset, int column) {
	char *p;
	int i = 0;
	while ((p = ptr(offset)) < ebuf && *p != '\n' && i < column) {
		i += *p == '\t' ? 8-(i&7) : 1;
		++offset;
	}
	return (offset);
}

void left() {
	if (0 < index)
		--index;
} 

void right() {
	if (index < pos(ebuf))
		++index;
}

void up() {
	index = adjust(prevline(prevline(index)-1), col);
}

void down() {
	index = adjust(nextline(index), col);
}

void lnbegin() {
	index = prevline(index);
}

void lnend() {
	index = nextline(index);
	left();
}

void wleft() {
	char *p;
	while (!isspace(*(p = ptr(index))) && buf < p)
		--index;
	while (isspace(*(p = ptr(index))) && buf < p)
		--index;
}

void pgdown() {
	page = index = prevline(epage-1);
	while (0 < row--)
		down();
	epage = pos(ebuf);
}

void pgup() {
	int i = LINES;
	while (0 < --i) {
		page = prevline(page-1); 
		up();
	}
}

void wright() {
	char *p;
	while (!isspace(*(p = ptr(index))) && p < ebuf)
		++index;
	while (isspace(*(p = ptr(index))) && p < ebuf)
		++index;
}

int is_printable(uint64_t keysym) {
    return keysym >= 0x20 && keysym < 0x7F;
}

void insert() {
	uint64_t ch;
	movegap();
	while ((ch = getch()) != zk_ctrl_l) {
		if (ch == zk_backspace) {
			if (buf < gap)
				--gap;
		} else if (gap < egap) {
            if (is_printable(ch)) {
			    *gap++ = ch;
            } else if (ch == zk_enter) {
			    *gap++ = '\n';
            }
		}
		index = pos(egap);
		redraw();
	}
}

void delete() {
	movegap();
	if (egap < ebuf)
		index = pos(++egap);
}

void file() {
	int i;
	int j = index;
	index = 0;
	movegap();
	write(i = creat(filename, MODE), egap, (int)(ebuf-egap));
	close(i);
	index = j;
}

void display() {
	char *p;
	int i, j;
	if (index < page)
		page = prevline(index);
	if (epage <= index) {
		page = nextline(index); 
		i = page == pos(ebuf) ? LINES-2 : LINES; 
		while (0 < i--)
			page = prevline(page-1);
	}
	move(0, 0);
	i = j = 0;
	epage = page;
	while (1) {
		if (index == epage) {
			row = i;
			col = j;
		}
		p = ptr(epage);
		if (LINES <= i || ebuf <= p)
			break;
		if (*p != '\r') {
			addch(*p, index==epage);
			j += *p == '\t' ? 8-(j&7) : 1;
		}
		if (*p == '\n' || COLS <= j) {
			++i;
			j = 0;
		}
		++epage;
	}
	clrtobot();
	if (++i < LINES)
		mvaddstr(i, 0, "<< EOF >>");
	move(row, col);
	refresh();
}

int main(int argc, char **argv) {
	int i;
    uint64_t ch;
    w = zw_open();
	egap = ebuf = buf + BUF;
	if (argc < 2)
		return (2);
	if (0 < (i = open(filename = *++argv, 0))) {
		gap += read(i, buf, BUF);
		if (gap < buf)
			gap = buf;
		close(i);
	}
	top();
	while (!done) {
		redraw();
		i = 0; 
		ch = getch(); 
		while (key[i] != 0 && ch != key[i]) ++i;
		(*func[i])();
	}
	return (0);
}

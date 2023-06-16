#include "zeso.h"
#include <stdlib.h>
#include <string.h>
#include "yeso.h"
#include "tamsyn8x16r.h"
#include "tamsyn8x16b.h"
#define BITS_TO_BYTES(x) ((x+7)/8)

// The destination type of the opaque pointer type zwin, declared but not defined in zeso.h
struct _zwin {
  ywin ywin;
  zpix *fb;
};

#define COLOR_FRAME 0xaaaaaa
#define COLOR_WHITE 0x838383
#define COLOR_BLACK 0x335343
static inline ypix colorize(zpix value) {
    return value ? COLOR_WHITE : COLOR_BLACK;
}

zwin zw_open() {
    zwin zw = malloc(sizeof(*zw));
    zw->fb = malloc(sizeof(zpix)*WIDTH_PX*HEIGHT_PX);
    zw->ywin = yw_open("Zorchpad Simulator", WIDTH_PX, HEIGHT_PX, "");

    zw_fill(zw, WHITE);
    zw_flip(zw);
    return zw;
}

void zw_close(zwin w) {
    yw_close(w->ywin);
    free(w->fb);
    free(w);
}

int zin_bounds(int x, int y) {
    return x >= 0 && x < WIDTH_PX && y >=0 && y < HEIGHT_PX;
}

zpix zw_get_pixel(zwin w, int x, int y) {
    if (!zin_bounds(x,y)) return 0;
    return w->fb[x+WIDTH_PX*y] ? 1 : 0;
}
void zw_set_pixel(zwin w, int x, int y, zpix value) {
    if (!zin_bounds(x,y)) return;
    w->fb[x+WIDTH_PX*y] = value;
}

void zw_flip(zwin w) {
    ypic p = yw_frame(w->ywin);
    int w_scale = p.w / WIDTH_PX;
    int h_scale = p.h / HEIGHT_PX;
    int scale = w_scale < h_scale ? w_scale : h_scale;
    yp_fill(p, COLOR_FRAME);
    for (int y=0; y<HEIGHT_PX; y++) {
        for (int x=0; x<WIDTH_PX; x++) {
           ypix color = colorize(w->fb[x+y*WIDTH_PX]);
           yp_fill(yp_sub(p, x*scale, y*scale, scale, scale), color);
        }
    }
    yw_flip(w->ywin);
}

void zw_fill(zwin p, zpix value) {
    memset(p->fb, value, WIDTH_PX*HEIGHT_PX*sizeof(zpix));
}

void zw_set_char(zwin w, int x, int y, char ch, int style_bold) {
    if (x < 0 || y < 0 || x >= WIDTH_CHAR || y >= HEIGHT_CHAR) return;
    int charIndex = ch <= 32 ? 0 : ch - 32;
    if (style_bold) {
        for (int row = 0; row < GLYPH_HEIGHT; row++) {
            for (int col = 0; col < GLYPH_WIDTH; col++) {
                zw_set_pixel(w,
                  x*GLYPH_WIDTH+col,
                  y*GLYPH_HEIGHT+row,
                  tamsyn8x16b_bits[BITS_TO_BYTES(tamsyn8x16b_width) * row + charIndex] & (0x1 << col)
                );
            }
        }
    } else {
        for (int row = 0; row < GLYPH_HEIGHT; row++) {
            for (int col = 0; col < GLYPH_WIDTH; col++) {
                zw_set_pixel(w,
                  x*GLYPH_WIDTH+col,
                  y*GLYPH_HEIGHT+row,
                  tamsyn8x16r_bits[BITS_TO_BYTES(tamsyn8x16r_width) * row + charIndex] & (0x80 >> col)
                );
            }
        }
    }
}
void zw_clear_char(zwin w, int x, int y) {
    zw_set_char(w, x, y, 0, 0);
}

char zw_get_char(zwin w) {
    while (1) {
		yw_wait(w->ywin, 0); // TODO: Deal with stuff like Control-C
		yw_event *e1 = yw_get_event(w->ywin);
		if (!e1) continue;
		yw_key_event *e = yw_as_key_event(e1);
		if (!e) continue;
		if (!e->down) continue;
		if (!e->s) continue;
		return e->s[0];
	}
}

uint32_t zw_get_key(zwin w) {
    while (1) {
		yw_wait(w->ywin, 0);
		yw_event *e1 = yw_get_event(w->ywin);
		if (!e1) continue;
		yw_key_event *e = yw_as_key_event(e1);
		if (!e) continue;
		if (!e->down) continue;
		if (!e->s) continue;
		return e->keysym;
	}
}

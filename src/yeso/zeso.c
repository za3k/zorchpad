#include "zeso.h"
#include <stdlib.h>
#include <string.h>

// The destination type of the opaque pointer type zwin, declared but not defined in zeso.h
struct _zwin {
  ywin ywin;
  zpix *fb;
};

static inline ypix colorize(zpix value) {
    return value ? 0x838383 : 0x335343;
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
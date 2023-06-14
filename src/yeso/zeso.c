#include "zeso.h"

static inline ypix colorize(zpix value) {
    //return value ? 0x335343 : 0x838383;
    return value ? 0xff0000ul : 0x00ff00ul;
    //return value ? 0x838383 : 0x335343;
}

zwin zw_open() {
    zwin w = yw_open("Zorchpad Simulator", 400, 240, "");
    zpic p = zw_frame(w);
    yp_fill(p, 0x0000ff);
    zp_fill(p, 1);
    zw_flip(w);
    return w;
}

void zw_close(zwin w) {
    yw_close(w);
}

zpic zw_frame(zwin w) {
    return yw_frame(w);
}

int zin_bounds(int x, int y) {
    return x >= 0 && x < 400 && y >=0 && y < 240;
}

void zp_set_pixel(zpic p, int x, int y, zpix value) {
    if (!zin_bounds(x,y)) return;
    ypix *pxl = yp_pix(p, x, y);
    *pxl = colorize(value);
}

void zw_flip(zwin w) {
    yw_flip(w);
}

void zp_fill(zpic p, zpix value) {
    yp_fill(p, colorize(value));
}

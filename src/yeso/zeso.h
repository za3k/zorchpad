#ifndef ZESO_H_INCLUDED
#define ZESO_H_INCLUDED
#include <stdint.h>
#include "yeso.h"

typedef struct _zwin *zwin; // _zwin defined in zeso.c

#define BLACK 0
#define WHITE 1
#define WIDTH_PX 400
#define HEIGHT_PX 240
typedef uint8_t zpix; // 0=black, 1=white
typedef struct { zpix *p; int w, h; } zpic;

zwin zw_open();
void zw_close(zwin w);
zpic zw_frame(zwin w);
void zw_set_pixel(zwin p, int x, int y, zpix value);
zpix zw_get_pixel(zwin p, int x, int y);
void zw_flip(zwin w);
void zw_fill(zwin p, zpix value);

#endif  /* ZESO_H_INCLUDED */

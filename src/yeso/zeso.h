#ifndef ZESO_H_INCLUDED
#define ZESO_H_INCLUDED
#include <stdint.h>
#include "yeso.h"

typedef ywin zwin;
typedef ypic zpic;
typedef uint32_t zpix; // 0=black, 1=white
#define BLACK 0
#define WHITE 1

zwin zw_open();
void zw_close(zwin w);
zpic zw_frame(zwin w);
void zp_set_pixel(zpic p, int x, int y, zpix value);
void zw_flip(zwin w);
void zp_fill(zpic p, zpix value);

#endif  /* ZESO_H_INCLUDED */

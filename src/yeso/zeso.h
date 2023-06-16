#ifndef ZESO_H_INCLUDED
#define ZESO_H_INCLUDED
#include <stdint.h>

typedef struct _zwin *zwin; // _zwin defined in zeso.c

#define BLACK 0
#define WHITE 1
#define WIDTH_PX 400
#define HEIGHT_PX 240
#define GLYPH_WIDTH 8
#define GLYPH_HEIGHT 16
#define WIDTH_CHAR ((WIDTH_PX+GLYPH_WIDTH-1)/GLYPH_WIDTH)
#define HEIGHT_CHAR ((HEIGHT_PX+GLYPH_HEIGHT-1)/GLYPH_HEIGHT)
typedef uint8_t zpix; // 0=black, 1=white
typedef struct { zpix *p; int w, h; } zpic;

zwin zw_open();
void zw_close(zwin w);
zpic zw_frame(zwin w);
void zw_set_pixel(zwin w, int x, int y, zpix value);
zpix zw_get_pixel(zwin w, int x, int y);
void zw_flip(zwin w);
void zw_fill(zwin w, zpix value);

void zw_set_char(zwin w, int x, int y, char c, int style_bold);
void zw_clear_char(zwin w, int x, int y);

char zw_get_char(zwin w);
uint32_t zw_get_key(zwin w);

#endif  /* ZESO_H_INCLUDED */

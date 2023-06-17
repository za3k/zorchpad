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
uint64_t zw_get_key(zwin w);

/* useful keysyms */
// A typical 40% keyboard
/*
enum {
    // Literal char values for the below
    // qwertyuiop
    // QWERTYUIOP
    // asdfghjkl;
    // ASDFGHJKL:
    // zxcvbnm,./
    // ZXCVBNM<>?

    // `1234567890-=
    // ~!@#$%^&*()_+

    zk_space  0x20

    zk_esc    0xff1b
    zk_enter  0xff0d
    zk_home   0xff50
    zk_end    0xff57
    zk_insert 0xff63
    zk_back   0xff08
    zk_del    0xffff
    zk_tab    0xff09
    zk_pgup   0xff55
    zk_pgdown 0xff56

    zk_left   0xff51
    zk_up     0xff52
    zk_right  0xff53
    zk_down   0xff54
    zk_printscreen 0xff61

    zk_f1     0xffbe
    zk_f2     0xffbf
    zk_f3     0xffc0
    zk_f4     0xffc1
    zk_f5     0xffc2
    zk_f6     0xffc3
    zk_f7     0xffc4
    zk_f8     0xffc5
    zk_f9     0xffc6
    zk_f10    0xffc7
    zk_f11    0xffc8
    zk_f12    0xffc9

    zk_shift_left  0xffe1
    zk_shift_left  0xffe2
    zk_ctrl_left   0xffe3
    zk_ctrl_right  0xffe4
    zk_alt_left    0xffe9
    zk_alt_right   0xffea
    zk_meta1       0xffeb

    zk_fn     0x1008ff2b
}
*/

enum { zk_left = 0xff51, zk_up, zk_right, zk_down,
       zk_pgup, zk_pgdn, zk_end, zk_home,
       zk_shift_l = 0xffe1, zk_shift_r, zk_control_l, zk_control_r,
       zk_caps_lock, zk_shift_lock, zk_meta_l, zk_meta_r,
       zk_backspace = 0xff08, zk_return = 0xff0d };

#endif  /* ZESO_H_INCLUDED */

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

// keysyms for a typical keyboard
enum {
    // Literal char values for the ASCII range, 0x20-0x7E
    zk_space=0x20,
    zk_exclamation_point, // aka bang
    zk_double_quote,
    zk_octothorpe, // aka hashtag aka pound sign
    zk_dollar_sign,
    zk_percent,
    zk_ampersand,
    zk_single_quote,
    zk_left_parenthesis,
    zk_right_parenthesis,
    zk_asterisk,
    zk_plus,
    zk_comma,
    zk_minus, // aka dash
    zk_period, // aka dot
    zk_slash,
    zk_0, zk_1, zk_2, zk_3, zk_4, zk_5, zk_6, zk_7, zk_8, zk_9,
    zk_color,
    zk_semicolon,
    zk_less_than, // aka left angle bracket
    zk_equal, // aka equals aka equals sign
    zk_greater_than, // aka right angle bracket
    zk_question_mark,
    zk_at_sign,
    zk_A=0x41, zk_B, zk_C, zk_D, zk_E, zk_F, zk_G, zk_H, zk_I, zk_J,
    zk_K, zk_L, zk_M, zk_N, zk_O, zk_P, zk_Q, zk_R, zk_S, zk_T, zk_U,
    zk_V, zk_W, zk_X, zk_Y, zk_Z,
    zk_left_square_bracket,
    zk_backslash,
    zk_right_square_bracket,
    zk_caret, // aka hat
    zk_underscore,
    zk_backtick, // aka grave
    zk_a=0x61, zk_b, zk_c, zk_d, zk_e, zk_f, zk_g, zk_h, zk_i, zk_j,
    zk_k, zk_l, zk_m, zk_n, zk_o, zk_p, zk_q, zk_r, zk_s, zk_t, zk_u,
    zk_v, zk_w, zk_x, zk_y, zk_z,
    zk_left_curly_bracket,
    zk_pipe, // aka vertical bar
    zk_right_curly_bracket,
    zk_tilde, // 0x7E, end of printable ascii

    zk_backspace = 0xff08,
    zk_esc    = 0xff1b,
    zk_enter  = 0xff0d, // aka return
    zk_home   = 0xff50,
    zk_end    = 0xff57,
    zk_insert = 0xff63,
    zk_back   = 0xff08,
    zk_del    = 0xffff,
    zk_tab    = 0xff09,
    zk_pgup   = 0xff55,
    zk_pgdown = 0xff56,

    zk_left   = 0xff51,
    zk_up     = 0xff52,
    zk_right  = 0xff53,
    zk_down   = 0xff54,
    zk_printscreen = 0xff61,

    zk_f1 = 0xffbe, zk_f2, zk_f3, zk_f4, zk_f5, zk_f6, zk_f7, zk_f8,
    zk_f9, zk_f10, zk_f11, zk_f12,

    zk_shift_left  = 0xffe1,
    zk_shift_right = 0xffe2,
    zk_ctrl_left   = 0xffe3,
    zk_ctrl_right  = 0xffe4,
    zk_caps_lock   = 0xffe5,
    zk_shift_lock  = 0xffe6,
    zk_alt_left    = 0xffe9,
    zk_alt_right   = 0xffea,
    zk_win         = 0xffeb, // Windows key, for me

    zk_fn = 0x1008ff2b,

    zk_ctrl_a = 0x100000061, zk_ctrl_b, zk_ctrl_c, zk_ctrl_d, zk_ctrl_e, zk_ctrl_f, zk_ctrl_g, zk_ctrl_h, zk_ctrl_i, zk_ctrl_j, zk_ctrl_k, zk_ctrl_l, zk_ctrl_m, zk_ctrl_n, zk_ctrl_o, zk_ctrl_p, zk_ctrl_q, zk_ctrl_r, zk_ctrl_s, zk_ctrl_t, zk_ctrl_u, zk_ctrl_v, zk_ctrl_w, zk_ctrl_x, zk_ctrl_y, zk_ctrl_z,
};

#endif  /* ZESO_H_INCLUDED */

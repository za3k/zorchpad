// Munching squares

#include "zeso.h"
#include <unistd.h>

int main()
{
  zwin w = zw_open();
    zpic fb = zw_frame(w);
    for (int y = 0; y < fb.h; y++) {
        zp_set_pixel(fb, y, y, WHITE);
    }
    sleep(5);

  /*
  for (int t = 0;; t++) {
    zpic fb = zw_frame(w);
    for (int y = 0; y < fb.h; y++) {
      zp_set_pixel(fb, t ^ y, y, black);
    }
    yw_flip(w);
  }
  */
}

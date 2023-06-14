// Munching squares

#include "zeso.h"
#include <unistd.h>

int main()
{
  zwin w = zw_open();
  for (int t = 0;; t++) {
    for (int y = 0; y < HEIGHT_PX; y++) {
      int x = t ^ y;
      zpix cur = zw_get_pixel(w, x, y);
      zw_set_pixel(w, x, y, cur ? 0 : 1);
    }
    zw_flip(w);
    sleep(0.1);
  }
}

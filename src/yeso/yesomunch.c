/* Munching squares, generalized to TrueColor. */

#include "yeso.h"

int main()
{
  ywin w = yw_open("munching squares", 1024, 1024, "");

  for (int t = 0;; t++) {
    ypic fb = yw_frame(w);
    for (int y = 0; y < fb.h; y++) {
      ypix *p = yp_pix(fb, 0, y);
      for (int x = 0; x < fb.w; x++) p[x] = (t & 1023) - ((x ^ y) & 1023);
    }

    yw_flip(w);
  }
}

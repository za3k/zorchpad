// Echo keyboard input

#include "zeso.h"
#include <stdint.h>
#include <stdio.h>

int main()
{
  zwin w = zw_open();
  int y=0, x=0;
  uint64_t in;
  zw_flip(w);
  while ((in=zw_get_key(w))!=0) {
    zw_set_char(w, x, y, in, 0, 0);
	if ((x=(x+1)%WIDTH_CHAR) == 0) {
		y=(y+1)%HEIGHT_CHAR;
    }
    zw_flip(w);
  }
  while (1) zw_flip(w);
}

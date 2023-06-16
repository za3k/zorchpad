// Print hello world

#include "zeso.h"

int main()
{
  zwin w = zw_open();
  int y=0, x=0;
  char *msg = "Hello, world!";
  while (*msg!='\0') {
    zw_set_char(w, x++, y, *msg++, 0);
    zw_flip(w);
  }
  for (int c=0; c<=255; c++) {
    zw_set_char(w, c%50, (c/50)+1, c, 0);
    zw_flip(w);
  }
  zw_flip(w);
  while (1) zw_flip(w);
}

// Print hello world

#include "zeso.h"

int main()
{
  zwin w = zw_open();
  int y=0, x=0;
  char *msg = "Hello, world!";
  while (*msg!='\0') {
    zw_set_char(w, x++, y, *msg++, 0);
  }
  zw_flip(w);
  while (1) zw_flip(w);
}

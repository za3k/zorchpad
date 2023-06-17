// Print keycodes

#include "zeso.h"
#include <stdint.h>
#include <stdio.h>

int main()
{
  zwin w = zw_open();
  zw_flip(w);
  while (1) {
    uint64_t keysym = zw_get_key(w);
    printf("Key pressed: 0x%lx %lu\n", keysym, keysym);
  }
  while (1) zw_flip(w);
}

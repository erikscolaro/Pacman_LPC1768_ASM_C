#include "image.h"
#define SMALL_DOT_HEIGHT 2
#define SMALL_DOT_WIDTH 2


// array size is 169
const uint8_t small_dot[]  = { 
  0xf8,0xf8,0xf8,0xf8,
};

const Image small_dot_image = {
  SMALL_DOT_WIDTH, 
  SMALL_DOT_HEIGHT, 
  small_dot
};
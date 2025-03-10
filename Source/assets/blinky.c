#include "image.h"
#define BLINKY_HEIGHT 13
#define BLINKY_WIDTH 13

// array size is 169
const uint8_t blinky[]  = {
  0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 
  0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xf6, 0xfb, 0xe0, 0xe0, 0xe0, 0xfb, 0xf6, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xf2, 0xff, 0xff, 0xf6, 0xe0, 0xf6, 0xff, 0xff, 0xf2, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xf2, 0x24, 0x00, 0xf6, 0xe0, 0xf6, 0x00, 0x24, 0xf2, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe9, 0x92, 0x92, 0xe9, 0xe0, 0xe9, 0x92, 0x92, 0xe9, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xe4, 0xe9, 0xe0, 0xe0, 0xe0, 0xe9, 0xe4, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0
};

const Image blinky_image = {
  BLINKY_WIDTH,
  BLINKY_HEIGHT,
  blinky
};
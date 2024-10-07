#include "cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  /* @todo [Part 3] */
  double ex = (double)x;
  if(y >= 60 && y < 110){
    return HSLAPixel(0,0,0);
  }
  return HSLAPixel(230,1,0.6);
}

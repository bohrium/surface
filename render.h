#include "stdbool.h"
#include "bitmap.h" 
#include "geometry.h" 

#ifndef RENDER_H
#define  RENDER_H

bool draw_trig(Bitmap* bmp, XYZ q0, XYZ q1, XYZ q2, RGB color);

#endif//RENDER_H

#include "stdbool.h"
#include "bitmap.h"

#ifndef GEOMETRY_H  
#define GEOMETRY_H  

typedef struct XYZ XYZ;
struct XYZ {
    double x, y, z;
};

typedef struct XY XY;
struct XY {
    int x, y;
};

bool oriented(XY a, XY b, XY c);
XY project(XYZ xyz, HW frame);

#endif//GEOMETRY_H  

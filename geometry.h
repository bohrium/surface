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

typedef struct Trig Trig;
struct Trig {
    XYZ a; XYZ b; XYZ c;
};

typedef struct Trigs Trigs;
struct Trigs {
    int cap, len;
    Trig* data;
};

void init_trigs(Trigs* tsp);
void append_trig(Trigs* tsp, Trig t);
void qsort_trigs(Trigs* tsp);
void free_trigs(Trigs* tsp);

#endif//GEOMETRY_H  

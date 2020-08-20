#include "math.h"
#include "stdbool.h"
#include "bitmap.h"

#ifndef GEOMETRY_H  
#define GEOMETRY_H  

typedef struct XYZ XYZ;
struct XYZ {
    double x, y, z;
};

XYZ cross(XYZ v, XYZ w);
XYZ normalize(XYZ v);
XYZ linear(XYZ v, double c, XYZ w);

typedef struct XY XY;
struct XY {
    int x, y;
};

double area(XY a, XY b, XY c);
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
void append_small_trig(Trigs* tsp, Trig t, double eps);
void qsort_trigs(Trigs* tsp);
void free_trigs(Trigs* tsp);

double horizontality(Trig* tp);

#endif//GEOMETRY_H  

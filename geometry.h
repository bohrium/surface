#include "stdbool.h"
#include "bitmap.h" 

typedef struct XY XY;
struct XY {
    int x, y;
};

bool oriented(XY a, XY b, XY c);

bool draw_triangle(Bitmap* bmp, XY p0, XY p1, XY p2, RGB color);


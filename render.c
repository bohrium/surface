#include "stdbool.h"
#include "stdio.h"
#include "bitmap.h" 
#include "geometry.h" 
#include "render.h" 

#define MIN(X,Y) ((X)<(Y)?(X):(Y)) 
#define MAX(X,Y) ((X)>(Y)?(X):(Y)) 

bool draw_trig(Bitmap* bmp, XYZ q0, XYZ q1, XYZ q2, RGB color)
{
    XY p0 = project(q0, bmp->dims);
    XY p1 = project(q1, bmp->dims);
    XY p2 = project(q2, bmp->dims);

    int h = MAX(0               , MIN(MIN(p0.y, p1.y), p2.y));
    int H = MIN(bmp->dims.height, MAX(MAX(p0.y, p1.y), p2.y));
    int w = MAX(0               , MIN(MIN(p0.x, p1.x), p2.x));
    int W = MIN(bmp->dims.width , MAX(MAX(p0.x, p1.x), p2.x));

    if (!oriented(p0, p1, p2)) {
        XY tmp = p2;
        p2 = p1;
        p1 = tmp;
    }

    for (int r = h; r != H; ++r) {
        for (int c = w; c != W; ++c) {
            if ( oriented(p0, p1, (XY){c,r}) &&     
                 oriented(p1, p2, (XY){c,r}) &&    
                 oriented(p2, p0, (XY){c,r})    ) {
                bmp->data[r * bmp->dims.width + c] = color;
            }
        }
    }
}


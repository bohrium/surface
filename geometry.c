#include "stdbool.h"
#include "stdio.h"
#include "bitmap.h" 
#include "geometry.h" 

#define MIN(X,Y) ((X)<(Y)?(X):(Y)) 
#define MAX(X,Y) ((X)>(Y)?(X):(Y)) 

bool oriented(XY a, XY b, XY c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x) > 0.0;
}

bool draw_triangle(Bitmap* bmp, XY p0, XY p1, XY p2, RGB color)
{
    int h = MAX(0               , MIN(MIN(p0.y, p1.y), p2.y));
    int H = MIN(bmp->dims.height, MAX(MAX(p0.y, p1.y), p2.y));
    int w = MAX(0               , MIN(MIN(p0.x, p1.x), p2.x));
    int W = MIN(bmp->dims.width , MAX(MAX(p0.x, p1.x), p2.x));
    printf("%d %d %d %d\n", h, H, w, W);

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


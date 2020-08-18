#include "stdlib.h"
#include "bitmap.h"
#include "geometry.h"

void main()
{
    const int H = 512;
    const int W = 512;
    Bitmap bm = (Bitmap){NULL, (HW){0, 0}};
    allocate(&bm, (HW){H, W});

    // background
    for (int r = 0; r != H; ++r) {
        for (int c = 0; c != W; ++c) {
            bm.data[r * W + c] = (RGB) {0, 50, 100};
        }
    }

    // triangle
    XY p0 = { 30,  30};
    XY p1 = {190, 190};
    XY p2 = { 60, 160};
    draw_triangle(&bm, p0, p1, p2, (RGB){250,200,210});

    write_to(&bm, "moo.bmp");
    deallocate(&bm);
}

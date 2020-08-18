#include "stdlib.h"
#include "math.h"
#include "bitmap.h"
#include "geometry.h"
#include "render.h"

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

    XYZ q0, q1, q2, q3;

    for (double t = -3.14/2; t < 3.14/2; t += 0.10) {
        double z = 2.0 + cos(t+0.00);
        q0 = (XYZ){sin(t+0.00), -0.5, 2.0 + cos(t+0.00)}; 
        q1 = (XYZ){sin(t+0.05), -0.5, 2.0 + cos(t+0.05)}; 
        q2 = (XYZ){sin(t+0.05), +0.5, 2.0 + cos(t+0.05)}; 
        q3 = (XYZ){sin(t+0.00), +0.5, 2.0 + cos(t+0.00)}; 
        draw_triangle(&bm, q0, q1, q2, (RGB){250,200/z,210});
        draw_triangle(&bm, q0, q2, q3, (RGB){250,200/z,210});
    }

    // triangle
    q0 = (XYZ){-0.30, -0.30, 1.0};
    q1 = (XYZ){0.90, 0.60, 1.0};
    q2 = (XYZ){0.80, 0.80, 1.0};
    q3 = (XYZ){0.60, 0.90, 1.0};
    draw_triangle(&bm, q0, q1, q2, (RGB){50,200,210});
    draw_triangle(&bm, q0, q2, q3, (RGB){50,100,210});

    for (double t = 3.14/2; t < 3.14; t += 0.10) {
        double z = 2.0 + cos(t+0.00);
        q0 = (XYZ){sin(t+0.00), -0.5, 2.0 + cos(t+0.00)}; 
        q1 = (XYZ){sin(t+0.05), -0.5, 2.0 + cos(t+0.05)}; 
        q2 = (XYZ){sin(t+0.05), +0.5, 2.0 + cos(t+0.05)}; 
        q3 = (XYZ){sin(t+0.00), +0.5, 2.0 + cos(t+0.00)}; 
        draw_triangle(&bm, q0, q1, q2, (RGB){250,200/z,210});
        draw_triangle(&bm, q0, q2, q3, (RGB){250,200/z,210});
    }



    write_to(&bm, "moo.bmp");
    deallocate(&bm);
}

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
    Trigs ts;
    init_trigs(&ts);

    // cylinder
    for (double t = -3.14; t < 3.14; t += 0.15) {
        double z = 2.0 + cos(t+0.00);
        q0 = (XYZ){sin(t+0.00), -0.5, 2.0 + cos(t+0.00)}; 
        q1 = (XYZ){sin(t+0.05), -0.5, 2.0 + cos(t+0.05)}; 
        q2 = (XYZ){sin(t+0.00), +0.5, 2.0 + cos(t+0.00)}; 
        q3 = (XYZ){sin(t+0.05), +0.5, 2.0 + cos(t+0.05)}; 
        append_trig(&ts, (Trig){q0,q1,q2});
        append_trig(&ts, (Trig){q1,q2,q3});
    }

    // triangle
    q0 = (XYZ){-0.30, -0.30, 2.0};
    q1 = (XYZ){0.90, 0.60, 2.0};
    q2 = (XYZ){0.80, 0.80, 2.0};
    q3 = (XYZ){0.60, 0.90, 2.0};
    append_trig(&ts, (Trig){q0,q1,q2});
    append_trig(&ts, (Trig){q1,q2,q3});

    qsort_trigs(&ts);

    for ( int i = 0; i != ts.len; ++i ) {
        q0 = ts.data[ts.len-1-i].a;
        q1 = ts.data[ts.len-1-i].b;
        q2 = ts.data[ts.len-1-i].c;
        draw_trig(&bm, q0, q1, q2, (RGB){250/q0.z, 40, 40});
    }

    write_to(&bm, "moo.bmp");
    deallocate(&bm);
    free_trigs(&ts);
}

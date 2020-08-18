#include "stdlib.h"
#include "stdio.h"
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

//    // cylinder
//    for (double t = -3.14; t < 3.14; t += 0.15) {
//        q0 = (XYZ){sin(t+0.00), -0.5, 2.0 + cos(t+0.00)}; 
//        q1 = (XYZ){sin(t+0.05), -0.5, 2.0 + cos(t+0.05)}; 
//        q2 = (XYZ){sin(t+0.00), +0.5, 2.0 + cos(t+0.00)}; 
//        q3 = (XYZ){sin(t+0.05), +0.5, 2.0 + cos(t+0.05)}; 
//        append_trig(&ts, (Trig){q0,q1,q2});
//        append_trig(&ts, (Trig){q1,q2,q3});
//    }
//    // triangles
//    q0 = (XYZ){-0.30, -0.30, 2.0};
//    q1 = (XYZ){0.90, 0.60, 2.0};
//    q2 = (XYZ){0.80, 0.80, 2.0};
//    q3 = (XYZ){0.60, 0.90, 2.0};
//    append_trig(&ts, (Trig){q0,q1,q2});
//    append_trig(&ts, (Trig){q1,q2,q3});

    // sphere
    for (double s = -3.14/2; s < 3.14/2; s += 0.02) {
        for (double t = -3.14; t < 3.14; t += 0.02) {
            if ( -3.14/6<s && s<3.14/6 && -3.14/4<t && t<3.14/3 ) continue; 
            if ( 3.14/7<s && s<3.14/3 && 4*3.14/9<t && t<2*3.14/3 ) continue; 
            if ( s*(t-3.14/2) > 3.14*3.14/4 ) continue; 
            if ( s*(t-3.14) < -3.14*3.14/4 ) continue; 
            q0 = (XYZ){cos(s+0.00)*cos(t+0.00), sin(s+0.00), 2.0 + cos(s+0.00)*sin(t+0.00)}; 
            q1 = (XYZ){cos(s+0.02)*cos(t+0.00), sin(s+0.02), 2.0 + cos(s+0.02)*sin(t+0.00)}; 
            q2 = (XYZ){cos(s+0.00)*cos(t+0.02), sin(s+0.00), 2.0 + cos(s+0.00)*sin(t+0.02)}; 
            q3 = (XYZ){cos(s+0.02)*cos(t+0.02), sin(s+0.02), 2.0 + cos(s+0.02)*sin(t+0.02)}; 
            append_trig(&ts, (Trig){q0,q1,q2});
            append_trig(&ts, (Trig){q1,q2,q3});
        }
    }

    qsort_trigs(&ts);

    for ( int i = 0; i != ts.len; ++i ) {
        Trig t = ts.data[ts.len-1-i];
        double avgx = (t.a.x + t.b.x + t.c.x)/3.0;
        double avgy = (t.a.y + t.b.y + t.c.y)/3.0;
        double avgz = (t.a.z + t.b.z + t.c.z)/3.0;
        double lum = (0.1 + 0.9*horizontality(&t))/(0.5+0.5*avgz);
        draw_trig(&bm, t.a, t.b, t.c, (RGB){
            50+200*lum*(1 + tanh(avgx))/2,
            50+200*lum*(1 + tanh(avgy))/2,
            50+200*lum*(1 + tanh(avgz))/2,
        });
    }

    write_to(&bm, "moo.bmp");
    deallocate(&bm);
    free_trigs(&ts);
}

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

    to_color(&bm, (RGB){250, 250, 250});

    XYZ q0, q1, q2, q3;
    Trigs ts;
    init_trigs(&ts);

    // cylinder
    for (double t = -3.14; t < 3.14; t += 0.25) {
        q0 = (XYZ){0.9*sin(t+0.00), -0.1, 2.0 + 0.9*cos(t+0.00)}; 
        q1 = (XYZ){0.9*sin(t+0.10), -0.1, 2.0 + 0.9*cos(t+0.10)}; 
        q2 = (XYZ){0.9*sin(t+0.00), +0.1, 2.0 + 0.9*cos(t+0.00)}; 
        q3 = (XYZ){0.9*sin(t+0.10), +0.1, 2.0 + 0.9*cos(t+0.10)}; 
        append_small_trig(&ts, (Trig){q0,q1,q2});
        append_small_trig(&ts, (Trig){q1,q2,q3});
    }

    // sphere
    for (double s = -3.14/2; s < 3.14/2; s += 0.01) {
        for (double t = -3.14; t < 3.14; t += 0.01) {
            if ( -3.14/6<s && s<3.14/6 && -3.14/4<t && t<3.14/3 ) continue; 
            if ( 3.14/7<s && s<3.14/3 && 4*3.14/9<t && t<2*3.14/3 ) continue; 
            if ( s*(t-3.14/2) > 3.14*3.14/4 ) continue; 
            if ( s*(t-3.14) < -3.14*3.14/4 ) continue; 
            if ( -3.14/24<s && s<3.14/24 ) continue; 
            q0 = (XYZ){cos(s+0.00)*cos(t+0.00), sin(s+0.00), 2.0 + cos(s+0.00)*sin(t+0.00)}; 
            q1 = (XYZ){cos(s+0.01)*cos(t+0.00), sin(s+0.01), 2.0 + cos(s+0.01)*sin(t+0.00)}; 
            q2 = (XYZ){cos(s+0.00)*cos(t+0.01), sin(s+0.00), 2.0 + cos(s+0.00)*sin(t+0.01)}; 
            q3 = (XYZ){cos(s+0.01)*cos(t+0.01), sin(s+0.01), 2.0 + cos(s+0.01)*sin(t+0.01)}; 
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

    Bitmap smaller = (Bitmap){NULL, (HW){0, 0}};

    halve(&bm, &smaller);
    twice(&smaller, &bm);
    write_to(&smaller, "moo.bmp");

    quick_blur(&bm);
    halve(&bm, &smaller);
    write_to(&smaller, "moo2.bmp");

    deallocate(&bm);
    deallocate(&smaller);
    free_trigs(&ts);
}

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "bitmap.h"
#include "geometry.h"
#include "render.h"

#define pi M_PI

void main()
{
    const int H = 512;
    const int W = 512;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.0. allocate resources  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    Bitmap bm = (Bitmap){NULL, (HW){0, 0}};
    allocate(&bm, (HW){H, W});

    Bitmap antialiased = (Bitmap){NULL, (HW){0, 0}};

    Trigs ts;
    init_trigs(&ts);

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.1. collect triangles  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    XYZ q0, q1, q2, q3;

    /*----------------  0.1.0. ...from fenced cylinder  ---------------------*/
    for (double t = -pi; t < pi; t += 0.25) {
        q0 = (XYZ){0.9*sin(t+0.00), -0.1, 2.0 + 0.9*cos(t+0.00)}; 
        q1 = (XYZ){0.9*sin(t+0.10), -0.1, 2.0 + 0.9*cos(t+0.10)}; 
        q2 = (XYZ){0.9*sin(t+0.00), +0.1, 2.0 + 0.9*cos(t+0.00)}; 
        q3 = (XYZ){0.9*sin(t+0.10), +0.1, 2.0 + 0.9*cos(t+0.10)}; 
        append_small_trig(&ts, (Trig){q0,q1,q2});
        append_small_trig(&ts, (Trig){q1,q2,q3});
    }

    /*----------------  0.1.1. ...from windowed sphere  ---------------------*/
    double dx = 0.01;
    for (double s = -pi/2; s < pi/2; s += dx) {
        for (double t = -pi; t < pi; t += dx) {
            if ( -pi/6<s && s<pi/6 && -pi/4<t && t<pi/3 ) continue; 
            if ( pi/7<s && s<pi/3 && 4*pi/9<t && t<2*pi/3 ) continue; 
            if ( s*(t-pi/2) > pi*pi/4 ) continue; 
            if ( s*(t-pi) < -pi*pi/4 ) continue; 
            if ( -pi/24<s && s<pi/24 ) continue; 
            q0 = (XYZ){cos(s+0.)*cos(t+0.), sin(s+0.), 2+cos(s+0.)*sin(t+0.)}; 
            q1 = (XYZ){cos(s+dx)*cos(t+0.), sin(s+dx), 2+cos(s+dx)*sin(t+0.)}; 
            q2 = (XYZ){cos(s+0.)*cos(t+dx), sin(s+0.), 2+cos(s+0.)*sin(t+dx)}; 
            q3 = (XYZ){cos(s+dx)*cos(t+dx), sin(s+dx), 2+cos(s+dx)*sin(t+dx)}; 
            append_trig(&ts, (Trig){q0,q1,q2});
            append_trig(&ts, (Trig){q1,q2,q3});
        }
    }

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.2. render triangles  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*----------------  0.2.0. sort triangles by depth  ---------------------*/
    qsort_trigs(&ts);

    /*----------------  0.2.1. set background to light gray  ----------------*/
    to_color(&bm, (RGB){250, 250, 250});

    /*----------------  0.2.2. from farthest to nearest triangle...  --------*/
    for ( int i = 0; i != ts.len; ++i ) {
        Trig t = ts.data[ts.len-1-i];
        /*------------  0.2.3. ...color by location and orientation  --------*/
        double avgx = (t.a.x + t.b.x + t.c.x)/3.0;
        double avgy = (t.a.y + t.b.y + t.c.y)/3.0;
        double avgz = (t.a.z + t.b.z + t.c.z)/3.0;
        double lum = (0.1 + 0.9*horizontality(&t))/(0.5+0.5*avgz);
        RGB color = {
            50+200*lum*(1 + tanh(avgx))/2,
            50+200*lum*(1 + tanh(avgy))/2,
            50+200*lum*(1 + tanh(avgz))/2,
        };

        /*------------  0.2.4. ...and paint onto canvas  --------------------*/
        draw_trig(&bm, t.a, t.b, t.c, color);
    }

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.3. write anti-aliased version to file  ~~~~~~~~~~~~~~~~~~~~*/

    quick_blur(&bm);
    halve(&bm, &antialiased);
    write_to(&antialiased, "moo2.bmp");

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.4. free resources  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    deallocate(&bm);
    deallocate(&antialiased);
    free_trigs(&ts);
}

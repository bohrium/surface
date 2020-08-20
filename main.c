#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "bitmap.h"
#include "geometry.h"
#include "render.h"

#define pi M_PI

void draw_sphere(Trigs* tsp, XYZ center, double rad, double ds, double dt)
{
    XYZ q0, q1, q2, q3;
    for (double s = -pi/2; s < pi/2; s += ds) {
        for (double t = -pi; t < pi; t += dt) {
            double ss0 = sin(s+0.), cs0 = cos(s+0.); 
            double ss1 = sin(s+ds), cs1 = cos(s+ds); 
            double st0 = sin(t+0.), ct0 = cos(t+0.); 
            double st1 = sin(t+dt), ct1 = cos(t+dt); 
            q0 = (XYZ){center.x+rad*cs0*ct0, center.y+rad*ss0, center.z+rad*cs0*st0}; 
            q1 = (XYZ){center.x+rad*cs1*ct0, center.y+rad*ss1, center.z+rad*cs1*st0}; 
            q2 = (XYZ){center.x+rad*cs0*ct1, center.y+rad*ss0, center.z+rad*cs0*st1}; 
            q3 = (XYZ){center.x+rad*cs1*ct1, center.y+rad*ss1, center.z+rad*cs1*st1}; 
            append_trig(tsp, (Trig){q0,q1,q2});
            append_trig(tsp, (Trig){q1,q2,q3});
        }
    }
}

void draw_cylinder(Trigs* tsp, XYZ center, XYZ axis, double rad, double dt, double dh)
{
    XYZ ax_b, ax_c;
    frame_at(axis, &ax_b, &ax_c);

    XYZ q0, q1, q2, q3;
    for (double t = -pi; t < pi; t += dt) {
        for (double h = 0.0; h < 1.0; h += dh) {
            XYZ cpos = linear(center, h   , axis);
            XYZ cneg = linear(center, h+dh, axis);
            q0 = linear(linear(cpos, rad*sin(t+0.), ax_b), rad*cos(t+0.), ax_c);
            q1 = linear(linear(cpos, rad*sin(t+dt), ax_b), rad*cos(t+dt), ax_c);
            q2 = linear(linear(cneg, rad*sin(t+0.), ax_b), rad*cos(t+0.), ax_c);
            q3 = linear(linear(cneg, rad*sin(t+dt), ax_b), rad*cos(t+dt), ax_c);
            append_trig(tsp, (Trig){q0,q1,q2});
            append_trig(tsp, (Trig){q1,q2,q3});
        }
    }
}

void draw_ruled(Trigs* tsp, XYZ a, XYZ b, XYZ c, XYZ d, double ds, double dt)
{
    XYZ ba = linear(b, -1.0, a); 
    XYZ ca = linear(c, -1.0, a); 
    XYZ db = linear(d, -1.0, b); 
    XYZ q0, q1, q2, q3;
    for (double s = 0.0; s < 1.0-ds/2; s += ds) {
        for (double t = 0.0; t < 1.0-dt/2; t += dt) {
            q0 = linear(linear(linear(a, s   , ba), (1-(s   ))*(t   ), ca), (s   )*(t   ), db); 
            q1 = linear(linear(linear(a, s+ds, ba), (1-(s+ds))*(t   ), ca), (s+ds)*(t   ), db); 
            q2 = linear(linear(linear(a, s   , ba), (1-(s   ))*(t+dt), ca), (s   )*(t+dt), db); 
            q3 = linear(linear(linear(a, s+ds, ba), (1-(s+ds))*(t+dt), ca), (s+ds)*(t+dt), db); 
            append_trig(tsp, (Trig){q0,q1,q2});
            append_trig(tsp, (Trig){q1,q2,q3});
        }
    }
}




void main()
{
    const int H = 1024;
    const int W = 1024;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.0. allocate resources  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    Bitmap bm = (Bitmap){NULL, (HW){0, 0}};
    allocate(&bm, (HW){H, W});

    Bitmap antialiased = (Bitmap){NULL, (HW){0, 0}};

    Trigs ts;
    init_trigs(&ts);

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.1. collect triangles  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    double dddx = 0.01;
    double ddx  = 0.03; 
    double dx   = 0.15;

    ///*----------------  0.1.0. ...from top of a sphere  ---------------------*/
    //draw_sphere(&ts,   (XYZ){0, -1.5, 2.0}, 1.0, 0.02, 0.05); 
    //draw_sphere(&ts,   (XYZ){0, -0.5, 2.0}, 0.1, 0.02, 0.05); 
    //draw_sphere(&ts,   (XYZ){0, -0.3, 2.0}, 0.1, 0.02, 0.05); 
    //draw_sphere(&ts,   (XYZ){0, -0.1, 2.0}, 0.1, 0.02, 0.05); 
    //draw_cylinder(&ts, (XYZ){0,  0.1, 2.0}, (XYZ){+0.2, 0.0, 0.8}, 0.4, 0.05, 0.02); 
    //draw_cylinder(&ts, (XYZ){0,  0.3, 2.0}, (XYZ){-0.8, 0.0, 0.2}, 0.4, 0.05, 0.02); 
    
    XYZ cent = {0.00,-1.30, 2.00}; 
    XYZ up   = normalize((XYZ){0.00, 1.00,-0.60}, 1.0); 
    XYZ ax_b;
    XYZ ax_c;
    frame_at(up, &ax_b, &ax_c);

    double radius= 1.20;
    draw_sphere  (&ts, cent, radius, dddx, ddx); 

    double altitude= 1.00;
    double spread = 0.25;
    double dt = 2*pi/6;
    double ddt = dt/6;
    double low = 0.2;  
    double high = 1.0;  
    double base = radius+low*altitude;  
    double run = 3*pi;
    double slope = (high-low)*altitude/run;
    draw_cylinder(&ts, cent, normalize(up, (radius+altitude)*cos(spread)), 0.01, dx, ddx); 
    for (double t = 0.0; t < run-dt/2; t += dt) {
        XYZ axis0 = linear(linear(up, spread*sin(t   ), ax_b), spread*cos(t   ), ax_c);
        XYZ axis1 = linear(linear(up, spread*sin(t+dt), ax_b), spread*cos(t+dt), ax_c);
        draw_cylinder(&ts, cent, normalize(axis0, radius+altitude), 0.01, dx, ddx); 
    }
    for (double t = 0.0; t < run-ddt/2; t += ddt) {
        XYZ axis0 = linear(linear(up, spread*sin(t    ), ax_b), spread*cos(t    ), ax_c);
        XYZ axis1 = linear(linear(up, spread*sin(t+ddt), ax_b), spread*cos(t+ddt), ax_c);
        draw_ruled(
            &ts,
            linear(cent, 1.0, normalize(up   , slope*t         + base*cos(spread))),
            linear(cent, 1.0, normalize(up   , slope*(t+ddt)    + base*cos(spread))),
            linear(cent, 1.0, normalize(axis0, slope*(t-ddt/2) + base)),
            linear(cent, 1.0, normalize(axis1, slope*(t+ddt/2) + base)),
            dx,
            ddx 
        );
        //append_small_trig(&ts, (Trig){
        //    linear(cent, 1.0, normalize(up   , slope*t         + base*cos(spread))),
        //    linear(cent, 1.0, normalize(axis0, slope*(t-ddt/2) + base)),
        //    linear(cent, 1.0, normalize(axis1, slope*(t+ddt/2) + base)),
        //}, dx); 
        //append_small_trig(&ts, (Trig){
        //    linear(cent, 1.0, normalize(up   , slope*(t-ddt)   + base*cos(spread))),
        //    linear(cent, 1.0, normalize(up   , slope*t         + base*cos(spread))),
        //    linear(cent, 1.0, normalize(axis0, slope*(t-ddt/2) + base)),
        //}, dx); 
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
        double lum = (0.2 + 0.8*horizontality(&t))/(0.5+0.5*avgz);
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

    //halve(&bm, &antialiased);
    //quick_blur(&antialiased);
    //twice(&antialiased, &bm);
    quick_blur(&bm);
    halve(&bm, &antialiased);
    write_to(&antialiased, "moo.bmp");

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.4. free resources  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    deallocate(&bm);
    deallocate(&antialiased);
    free_trigs(&ts);
}

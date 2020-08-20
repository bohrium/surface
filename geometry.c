#include "math.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "geometry.h"

#define ABS(X) ((X)>0?(X):-(X))

XYZ cross(XYZ v, XYZ w)
{
    return (XYZ){
        v.y*w.z - v.z*w.y,
        v.z*w.x - v.x*w.z, 
        v.x*w.y - v.y*w.x,
    };
}
XYZ normalize(XYZ v, double n)
{
    double norm = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return (XYZ){
        n*v.x/norm,
        n*v.y/norm,
        n*v.z/norm,
    };
}
XYZ linear(XYZ v, double c, XYZ w)
{
    return (XYZ){
        v.x + c*w.x,
        v.y + c*w.y,
        v.z + c*w.z,
    };
}
void frame_at(XYZ v, XYZ* perp_b, XYZ* perp_c)
{
    XYZ _ = v.x == 0.0 ? (XYZ){1,0,0} :
            v.y == 0.0 ? (XYZ){0,1,0} :
                         (XYZ){0,0,1} ;
    *perp_b = normalize(cross(v, _      ), 1.0); 
    *perp_c = normalize(cross(v, *perp_b), 1.0);
}

bool oriented(XY a, XY b, XY c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x) >= 0.0;
}

XY project(XYZ xyz, HW frame)
{
    int H = frame.height;
    int W = frame.width;
    double scale = 1.0 / (1.0 + xyz.z/10.0); 
    return (XY){
        .x = W * (0.5 * (scale * xyz.x + 1.0)),  
        .y = H * (0.5 * (scale * xyz.y + 1.0)),  
    };
}

void init_trigs(Trigs* tsp)
{
    *tsp = (Trigs){.cap=0, .len=0, .data=NULL};
}

void append_small_trig(Trigs* tsp, Trig t, double eps)
{
    if (
        ABS(t.b.x - t.a.x) < eps && ABS(t.c.x - t.b.x) < eps && ABS(t.a.x - t.c.x) < eps &&
        ABS(t.b.y - t.a.y) < eps && ABS(t.c.y - t.b.y) < eps && ABS(t.a.y - t.c.y) < eps &&
        ABS(t.b.z - t.a.z) < eps && ABS(t.c.z - t.b.z) < eps && ABS(t.a.z - t.c.z) < eps ) {
        append_trig(tsp, t);
    } else {
        XYZ avga = { .x=(t.b.x+t.c.x)/2, .y=(t.b.y+t.c.y)/2, .z=(t.b.z+t.c.z)/2 };
        XYZ avgb = { .x=(t.c.x+t.a.x)/2, .y=(t.c.y+t.a.y)/2, .z=(t.c.z+t.a.z)/2 };
        XYZ avgc = { .x=(t.a.x+t.b.x)/2, .y=(t.a.y+t.b.y)/2, .z=(t.a.z+t.b.z)/2 };
        append_small_trig(tsp, (Trig){avga, avgb, t.c }, eps);
        append_small_trig(tsp, (Trig){t.a , avgb, avgc}, eps);
        append_small_trig(tsp, (Trig){avga, t.b , avgc}, eps);
        append_small_trig(tsp, (Trig){avga, avgb, avgc}, eps);
    }
}
void append_trig(Trigs* tsp, Trig t)
{
    if ( tsp->len == tsp->cap ) {
        int new_cap = (3*tsp->cap)/2 + 1;
        Trig* new_data = malloc(sizeof(Trig) * new_cap);
        for ( int i = 0; i != tsp->len; ++i ) {
            new_data[i] = tsp->data[i];
        }
        free(tsp->data); tsp->data = new_data;
        tsp->cap = new_cap;
    }
    tsp->data[tsp->len] = t;
    tsp->len += 1;
}

void qsort_trigs_inner(Trig* beg, Trig* end);
void qsort_trigs(Trigs* tsp)
{
    qsort_trigs_inner(tsp->data, tsp->data + tsp->len);
}
void swap_trigs(Trig* a, Trig* b)
{
    Trig tmp = *a; *a = *b; *b = tmp;
}
void qsort_trigs_inner(Trig* beg, Trig* end)
{
    Trig* bbeg = beg; 
    Trig* eend = end; 
    if ( end-beg <= 1 ) { return; }
    while ( beg+1 != end )  {
        if ( beg->a.z < (beg+1)->a.z ) { swap_trigs(beg+1,end-1); end-=1; } 
        else                           { swap_trigs(beg+1,beg  ); beg+=1; }
    }
    qsort_trigs_inner(bbeg, beg);
    qsort_trigs_inner(beg+1, eend);
}

void free_trigs(Trigs* tsp)
{
    free(tsp->data); tsp->data = NULL;
    tsp->cap = tsp->len = 0;
}

double horizontality(Trig* tp)
{
    XYZ a = tp->a;
    XYZ b = tp->b;
    XYZ c = tp->c;
    double xx = (b.y-a.y)*(c.z-a.z) - (b.z-a.z)*(c.y-a.y); 
    double yy = (b.z-a.z)*(c.x-a.x) - (b.x-a.x)*(c.z-a.z); 
    double zz = (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x); 
    return sqrt(zz*zz / (xx*xx + yy*yy + zz*zz)); 
}



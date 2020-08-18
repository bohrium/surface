#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "geometry.h"

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

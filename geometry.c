#include "stdbool.h"
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


/* author: samtenka
 * change: 2020-08-19
 * create: 2020-08-19 
 * descrp: interfaces for bitmap reading and writing
 * to use: call `read_from` and `write_to` 
*/

#include "bitmap.h"

void to_color(Bitmap* bmp, RGB color)
{
    int H = bmp->dims.height;
    int W = bmp->dims.width;

    for (int r = 0; r != H; ++r) {
        for (int c = 0; c != W; ++c) {
            bmp->data[r * W + c] = color;
        }
    }
}

void quick_blur(Bitmap* bmp)
{
    int H = bmp->dims.height;
    int W = bmp->dims.width;
    if ( H < 3 || W < 3 ) { return; }

    for (int r = 1; r != H-1; ++r) {
        for (int c = 1; c != W-1; ++c) {
            bmp->data[r*W + c].R *= 0.6;
            bmp->data[r*W + c].G *= 0.6;
            bmp->data[r*W + c].B *= 0.6;

            bmp->data[r*W + c].R += 0.1*bmp->data[(r+1)*W + (c  )].R;
            bmp->data[r*W + c].G += 0.1*bmp->data[(r+1)*W + (c  )].G;
            bmp->data[r*W + c].B += 0.1*bmp->data[(r+1)*W + (c  )].B;

            bmp->data[r*W + c].R += 0.1*bmp->data[(r-1)*W + (c  )].R;
            bmp->data[r*W + c].G += 0.1*bmp->data[(r-1)*W + (c  )].G;
            bmp->data[r*W + c].B += 0.1*bmp->data[(r-1)*W + (c  )].B;

            bmp->data[r*W + c].R += 0.1*bmp->data[(r  )*W + (c+1)].R;
            bmp->data[r*W + c].G += 0.1*bmp->data[(r  )*W + (c+1)].G;
            bmp->data[r*W + c].B += 0.1*bmp->data[(r  )*W + (c+1)].B;

            bmp->data[r*W + c].R += 0.1*bmp->data[(r  )*W + (c-1)].R;
            bmp->data[r*W + c].G += 0.1*bmp->data[(r  )*W + (c-1)].G;
            bmp->data[r*W + c].B += 0.1*bmp->data[(r  )*W + (c-1)].B;
        }
    }
}

void halve(Bitmap* src, Bitmap* dst)
{
    int H = src->dims.height;
    int W = src->dims.width;

    allocate(dst, (HW){H/2,W/2}); 
    to_color(dst, (RGB){0,0,0});

    for (int r = 0; r != H; ++r) {
        for (int c = 0; c != W; ++c) {
            dst->data[(r/2)*(W/2) + (c/2)].R += 0.25 * src->data[r*W + c].R;
            dst->data[(r/2)*(W/2) + (c/2)].G += 0.25 * src->data[r*W + c].G;
            dst->data[(r/2)*(W/2) + (c/2)].B += 0.25 * src->data[r*W + c].B;
        }
    }

}

void twice(Bitmap* src, Bitmap* dst)
{
    int H = src->dims.height;
    int W = src->dims.width;

    allocate(dst, (HW){2*H,2*W}); 
    to_color(dst, (RGB){0,0,0});

    for (int r = 0; r != 2*H; ++r) {
        for (int c = 0; c != 2*W; ++c) {
            dst->data[r*(2*W) + c].R = src->data[(r/2)*W + (c/2)].R;
            dst->data[r*(2*W) + c].G = src->data[(r/2)*W + (c/2)].G;
            dst->data[r*(2*W) + c].B = src->data[(r/2)*W + (c/2)].B;
        }
    }
}

double median_of_three(double a, double b, double c)
{
    return (a<b) ? (b<c) ? b :
                   (c<a) ? a : c
                 : (c<b) ? b :
                   (a<c) ? a : c;
}
double middle_of_nine(double arr[9])
{
    return median_of_three(
        median_of_three(arr[0], arr[3], arr[6]),
        median_of_three(arr[7], arr[1], arr[4]),
        median_of_three(arr[5], arr[8], arr[2])
    ); 
} 

double middle_of_twenty_five(double arr[25])
{
    return median_of_three(
        middle_of_nine(arr+ 0),
        middle_of_nine(arr+ 8),
        middle_of_nine(arr+16)
    );
} 



void quick_sharp(Bitmap* bmp)
{
    int H = bmp->dims.height;
    int W = bmp->dims.width;
    if ( H < 5 || W < 5 ) { return; }

    for (int r = 2; r != H-2; ++r) {
        for (int c = 2; c != W-2; ++c) {
            double rs[25];
            double gs[25];
            double bs[25];
            for ( int dr = -2; dr != 3; ++dr ) {
                for ( int dc = -2; dc != 3; ++dc ) {
                    rs[(dr+2)*5+(dc+2)] = bmp->data[(r+dr)*W + (c+dc)].R;
                    gs[(dr+2)*5+(dc+2)] = bmp->data[(r+dr)*W + (c+dc)].G;
                    bs[(dr+2)*5+(dc+2)] = bmp->data[(r+dr)*W + (c+dc)].B;
                }
            }
            bmp->data[r*W + c].R = middle_of_twenty_five(rs);
            bmp->data[r*W + c].G = middle_of_twenty_five(gs);
            bmp->data[r*W + c].B = middle_of_twenty_five(bs);
        }
    }
}

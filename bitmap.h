/* author: samtenka
 * change: 2020-08-17
 * create: 2018-03-29 
 * descrp: interfaces for bitmap reading and writing
 * to use: call `read_from` and `write_to` 
*/

#ifndef BITMAP_H
#define BITMAP_H

typedef struct RGB RGB;
struct RGB {
    double R,G,B;
};

typedef struct HW HW;
struct HW {
    int height, width;
};

typedef struct Bitmap Bitmap;
struct Bitmap {
    RGB* data;
    HW dims;
};

void allocate(Bitmap* bmp, HW dims);
void deallocate(Bitmap* bmp);
void read_from(Bitmap* bmp, const char* filename);
void write_to(Bitmap* bmp, const char* filename);

#endif//BITMAP_H

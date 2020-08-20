#include "stdlib.h"
#include "stdio.h"
#include "bitmap.h"

int row_size(Bitmap* bmp);
long long fgetnum(FILE* file, int num_bytes);
void fputnum(FILE* file, long long val, int num_bytes);

void allocate(Bitmap* bmp, HW dims)
{
    bmp->dims = dims;
    bmp->data = malloc(sizeof(RGB) * dims.height * dims.width);
}

void deallocate(Bitmap* bmp) {
    free(bmp->data); bmp->data = NULL;
    bmp->dims = (HW){0, 0};
}

void read_from(Bitmap* bmp, const char* filename)
{
    deallocate(bmp);

    FILE* file = fopen(filename, "rb");
    {
        fseek(file, 18, SEEK_SET);
        int w = fgetnum(file, 4);
        int h = fgetnum(file, 4);
        printf("read w %d, h %d\n", w, h);
        allocate(bmp, (HW){h, w});

        fseek(file, 10, SEEK_SET);
        int start = fgetnum(file, 4); 

        // seek to data beginning
        fseek(file, start/*18+36*/, SEEK_SET);
        for (int r = 0; r != h; ++r) {
            for (int c = 0; c != w; ++c) {
                bmp->data[r*w + c].B = (double) fgetc(file);
                bmp->data[r*w + c].G = (double) fgetc(file);
                bmp->data[r*w + c].R = (double) fgetc(file);
            }
            // padding
            for (int c3 = 3*w; c3 != row_size(bmp); ++c3) {
                fgetc(file);
            }
            //for (int c3 = 0; c3 != 0; ++c3) {
            //    fgetc(file);
            //}
        }        
    }
    fclose(file);
}

void write_to(Bitmap* bmp, const char* filename)
{
    int h = bmp->dims.height;
    int w = bmp->dims.width;
    printf("write w %d, h %d\n", w, h);
    int size = 18+36 + row_size(bmp) * h;

    FILE* file = fopen(filename, "wb");
    {
        // header
        fputs("BM", file);
        int header[14][2] = {
            {size, 4}, {0, 4}, {18+36, 4}, {40, 4}, // header size info
            {w, 4}, {h, 4},                         // bitmap dims
            {1, 2}, {24, 2}, {0, 4},                // compression info
            {3 * row_size(bmp) * h, 4},             // resolution
            {2835, 4}, {2835, 4},                   // ... more resolution
            {0, 4}, {0, 4},                         // (used) color table info
        }; 
        for (int i = 0; i < 14; ++i) {
            fputnum(file, header[i][0], header[i][1]);
        }

        // data
        for (int r = 0; r != h; ++r) {
            for (int c = 0; c != w; ++c) {
                fputc((int) bmp->data[r*w + c].B, file);
                fputc((int) bmp->data[r*w + c].G, file);
                fputc((int) bmp->data[r*w + c].R, file);
            }
            // padding
            for (int c3 = 3*w; c3 != row_size(bmp); ++c3) {
                fputc(0, file);
            }
            //for (int c3 = 0; c3 != 0; ++c3) {
            //    fputc(0, file);
            //}
        }
    }
    fclose(file);
}

int row_size(Bitmap* bmp)
{
    /*bitmap rows are padded to be multiple of 4 bytes, so 1--> 3--> 4
                                                           2--> 6--> 8
                                                           3--> 9-->12
                                                           4-->12-->12 */
    //int n = 3*bmp->dims.width;
    //return n%4==0 ? n : n+4-(n%4);
    int n = 3 * bmp->dims.width+3;
    return n - (n%4);
    //return 4 * bmp->dims.width;
}

long long fgetnum(FILE* file, int num_bytes)
{
    /*assumes least significant byte comes first*/
    long long val = 0;
    long long power = 1;
    for (int i = 0; i != num_bytes; ++i) {
        val += power * fgetc(file);
        power <<= 8;
    }
    return val;
}

void fputnum(FILE* file, long long val, int num_bytes)
{
    /*writes least significant byte first*/
    for (int i = 0; i != num_bytes; ++i) {
        fputc(val & 0xff, file);
        val >>= 8;
    }
}



#include "stdlib.h"
#include "bitmap.h"

void main()
{
    Bitmap bm = (Bitmap){NULL, (HW){0, 0}};
    //allocate(&bm, (HW){6,7});
    read_from(&bm, "sam.bmp");
    write_to(&bm, "moo.bmp");
    deallocate(&bm);
}

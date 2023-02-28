#include <stdio.h>
#include <gd.h>

int main(int argc, char * argv[])
{
    gdImagePtr img;
    int color;
    FILE * f;

    img = gdImageCreateTrueColor(100, 100);
    color = gdTrueColor(255, 255, 255);
    gdImageLine(img, 10, 80, 89, 80, color);
    f = fopen("wynik.png", "wb");
    gdImagePng(img, f);
    fclose(f);
    gdImageDestroy(img);

    return 0;
}

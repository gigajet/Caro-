#include "csDes_sup.h"
#include <cstdio>
#include "basicdraw.h"
#include "screenUtils.h"
#include <windows.h>

struct CsDes_Pixel {
        char ASCII;
        int ForegroundColor;
        int BackgroundColor;
};
void ReadCsDesImage (const char * fileName, int & width, int & height, short background[], short foreground[], char ascii[]) {
    width = height = -1;
    ///Open File
    FILE * hImg = fopen(fileName, "rb");
    if (hImg == NULL) return;

    fscanf(hImg, "%d %d\n",&width,&height);

    CsDes_Pixel tmp;

    ///Read & close
    for (int w=0; w<width; ++w)
        for (int h=0; h<height; ++h) {
            fread(&tmp, sizeof(CsDes_Pixel), 1, hImg);
            background[h*width+w] = tmp.BackgroundColor;
            foreground[h*width+w] = tmp.ForegroundColor;
            ascii[h*width+w] = tmp.ASCII;

        }
    fclose(hImg);
}

void PutCsDesPixel (short x, short y, short background, short foreground, char ascii) {
    GotoXY(x,y);
    SetColor(background, foreground);
    PutChar(ascii);
}
void PutCsDesImage (short x, short y, int width, int height, short background[], short foreground[], char ascii[]) {
    for (short w=0; w<width; ++w)
        for (short h=0; h<height; ++h) {
            PutCsDesPixel(x+w,y+h,background[h*width+w],foreground[h*width+w],ascii[h*width+w]);
        }
}


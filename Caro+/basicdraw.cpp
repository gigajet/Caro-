#include "basicdraw.h"
#include <windows.h>
#include <cstdio>

void GotoXY (short x, short y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {x,y});
}
void SetColor (short BackgroundColor, short ForegroundColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BackgroundColor<<4)+ForegroundColor);
}
void PutChar (char c) {
    printf("%c",c);
}
void PutString (const char* s) {
    printf("%s",s);
}
void PutFilledRectangle (short x1, short y1, short x2, short y2, char c) {
    for (short i=y1; i<=y2; ++i) {
        GotoXY(x1, i);
        for (short j=x1; j<=x2; ++j) PutChar(c);
    }
}
void PutBorderedRectangle (short x1, short y1, short x2, short y2,
                           char TopLeftCorner, char TopRightCorner, char BottomLeftCorner, char BottomRightCorner,
                           char TopSide, char LeftSide, char RightSide, char BottomSide) {
    GotoXY(x1, y1); PutChar(TopLeftCorner);
    for (short i=x1+1; i<=x2-1; ++i) PutChar(TopSide);
    PutChar(TopRightCorner);
    for (short i=y1+1; i<=y2-1; ++i) {
        GotoXY(x1, i); PutChar(LeftSide);
        GotoXY(x2, i); PutChar(RightSide);
    }
    GotoXY(x1, y2); PutChar(BottomLeftCorner);
    for (short i=x1+1; i<=x2-1; ++i) PutChar(BottomSide);
    PutChar(BottomRightCorner);
}

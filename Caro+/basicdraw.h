#ifndef _BASICDRAW_H_
#define _BASICDRAW_H_

namespace Color {
    const int Black = 0,
              Blue  = 1,
              Green = 2,
              Cyan  = 3,
              Red   = 4,
              Magenta = 5,
              Yellow = 6,
              LightGray = 7,
              DarkGray = 8,
              LightBlue = 9,
              LightGreen = 10,
              LightCyan = 11,
              LightRed = 12,
              LightMagenta = 13,
              LightYellow = 14,
              White = 15;
};

void GotoXY (short x, short y);
void SetColor (short BackgroundColor, short ForegroundColor);
void PutChar (char c);
void PutString (const char* s);
void PutFilledRectangle (short x1, short y1, short x2, short y2, char c);
void PutBorderedRectangle (short x1, short y1, short x2, short y2,
                           char TopLeftCorner='/', char TopRightCorner='\\', char BottomLeftCorner='\\', char BottomRightCorner='/',
                           char TopSide='-', char LeftSide='|', char RightSide='|', char BottomSide='-');


#endif // _BASICDRAW_H_

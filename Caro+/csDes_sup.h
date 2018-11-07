#ifndef _CONSOLE_DESIGNER_SUPPORT_
#define _CONSOLE_DESIGNER_SUPPORT_

#include "globalVars.h"

void ReadCsDesImage (const char * fileName, int & width, int & height,
                     short background[], short foreground[], char ascii[]);
void PutCsDesImage (short x, short y, int width, int height, short background[], short foreground[], char ascii[]);

#endif // _CONSOLE_DESIGNER_SUPPORT_

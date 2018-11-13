#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "dataStruct.h"

bool isLegalMove (AMove Move);
bool isWinningMove (AMove Move);

void NextMove (int ScreenMode, short &x, short &y);
void Expand (short x, short y, CoordList &The_List, short radius);

#endif // _LOGIC_H_

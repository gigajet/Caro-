#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "dataStruct.h"

///Nước đi vào tọa độ (x,y) có hợp lệ hay không?
bool isLegalMove (AMove Move);
///Nước đi vào tọa độ (x,y) có phải nước đi thắng hay không?
bool isWinningMove (AMove Move);

#endif // _LOGIC_H_

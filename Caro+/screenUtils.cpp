#include "screenUtils.h"
#include "windows.h"
#include "globalVars.h"
#include <cstdio>

void AdjustScreenSize (short x, short y) {
    //HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    char cmd[128];
    sprintf(cmd,"mode %d,%d",x,y);
    system(cmd);
    SetConsoleCP(DEFAULT_CODE_PAGE);
    SetConsoleOutputCP(DEFAULT_CODE_PAGE);
}
void ClearScreen () {
    system("cls");
}

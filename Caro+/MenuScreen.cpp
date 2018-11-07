#include "MenuScreen.h"
#include "globalVars.h"
#include "csDes_sup.h"
#include "screenUtils.h"
#include "basicdraw.h"
#include <windows.h>
#include <cstdio>
#include <winuser.h>

void SplashScreen() {
    AdjustScreenSize(110, 35);
    SetColor(Color::Black, Color::White);
    ClearScreen();
    short bg[5000], fg[5000]; char as[5000];
    int width, height;
    ReadCsDesImage("splashScreen.dat",width,height,bg,fg,as);
    PutCsDesImage(0,0,width,height,bg,fg,as);

    while (!GetAsyncKeyState(VK_RETURN)) ;
    return;
}

void MainMenu_UpdateCursor (int oldCursor, int newCursor) {
    SetColor(Color::Black, Color::White);
    if (oldCursor != -1) {
        GotoXY(0, oldCursor+3);
        printf("%s",MainMenu_Options_Description[oldCursor]);
    }
    if (newCursor != -1) {
        GotoXY(0, newCursor+3);
        SetColor(Color::Green, Color::LightYellow);
        printf("%s",MainMenu_Options_Description[newCursor]);
    }
}

int MainMenuScreen (int optionCursor) {
    SetColor(Color::Black, Color::White);
    ClearScreen(); GotoXY(0,0);
    printf("----------------------MAIN MENU-----------------------\n");
    printf("Press Enter to choose an item.\n\n");

    for (int i=0; i<MainMenu_Num_Options; ++i)
        printf("%s\n",MainMenu_Options_Description[i]);
    MainMenu_UpdateCursor(-1, optionCursor);

    INPUT_RECORD InputRecord;
    DWORD numEventRead;

    ///Wait until VK_RETURN release.
    while (!GetAsyncKeyState(VK_RETURN)) ;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    while (true) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &numEventRead);
        if (InputRecord.EventType==KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown==true) {
            switch (InputRecord.Event.KeyEvent.wVirtualKeyCode) {
                case 0x31:
                    MainMenu_UpdateCursor(optionCursor, 0); optionCursor=0; break;
                case 0x32:
                    MainMenu_UpdateCursor(optionCursor, 1); optionCursor=1;break;
                case 0x33:
                    MainMenu_UpdateCursor(optionCursor, 2); optionCursor=2;break;
                case 0x34:
                    MainMenu_UpdateCursor(optionCursor, 3); optionCursor=3;break;
                case 0x35:
                    MainMenu_UpdateCursor(optionCursor, 4); optionCursor=4;break;
                case 0x36:
                    MainMenu_UpdateCursor(optionCursor, 5); optionCursor=5;break;
                case 0x37:
                    MainMenu_UpdateCursor(optionCursor, 6); optionCursor=6;break;
                case 0x38:
                    MainMenu_UpdateCursor(optionCursor, 7); optionCursor=7;break;
                case VK_UP: if (optionCursor>0) {
                        MainMenu_UpdateCursor(optionCursor, optionCursor-1);
                        optionCursor--;
                    } else {
                        MainMenu_UpdateCursor(optionCursor, MainMenu_Num_Options-1);
                        optionCursor = MainMenu_Num_Options-1;
                    }; break;
                case VK_DOWN: if (optionCursor<MainMenu_Num_Options-1) {
                        MainMenu_UpdateCursor(optionCursor, optionCursor+1);
                        optionCursor++;
                    } else {
                        MainMenu_UpdateCursor(optionCursor, 0);
                        optionCursor = 0;
                    }; break;
                case VK_RETURN: {
                    return optionCursor;
                }
                default: {
                }
            }
        }
    }
    return -1;
}

void Statistic_Draw_Chart (int x, int y, int numberOfComponent, int component[], int componentColor[], const char * componentDescription[]) {
    int sumOfComponents = 0; for (int i=0; i<numberOfComponent; ++i) sumOfComponents+=component[i];
    int percent[numberOfComponent]; for (int i=0; i<numberOfComponent; ++i) percent[i]=0;

    if (sumOfComponents > 0) {
        int percent_nay_gio=0;
        for (int i=0; i<numberOfComponent-1; ++i) {
            percent[i] = 1.0*component[i]/sumOfComponents*100;
            percent_nay_gio += percent[i];
        }
        percent[numberOfComponent-1] = 100-percent_nay_gio;

        GotoXY(x,y);
        int totalPrinted = 0, curX=x, curY=y;
        for (int i=0; i<numberOfComponent; ++i) {
            for (int j=1; j<=percent[i]; ++j) {
                SetColor(componentColor[i], (componentColor[i]+1<16)?componentColor[i]+1:0);

                for (int k=1; k<=One_Percent_Y_Graphical_Size; ++k) {
                    for (int m=1; m<=One_Percent_X_Graphical_Size; ++m)
                        PutChar(' ');
                    curY++;
                    GotoXY(curX, curY);
                }
                totalPrinted++;
                if (totalPrinted % 10 == 0) {
                    curY=y; curX+=One_Percent_X_Graphical_Size;
                    GotoXY(curX, curY);
                }
                else {
                }
            }
        }
    }
    else {
        SetColor(Color::Black, Color::Yellow);
        PutBorderedRectangle(x,y,x+10*One_Percent_X_Graphical_Size,y+10*One_Percent_Y_Graphical_Size,
                             218, 191, 192, 217, 196, 179, 179, 196);
        GotoXY(x+1*One_Percent_X_Graphical_Size, y+4*One_Percent_Y_Graphical_Size);
        printf("No match played.");
        GotoXY(x+1*One_Percent_X_Graphical_Size, y+5*One_Percent_Y_Graphical_Size);
        printf("So no statistic available.");
    }

    ///Chú thích
    for (int i=0; i<numberOfComponent; ++i) {
        GotoXY(x, y+One_Percent_Y_Graphical_Size*11+1+2*i);
        SetColor(componentColor[i], (componentColor[i]+1<16)?componentColor[i]+1:0);
        PutChar(' ');
        SetColor(Color::Black, Color::White);
        PutChar(' ');
        PutString(componentDescription[i]);
        printf(" : %d (%d\%%)",component[i],percent[i]);
    }
}

void StatisticScreen () {
    SetColor(Color::Black, Color::White); ClearScreen();
    GotoXY(2,1); printf("Statistic (Press Enter to return to main menu)");

    GotoXY(2,3); printf("PvP matches");
    int componentPvP[] = {Number_of_PvP_P1_Wins,Number_of_PvP_P2_Wins,Number_of_PvP_Draws};
    int colorPvP[] = {Color::LightGreen, Color::LightYellow, Color::LightGray};
    Statistic_Draw_Chart(2,5,3,componentPvP,colorPvP,Legend_PvP_Description);

    SetColor(Color::Black, Color::White); GotoXY(50,3); printf("PvC matches");
    int componentPvC[] = {Number_of_PvC_Wins, Number_of_PvC_Loses, Number_of_PvC_Draws};
    int colorPvC[] = {Color::LightGreen, Color::LightYellow, Color::LightGray};
    Statistic_Draw_Chart(50,5,3,componentPvC,colorPvC,Legend_PvC_Description);

    fflush(stdin); fgetc(stdin);
}

void DataScreen () {

}
void OptionScreen () {
}
void PvPScreen () {
}
void PvCScreen() {
}

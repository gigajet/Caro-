#include <conio.h>

#include "globalVars.h"
#include "basicdraw.h"
#include "MenuScreen.h"
#include <cstdio>

int main()
{
    SplashScreen();
    Option_InitializeOption(Option_File);
    Stat_Load();
    Data_InitializeSlots();

    while (Exit_Signal == false) {
        int choice = MainMenuScreen(0);
        switch (choice) {
            case 0: { //PvP
                Screen_Mode = 0;
                PlayScreen(1);
                break;
            }
            case 1: { //PvC Easy
                Screen_Mode = 1;
                PlayScreen(1);
                break;
            }
            case 2: { //PvC Norm
                Screen_Mode = 2;
                PlayScreen(1);
                break;
            }
            case 3: { //PvC Hard
                Screen_Mode = 3;
                PlayScreen(1);
                break;
            }
            case 4: { //Data
                if (DataScreen(0)) {
                    PlayScreen(0);
                }
                break;
            }
            case 5: StatisticScreen(); break;
            case 6: OptionScreen(); break;
            case 7: Exit_Signal=true; break;
        }
    }

    Data_FinalizeSlots();
    Stat_Save();
    Option_FinalizeOption(Option_File);
    return 0;
}

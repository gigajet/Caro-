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
            case 0: PvPScreen(1); break;
            case 1: PvCScreen(); break;
            case 2: break;
            case 3: break;
            case 4: DataScreen(0); break;
            case 5: StatisticScreen(); break;
            case 6: OptionScreen(); break;
            case 7: Exit_Signal=true; break;
        }
    }

    Data_FinalizeSlots(); //Do we really need this?
    Stat_Save();
    return 0;
}

#include <conio.h>

#include "globalVars.h"
#include "basicdraw.h"
#include "MenuScreen.h"
#include <cstdio>

int main()
{
    SplashScreen();


    Number_of_PvP_P1_Wins = 23;
    Number_of_PvP_P2_Wins = 34;
    Number_of_PvP_Draws = 47;

    while (Exit_Signal == false) {
        int choice = MainMenuScreen(0);
        switch (choice) {
            case 0: break;
            case 1: break;
            case 2: break;
            case 3: break;
            case 4: DataScreen(); break;
            case 5: StatisticScreen(); break;
            case 6: OptionScreen(); break;
            case 7: Exit_Signal=true; break;
        }
    }
    return 0;
}

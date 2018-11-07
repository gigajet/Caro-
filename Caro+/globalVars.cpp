#include "globalVars.h"

const int MAX_BOARD_LOGICAL_SIZE = 20;
const int DEFAULT_BOARD_LOGICAL_SIZE = 15;
const int DEFAULT_BOARD_CELL_GRAPHICAL_SIZE = 3;

const int MAX_SCREEN_GRAPHICAL_WIDTH = 80;
const int MAX_SCREEN_GRAPHICAL_HEIGHT = 60;

const int DEFAULT_CODE_PAGE = 850;

int Board_Logical_Size;
int Board_Cell_Graphical_Size;


const int MainMenu_Num_Options = 8; ///from 0 to MainMenu_Num_Options-1
const char* MainMenu_Options_Description[MainMenu_Num_Options] =
                                {"1. New Game (PvP)         ",
                                 "2. New Game (PvC - Easy)  ",
                                 "3. New Game (PvC - Normal)",
                                 "4. New Game (PvC - Hard)  ",
                                 "5. Load Game              ",
                                 "6. Statistic              ",
                                 "7. Options                ",
                                 "8. Exit                   "};
bool Exit_Signal = false;


int Number_of_PvP_P1_Wins, Number_of_PvP_P2_Wins, Number_of_PvP_Draws;
int Number_of_PvC_Wins, Number_of_PvC_Loses, Number_of_PvC_Draws;
int One_Percent_X_Graphical_Size = 2, One_Percent_Y_Graphical_Size = 2;

const char * Legend_PvP_Description[3] = {"Player 1 win",
                                    "Player 2 win",
                                    "Draw"};
const char * Legend_PvC_Description[3] = {"Player win",
                                    "Computer win",
                                    "Draw"};

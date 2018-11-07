#ifndef _GLOBALVARS_H_
#define _GLOBALVARS_H_

extern const int MAX_BOARD_LOGICAL_SIZE;
extern const int DEFAULT_BOARD_LOGICAL_SIZE;
extern const int DEFAULT_BOARD_CELL_GRAPHICAL_SIZE;

extern const int MAX_SCREEN_GRAPHICAL_WIDTH;
extern const int MAX_SCREEN_GRAPHICAL_HEIGHT;

extern const int DEFAULT_CODE_PAGE;

extern int Board_Logical_Size;
extern int Board_Cell_Graphical_Size;

extern const int MainMenu_Num_Options;
extern const char* MainMenu_Options_Description[];

extern bool Exit_Signal;

extern int Number_of_PvP_P1_Wins, Number_of_PvP_P2_Wins, Number_of_PvP_Draws;
extern int Number_of_PvC_Wins, Number_of_PvC_Loses, Number_of_PvC_Draws;
extern int One_Percent_X_Graphical_Size, One_Percent_Y_Graphical_Size;
extern const char * Legend_PvP_Description[3];
extern const char * Legend_PvC_Description[3];

#endif // _GLOBALVARS_H_

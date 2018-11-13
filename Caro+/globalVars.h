#ifndef _GLOBALVARS_H_
#define _GLOBALVARS_H_

#include "dataStruct.h"

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

extern const int TIME_CONSTRAINT_MODE;
extern const int PLACE_STONE_MODE;
extern const int ISOLETE_MODE;
extern const int CARO_MODE;
extern int Current_Game_Mode;

extern short Board_Cell_Color_1;
extern short Board_Cell_Color_2;
extern short Board_Cell_Selecting_Color_1;
extern short Board_Cell_Selecting_Color_2;
extern short Board_Move_Color_1;
extern short Board_Move_Color_2;
extern short Board_Move_Color_0;
extern short Board_Move_Color_S;

extern short Board_Border_Color;

extern char Board[];
//#define board(w,h) Board[(h)*MAX_BOARD_LOGICAL_SIZE+(w)]

char &board(short w, short h);

extern char Board_Move_Icon_1[];
extern char Board_Move_Icon_2[];
extern char Board_Move_Icon_0[];

extern const int MAX_NAME_LENGTH;
extern short Current_Turn;
extern char Player_1_Name[];
extern char Player_2_Name[];

extern int Num_Last_Move;

extern MoveList Move_List;
extern int Player_1_Stone;
extern int Player_2_Stone;

extern int Illegal_Move_Delay;
extern int Illegal_Move_Color;

extern short Board_Current_Cursor_X;
extern short Board_Current_Cursor_Y;

extern bool P1_Move_First;
extern int Board_Num_Cell_Placed;

extern const int Num_Initial_Stone;

extern const char * Stat_File_Path;

extern const int Num_Data_Slot;
extern const char * Slot_Data_File [];
extern GameData Slot_Data[];
extern bool Slot_Non_Empty[];

extern int Screen_Mode;

extern const char * Option_File;

extern const int Slot_Width;
extern const int Slot_Height;

extern bool Enable_Undo;

//equals 1 or 2, indicates that player is Computer, not a player.
extern short Computer_Player;
extern CoordList Child_List;
extern bool Already_in_Child_List[];

#endif // _GLOBALVARS_H_

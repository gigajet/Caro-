#include "globalVars.h"
#include "basicdraw.h"

const int MAX_BOARD_LOGICAL_SIZE = 20;
const int DEFAULT_BOARD_LOGICAL_SIZE = 15;
const int DEFAULT_BOARD_CELL_GRAPHICAL_SIZE = 2;

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


const int TIME_CONSTRAINT_MODE = 1;
const int PLACE_STONE_MODE = 2;
const int ISOLETE_MODE = 4;
const int CARO_MODE = 8;
int Current_Game_Mode;

short Board_Cell_Color_1 = Color::Blue;
short Board_Cell_Color_2 = Color::Black;
short Board_Cell_Selecting_Color_1 = Color::LightBlue;
short Board_Cell_Selecting_Color_2 = Color::LightGray;
short Board_Move_Color_1 = Color::LightGreen;
short Board_Move_Color_2 = Color::LightYellow;
short Board_Move_Color_0 = Color::White;
short Board_Move_Color_S = Color::Red;

///Sẽ dùng nét đôi vẽ rìa bàn cờ.
short Board_Border_Color = Color::Magenta;

///(0,0), (1,0), (2,0), (3,0) ...
///(1,0), ...
///Ô (w,h) sẽ nằm ở h*MAX_BOARD_LOGICAL_SIZE+w;
char Board[MAX_BOARD_LOGICAL_SIZE*MAX_BOARD_LOGICAL_SIZE];

///Biểu tượng của 2 người chơi 1 và 2.
///Ô (xRel, yRel) của đồ họa nằm ở yRel*DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+xRel;
char Board_Move_Icon_1[DEFAULT_BOARD_CELL_GRAPHICAL_SIZE*DEFAULT_BOARD_CELL_GRAPHICAL_SIZE] = {(char)218,(char)191,
                                                                                            (char)192,(char)217};
char Board_Move_Icon_2[DEFAULT_BOARD_CELL_GRAPHICAL_SIZE*DEFAULT_BOARD_CELL_GRAPHICAL_SIZE] = {'/','\\','\\','/'};

///Không ai đi vào đây.
char Board_Move_Icon_0[DEFAULT_BOARD_CELL_GRAPHICAL_SIZE*DEFAULT_BOARD_CELL_GRAPHICAL_SIZE] = {' ',' ',' ',' '};

///Lượt hiện tại, nếu là người chơi 1, thì bằng 1, ngược lại bằng 2.
const int MAX_NAME_LENGTH = 10;
int Current_Turn;
///Tên người chơi 1
char Player_1_Name [MAX_NAME_LENGTH] = {"Gryyna\0"};
///Tên người chơi 2
char Player_2_Name [MAX_NAME_LENGTH] = {"Yellos\0"};

///Số nước đi cuối được viết lên màn hình
int Num_Last_Move = 10;

///Danh sách nước đi, từ đầu tới cuối.
MoveList Move_List;
int Player_1_Stone;
int Player_2_Stone;

///Viết chữ "Illegal move" rồi đơ màn hình bao lâu? (đơn vị mili-giây)
///Sau khi đơ xong thì xóa luôn.
int Illegal_Move_Delay = 500;

int Illegal_Move_Color = Color::LightRed;

///Tọa độ logic con trỏ X
int Board_Current_Cursor_X;
///Tọa độ logic con trỏ Y
int Board_Current_Cursor_Y;

bool P1_Move_First = 1;

///Số ô đã được đánh dấu. Dùng biến này xét xem game có kết thúc hòa?
int Board_Num_Cell_Placed;

const int Num_Initial_Stone = 3;

const char * Stat_File_Path = "stat.dat";

const int Num_Data_Slot = 4;
const char * Slot_Data_File [Num_Data_Slot] = {"data00.dat",
                                               "data01.dat",
                                               "data02.dat",
                                               "data03.dat"};
GameData Slot_Data[Num_Data_Slot];
bool Slot_Non_Empty[Num_Data_Slot];

int Screen_Mode;
const char * Option_File = "option.dat";

const int Slot_Width = 20;
const int Slot_Height = 6;

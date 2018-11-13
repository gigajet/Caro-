#include "MenuScreen.h"
#include "globalVars.h"
#include "csDes_sup.h"
#include "screenUtils.h"
#include "basicdraw.h"
#include <windows.h>
#include <cstdio>
#include <winuser.h>
#include <cctype>
#include "logic.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>

using namespace std;

void SplashScreen() {
    AdjustScreenSize(110, 35);
    SetColor(Color::Black, Color::White);
    ClearScreen();
    short bg[5000], fg[5000]; char as[5000];
    int width, height;
    ReadCsDesImage("splashScreen.dat",width,height,bg,fg,as);
    PutCsDesImage(0,0,width,height-1,bg,fg,as);

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
    AdjustScreenSize(80,24);
    SetColor(Color::Black, Color::White);
    ClearScreen(); GotoXY(0,0);
    printf("----------------------MAIN MENU-----------------------\n");
    printf("Press Enter to choose an item.\n\n");

    for (int i=0; i<MainMenu_Num_Options; ++i)
        printf("%s\n",MainMenu_Options_Description[i]);
    MainMenu_UpdateCursor(-1, optionCursor);

    INPUT_RECORD InputRecord;
    DWORD numEventRead;

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
    AdjustScreenSize(80,35);
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


///Vẽ bảng và số chỉ dòng, cột.
///(x,y) là số chỉ góc trái trên của BẢNG.
void Play_DrawBoard (int x, int y) {
    GotoXY(x,y);
    SetColor(Color::Black, Board_Border_Color);
    PutBorderedRectangle(x,y,x+1+Board_Cell_Graphical_Size*Board_Logical_Size, y+1+Board_Cell_Graphical_Size*Board_Logical_Size,
                         218, 191, 192, 217, 196, 179, 179, 196);
    for (int w=0; w<Board_Logical_Size; ++w)
    for (int h=0; h<Board_Logical_Size; ++h) {
        if ((w+h)%2 == 0) {
            SetColor(Board_Cell_Color_1, (Board_Cell_Color_1+1<16)?Board_Cell_Color_1+1:0);
        }
        else {
            SetColor(Board_Cell_Color_2, (Board_Cell_Color_2+1<16)?Board_Cell_Color_2+1:0);
        }
        short curX=x+1+Board_Cell_Graphical_Size*w, curY=y+1+Board_Cell_Graphical_Size*h;
        GotoXY(curX, curY);
        for (int i=0; i<Board_Cell_Graphical_Size; ++i) {
            for (int j=0; j<Board_Cell_Graphical_Size; ++j)
                PutChar(' ');
            curY++; GotoXY(curX, curY);
        }
    }
}
///Vẽ số chỉ dòng.
void Play_DrawRowIndicator (int x, int y) {
    {
        SetColor(Color::Black, Color::DarkGray);
        int curX=x, curY=y;
        GotoXY(curX, curY);
        for (int i=0; i<Board_Logical_Size; ++i) {
            printf("%2d",i+1);
            curY+=Board_Cell_Graphical_Size;
            GotoXY(curX, curY);
        }
    }
}
///Vẽ số chỉ cột
void Play_DrawColIndicator (int x, int y) {
    {
        SetColor(Color::Black, Color::DarkGray);
        int curX=x, curY=y;
        GotoXY(curX, curY);
        for (int i=0; i<Board_Logical_Size; ++i) {
            printf("%c ",i+'a');
            curX+=Board_Cell_Graphical_Size;
            GotoXY(curX,curY);
        }
    }
}

///Lấy tọa độ trái trên của ô (w,h) của bảng có góc trái trên là (x,y)
///Nhắc lại: Tọa độ bảng tính từ 0.
void Play_GetGraphicalCoordinate (int x, int y, int w, int h, int &xOut, int &yOut) {
    ///Cộng 1? Tại bức tường!
    xOut = x+1+Board_Cell_Graphical_Size*w;
    yOut = y+1+Board_Cell_Graphical_Size*h;
}

///Cập nhật đồ họa tại 1 ô.
///Last argument is for the board.
void Play_UpdateCell (int x, int y, int w, int h, bool isSelecting, char b[] = Board) {
    short BG, FG;
    char * MovIcon;
    ///Find Background
    if (toupper(b[h*MAX_BOARD_LOGICAL_SIZE+w])=='S') BG=Board_Move_Color_S;
    else if ((w+h)%2==0) {
        BG = (isSelecting)?Board_Cell_Selecting_Color_1:Board_Cell_Color_1;
    }
    else BG = (isSelecting)?Board_Cell_Selecting_Color_2:Board_Cell_Color_2;

    ///Find foreground and MovIcon
    if (b[h*MAX_BOARD_LOGICAL_SIZE+w]=='1') {
        FG = Board_Move_Color_1; MovIcon = Board_Move_Icon_1;
    }
    else if (b[h*MAX_BOARD_LOGICAL_SIZE+w]=='2') {
        FG = Board_Move_Color_2; MovIcon = Board_Move_Icon_2;
    }
    else {
        FG = Board_Move_Color_0; MovIcon = Board_Move_Icon_0;
    }

    SetColor(BG, FG);
    int curX, curY;
    Play_GetGraphicalCoordinate(x,y,w,h,curX,curY);
    GotoXY(curX, curY);
    int curCount=0;
    for (int i=0; i<DEFAULT_BOARD_CELL_GRAPHICAL_SIZE; ++i) {
        for (int j=0; j<DEFAULT_BOARD_CELL_GRAPHICAL_SIZE; ++j) PutChar(MovIcon[curCount++]);
        curY++; GotoXY(curX, curY);
    }
}

///Cập nhật số chỉ dòng
///Hai tham số đầu là tọa độ góc trái trên của số chỉ dòng.
///Số chỉ từ 0, nhưng in (cho đẹp) từ ... 1
void Play_UpdateRowIndicator (int x, int y, int OldRowIndicator, int NewRowIndicator) {
    if (OldRowIndicator>=0 && OldRowIndicator<Board_Logical_Size) {
        int curX=x, curY=y+OldRowIndicator*Board_Cell_Graphical_Size;
        GotoXY(curX, curY); SetColor(Color::Black, Color::DarkGray);
        printf("%2d\n  ",OldRowIndicator+1);
    }
    if (NewRowIndicator>=0 && NewRowIndicator<Board_Logical_Size) {
        int curX=x, curY=y+NewRowIndicator*Board_Cell_Graphical_Size;
        GotoXY(curX, curY); SetColor(Color::Cyan, Color::LightYellow);
        printf("%2d\n  ",NewRowIndicator+1);
    }
}

///Cập nhật số chỉ cột
///Hai tham số đầu là tọa độ góc trái trên của số chỉ cột.
///Số chỉ từ a, b, ..., tối đa z.
///Nhưng xem a=0, b=1, ...
void Play_UpdateColIndicator (int x, int y, int OldColIndicator, int NewColIndicator) {
    if (OldColIndicator>=0 && OldColIndicator<Board_Logical_Size) {
        int curX=x+OldColIndicator*Board_Cell_Graphical_Size, curY=y;
        GotoXY(curX, curY); SetColor(Color::Black, Color::DarkGray);
        printf("%c ",OldColIndicator+'a');
    }
    if (NewColIndicator>=0 && NewColIndicator<Board_Logical_Size) {
        int curX=x+NewColIndicator*Board_Cell_Graphical_Size, curY=y;
        GotoXY(curX, curY); SetColor(Color::Cyan, Color::LightYellow);
        printf("%c ",NewColIndicator+'a');
    }
}
///Viết hướng dẫn cho người chơi
void Play_DrawInstruction (int x, int y) {
    int curX=x, curY=y; GotoXY(curX, curY); SetColor(Color::Black, Color::White);
    PutString("Arrow keys: Move cursor");
    curY++; GotoXY(curX, curY); //next line
    PutString("Space:      Make a move");
    curY++; GotoXY(curX, curY); //next line

    if (Current_Game_Mode & PLACE_STONE_MODE) {
    PutString("Tab: Place a stone (if any)");
        curY++; GotoXY(curX, curY); //next line
    }

    if (Enable_Undo) {
        PutString("U:                 Undo");
        curY++; GotoXY(curX, curY);
    }
    PutString("D:     Save / Load game");
    curY++; GotoXY(curX, curY); //next line
    PutString("M:    Exit to Main Menu");
    curY++; GotoXY(curX, curY); //next line
    PutString("ESC:          Exit Game");
    curY++; GotoXY(curX, curY); //next line
}
///Vẽ chỉ lượt
void Play_DrawTurnIndicator (int x, int y, bool DrawStoneLeft=0) {
    int curX=x, curY=y;
    ///Draw for player 1
    SetColor(Color::Black, Board_Move_Color_1);
    for (int i=0; i<DEFAULT_BOARD_CELL_GRAPHICAL_SIZE; ++i)
        for (int j=0; j<DEFAULT_BOARD_CELL_GRAPHICAL_SIZE; ++j) {
            GotoXY(curX+j, curY+i);
            PutChar(Board_Move_Icon_1[i*DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+j]);
        }
    GotoXY(curX+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+1, curY);
    PutString(Player_1_Name);
    if (DrawStoneLeft) {
        GotoXY(curX+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+1, curY+1);
        printf("%d stone(s) left",Player_1_Stone);
    }

    ///Draw middle section
    curY+=DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+3; SetColor(Color::Black, Color::White);
    GotoXY(curX, curY);
    PutString(" TURN OF ");

    curY+=3+1;
    ///Draw for player 2
    SetColor(Color::Black, Board_Move_Color_2);
    for (int i=0; i<DEFAULT_BOARD_CELL_GRAPHICAL_SIZE; ++i)
        for (int j=0; j<DEFAULT_BOARD_CELL_GRAPHICAL_SIZE; ++j) {
            GotoXY(curX+j, curY+i);
            PutChar(Board_Move_Icon_2[i*DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+j]);
        }
    GotoXY(curX+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+1, curY+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE/2);
    PutString(Player_2_Name);
    if (Screen_Mode>0) PutString (" (Computer)");
    if (DrawStoneLeft) {
        GotoXY(curX+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+1, curY+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE/2+1);
        printf("%d stone(s) left",Player_2_Stone);
    }
}
///Hai tham số đầu là tọa độ của góc trái trên của phần chỉ lượt.
void Play_UpdateTurnIndicator (int x, int y, int OldTurnIndicator, int NewTurnIndicator) {
    int curX=x+4, curY=y; SetColor(Color::Black, Color::White);
    if (OldTurnIndicator>=1 && OldTurnIndicator<=2) {
        if (OldTurnIndicator==1) {
            GotoXY(curX, curY+2); PutChar(' ');
            GotoXY(curX, curY+3); PutChar(' ');
            GotoXY(curX, curY+4); PutChar(' ');
        }
        else {
            GotoXY(curX, curY+6); PutChar(' ');
            GotoXY(curX, curY+7); PutChar(' ');
            GotoXY(curX, curY+8); PutChar(' ');
        }
    }
    if (NewTurnIndicator>=1 && NewTurnIndicator<=2) {
        if (NewTurnIndicator==1) {
            GotoXY(curX, curY+2); PutChar('^');
            GotoXY(curX, curY+3); PutChar('|');
            GotoXY(curX, curY+4); PutChar('|');
        }
        else {
            GotoXY(curX, curY+6); PutChar('|');
            GotoXY(curX, curY+7); PutChar('|');
            GotoXY(curX, curY+8); PutChar('v');
        }
    }
}
///Chuyển tọa độ 2 số về tọa độ đọc được trên bảng
void Play_ConvertToReadableCoord (int w, int h, char out[]) {
    char tw = w+'a'; int th=h+1;
    sprintf(out,"%c%d",tw,th);
}
///Vẽ chỉ nước đi gần đây.
void Play_DrawLastMove(int x, int y) {
    int curX=x, curY=y; SetColor(Color::Black, Color::White);
    PutFilledRectangle(curX, curY, curX+MAX_NAME_LENGTH+16, curY+Num_Last_Move+2,' ');
    GotoXY(curX, curY); PutString("LAST MOVE(S)");
    curY++; GotoXY(curX, curY); for (int i=1; i<=16; ++i) PutChar(238);
    int printed = 0; char crd[7];
    for (int i=(int)Move_List.size()-1; i>=0 && printed<Num_Last_Move; --i) {
        curY++; GotoXY(curX, curY);
        if (Move_List[i].Maker == 1) {
            SetColor(Color::Black, Board_Move_Color_1);
            printf("%-3d ",i+1);
            printf("%-*s",MAX_NAME_LENGTH,Player_1_Name);
            PutString("  ");
            Play_ConvertToReadableCoord(Move_List[i].x, Move_List[i].y, crd);
            PutString(crd);

            if (!Move_List[i].TurnLost) {
                PutString(" (stone)");
            }
        }
        else if (Move_List[i].Maker == 2) {
            SetColor(Color::Black, Board_Move_Color_2);
            printf("%-3d ",i+1);
            printf("%-*s",MAX_NAME_LENGTH,Player_2_Name);
            PutString("  ");
            Play_ConvertToReadableCoord(Move_List[i].x, Move_List[i].y, crd);
            PutString(crd);

            if (!Move_List[i].TurnLost) {
                PutString(" (stone)");
            }
        }
        printed++;
    }
}
///Vẽ chỉ chế độ game hiện tại.
void Play_DrawGameModeIndicator (int x, int y) {
    int curX=x, curY=y; GotoXY(curX, curY);
    SetColor(Color::Black, Color::White);
    PutString("GAME MODE"); curY++;
    GotoXY(curX, curY); for (int i=1; i<=16; ++i) PutChar(238);

    if (Current_Game_Mode & TIME_CONSTRAINT_MODE) {
        curY++; GotoXY(curX, curY);
        PutString("+ Time Constraint");
    }
    if (Current_Game_Mode & PLACE_STONE_MODE) {
        curY++; GotoXY(curX, curY);
        PutString("+ Stone mode");
    }
    if (Current_Game_Mode & ISOLETE_MODE) {
        curY++; GotoXY(curX, curY);
        PutString("+ Isolete mode");
    }
    if (Current_Game_Mode & CARO_MODE) {
        curY++; GotoXY(curX, curY);
        PutString("+ Caro mode");
    }
    else {
        curY++; GotoXY(curX, curY);
        PutString("+ Gomoku mode");
    }
}
///Vẽ chỉ nước đi phạm luật
void Play_DrawIllegalMoveIndicator (int x, int y) {
    GotoXY(x,y); SetColor(Color::Black, Illegal_Move_Color);
    PutString("ILLEGAL MOVE!");
    Sleep((DWORD)Illegal_Move_Delay);
    GotoXY(x,y);
    PutString("             ");
}
void Play_ClearLogicalBoard () {
    for (int h=0; h<Board_Logical_Size; ++h)
    for (int w=0; w<Board_Logical_Size; ++w)
        Board[h*MAX_BOARD_LOGICAL_SIZE+w] = '.';
}
void Play_WinningAnimation (int winner) {
    using namespace std;
    AdjustScreenSize(110,35);
    SetColor(Color::Black, Color::White); ClearScreen();
    short bg[5000], fg[5000]; char ascii[5000]; int width, height;

    if (winner == 1) {
        ReadCsDesImage("p1win.dat",width,height,bg,fg,ascii);
        PutCsDesImage(0,0,width,height-1,bg,fg,ascii);
        GotoXY(24,30); SetColor(Color::Black, Color::LightYellow);
        cout<<"Congratulation! Press Enter to return to main menu..."<<endl;
    }
    else {
        ReadCsDesImage("p2win.dat",width,height,bg,fg,ascii);
        PutCsDesImage(0,0,width,height-1,bg,fg,ascii);
        GotoXY(24,30); SetColor(Color::Black, Color::LightGreen);
        cout<<"Congratulation! Press Enter to return to main menu..."<<endl;
    }
    fflush(stdin); fgetc(stdin);
}
void Play_DrawAnimation () {
    using namespace std;
    AdjustScreenSize(110,35);
    SetColor(Color::Black, Color::White); ClearScreen();
    short bg[5000], fg[5000]; char ascii[5000]; int width, height;

    ReadCsDesImage("drawanim.dat",width,height,bg,fg,ascii);
    PutCsDesImage(0,0,width,height-1,bg,fg,ascii);
    GotoXY(24,30); SetColor(Color::Black, Color::LightCyan);
    cout<<"Game ended in a draw. Press Enter to return to main menu..."<<endl;
    fflush(stdin); fgetc(stdin);
}
void Play_InitializeNewGame () {
    Current_Turn = (P1_Move_First)?1:2;
    Play_ClearLogicalBoard();
    Move_List.clear();
    if (Current_Game_Mode & PLACE_STONE_MODE) {
        Player_1_Stone = Player_2_Stone = Num_Initial_Stone;
    }
    Board_Num_Cell_Placed=0;

    for (int w=0; w<Board_Logical_Size; ++w)
        for (int h=0; h<Board_Logical_Size; ++h)
            Already_in_Child_List[h*MAX_BOARD_LOGICAL_SIZE+w] = 0;
}
void Play_ReloadGraphics (int boardx, int boardy) {
    using namespace std;
    AdjustScreenSize(110, (Board_Logical_Size==20)?45:36);
    SetColor(Color::Black, Color::White); ClearScreen();
    Board_Cell_Color_1 = Color::Blue;
    Board_Cell_Color_2 = Color::Black;


    ///Hide the cursor
    CONSOLE_CURSOR_INFO csnfo = {100, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csnfo);

    Play_DrawBoard(boardx,boardy);
    Play_DrawRowIndicator(boardx-2, boardy+1);
    Play_DrawColIndicator(boardx+1, boardy-2);

    Play_DrawInstruction (boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                          max(boardy+Board_Logical_Size*Board_Cell_Graphical_Size/2, 20));

    Play_DrawTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                           boardy+1, Current_Game_Mode & PLACE_STONE_MODE);

    if (Current_Game_Mode&PLACE_STONE_MODE) {
        Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+24,
                    boardy+1);
    }
    else {
        Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+16,
                    boardy+1);
    }

    Play_DrawGameModeIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+(Current_Game_Mode&PLACE_STONE_MODE ? 24 : 16),
                    boardy + Board_Logical_Size*Board_Cell_Graphical_Size/2);

    Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                           boardy+1,-1,Current_Turn);
}
void Play_GraphicsReady (int boardx, int boardy) {
    ///Default coordinate.
    Board_Current_Cursor_X = Board_Current_Cursor_Y = Board_Logical_Size / 2;
    Play_UpdateCell(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y, 1);
    Play_UpdateColIndicator(boardx+1, boardy-2, -1, Board_Current_Cursor_X);
    Play_UpdateRowIndicator(boardx-2, boardy+1, -1, Board_Current_Cursor_Y);
}
void Play_SimulateBoard (int x, int y, const MoveList& Move_List, char b[] = Board) {
    Play_DrawBoard(x,y);
    for (AMove m : Move_List) {
        Board_Num_Cell_Placed++;
        if (!m.TurnLost) { //Stone
            Play_UpdateCell(x,y,m.x,m.y,0);
        }
        else {
            b[m.y*MAX_BOARD_LOGICAL_SIZE+m.x] = (m.Maker==1)?'1':'2';
            Play_UpdateCell(x,y,m.x,m.y,0,b);
        }
        CoordList Expanded;
        Expand(Board_Current_Cursor_X, Board_Current_Cursor_Y, Expanded, 1);
        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
            Child_List.push_back({cEx.x, cEx.y});
        }
    }
}
void Debug_Test () {
//    { ///Test Row indicator
//        Play_UpdateRowIndicator(boardx-2, boardy+1, -1, 5);
//        fflush(stdin); fgetc(stdin);
//        Play_UpdateRowIndicator(boardx-2, boardy+1, 5, 7);
//        fflush(stdin); fgetc(stdin);
//    }

//    { ///Test Col indicator
//        Play_UpdateColIndicator(boardx+1, boardy-2, -1, 5);
//        fflush(stdin); fgetc(stdin);
//        Play_UpdateColIndicator(boardx+1, boardy-2, 5, 7);
//        fflush(stdin); fgetc(stdin);
//    }

//    { ///Test turn indicator
//         Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
//                                boardy+1, -1, 1);
//         fflush(stdin); fgetc(stdin);
//         Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
//                                boardy+1, 1, 2);
//         fflush(stdin); fgetc(stdin);
//         Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
//                                boardy+1, 2, -1);
//         fflush(stdin); fgetc(stdin);
//    }

//    { ///Test last moves
//        Move_List.push_back({0,1,0,0});
//        Move_List.push_back({0,2,1,1});
//        Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
//                          MAX_NAME_LENGTH+15,
//                          boardy+1);
//    }

//    { ///Test Turn indicator (stone mode)
//        Player_1_Stone = 0;
//        Player_2_Stone = 23;
//        Play_DrawTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
//                           boardy+1,1);
//    }

//    { ///Test illegal mode
//        Play_DrawIllegalMoveIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
//                          boardy+Board_Logical_Size*Board_Cell_Graphical_Size*5/6);
//    }
}
void Play_Undo (int boardx, int boardy) {
    /**
    Change:
    - Move_List
    Redraw:
    - Cell at last move
    - Turn indicator
    - Last move list.
    **/
    if (!Move_List.empty()) { //There is things to undo
        AMove m = Move_List.back(); ///Last element
        Move_List.pop_back();

        Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                                boardy+1,Current_Turn,-1);

        if (!m.TurnLost) {
            if (m.Maker==1) {
                Player_1_Stone ++;
            }
            else {
                Player_2_Stone ++;
            }
        }
        else {
            Current_Turn = 3-Current_Turn; //Alt turn
        }

        board(m.x, m.y) = '.'; Board_Num_Cell_Placed--;

        Play_UpdateCell(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y, 0);
        Play_UpdateColIndicator(boardx+1, boardy-2, Board_Current_Cursor_X, m.x);
        Play_UpdateRowIndicator(boardx-2, boardy+1, Board_Current_Cursor_Y, m.y);
        Board_Current_Cursor_X = m.x;
        Board_Current_Cursor_Y = m.y;
        Play_UpdateCell(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y, 1);

        Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                                                    boardy+1,-1,Current_Turn);
        if (Current_Game_Mode&PLACE_STONE_MODE) {
            Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+24,
                    boardy+1);
        }
        else {
            Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+16,
                    boardy+1);
        }
    } //empty
}
void Play_MoveBoardCursor (int boardx, int boardy, int logicx, int logicy,
                           bool UpdateRowIndicator, bool UpdateColIndicator) {
    Play_UpdateCell(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y, 0);
    Play_UpdateCell(boardx, boardy, logicx, logicy, 1);
    if (UpdateRowIndicator) {
        Play_UpdateRowIndicator(boardx-2, boardy+1, Board_Current_Cursor_Y, logicy);
    }
    if (UpdateColIndicator) {
        Play_UpdateColIndicator(boardx+1, boardy-2, Board_Current_Cursor_X, logicx);
    }
    Board_Current_Cursor_X = logicx; Board_Current_Cursor_Y = logicy;
}
void PlayScreen(bool IsANewGame) {
    if (IsANewGame) {
        Play_InitializeNewGame();
    }
    int boardx=2, boardy=2;

    Play_ReloadGraphics(boardx, boardy);
    Play_SimulateBoard(boardx, boardy, Move_List);
    Play_GraphicsReady(boardx, boardy);

    INPUT_RECORD InputRecord; DWORD numEventsRead;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    bool Main_Menu_Signal = 0;

    while (!Exit_Signal && !Main_Menu_Signal) {
        if (Current_Turn == Computer_Player) {
            short x, y;
            NextMove(Screen_Mode, Computer_Player==1, x, y);

            AMove Move = {1, Current_Turn, x, y};
            if (isLegalMove(Move)) {
                ///Đã có người thắng hay chưa?
                ///Thắng, animation, còn thua thì vẫn luân phiên.
                Board_Num_Cell_Placed ++;
                if (isWinningMove(Move)) {
                    #define DEBUG
                    #ifdef DEBUG
                    GotoXY(0,0); SetColor(Color::Black, Color::White);
                    cout<<"Computer moves at "<<x<<" "<<y<<" and then wins";
                    Sleep(1000);
                    #endif // DEBUG
                    Play_WinningAnimation (Current_Turn);
                    Number_of_PvC_Loses ++;
                    Main_Menu_Signal = 1;
                    break;
                }
                else {
                    if (Board_Num_Cell_Placed == Board_Logical_Size*Board_Logical_Size) {
                        Play_DrawAnimation ();
                        Number_of_PvC_Draws ++;
                        Main_Menu_Signal = 1;
                        break;
                    }
                }
                Move_List. push_back(Move);
                board(x, y) = (Current_Turn==1)?'1':'2';
                Play_MoveBoardCursor(boardx, boardy, x, y,1,1);

                CoordList Expanded;
                Expand(Board_Current_Cursor_X, Board_Current_Cursor_Y, Expanded, 1);
                for (Coord cEx : Expanded) {
                    Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
                    Child_List.push_back({cEx.x, cEx.y});
                }

                Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                                        boardy+1,Current_Turn,3-Current_Turn);
                Current_Turn = 3-Current_Turn;

                //Draw last moves
                if (Current_Game_Mode&PLACE_STONE_MODE) {
                    Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+24,
                    boardy+1);
                }
                else {
                    Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                    MAX_NAME_LENGTH+16,
                    boardy+1);
                }
            } //isLegal
        }
        else {
            ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &numEventsRead);
            switch (InputRecord.EventType) {
                case KEY_EVENT: {
                    if (InputRecord.Event.KeyEvent.bKeyDown == 0) break;
                    switch (InputRecord.Event.KeyEvent.wVirtualKeyCode) {
                        case VK_ESCAPE : Exit_Signal=1; break;
                        case 0x4D : Main_Menu_Signal=1; break; //VK_M
                        case 0x44 : { //VK_D
                            DataScreen(1);
                            Play_ReloadGraphics(boardx, boardy);
                            Play_ClearLogicalBoard();
                            Play_SimulateBoard(boardx, boardy, Move_List);
                            Play_GraphicsReady(boardx, boardy);
                        }; break;
                        case 0x55 : { //VK_U
                            Play_Undo (boardx, boardy);
                            if (Screen_Mode > 0)
                                Play_Undo(boardx, boardy);
                        }; break;
                        case VK_UP : {
                            if (Board_Current_Cursor_Y-1>=0) {
                                Play_MoveBoardCursor(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y-1,1,1);
                            }
                            else {
                                Play_MoveBoardCursor(boardx, boardy, Board_Current_Cursor_X, Board_Logical_Size-1,1,1);
                            }
                        }; break;
                        case VK_DOWN : {
                            if (Board_Current_Cursor_Y+1<Board_Logical_Size) {
                                Play_MoveBoardCursor(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y+1,1,1);
                            }
                            else {
                                Play_MoveBoardCursor(boardx, boardy, Board_Current_Cursor_X, 0,1,1);
                            }
                        }; break;
                        case VK_LEFT : {
                            if (Board_Current_Cursor_X-1>=0) {
                                Play_MoveBoardCursor(boardx, boardy, Board_Current_Cursor_X-1, Board_Current_Cursor_Y,1,1);
                            }
                            else {
                                Play_MoveBoardCursor(boardx, boardy, Board_Logical_Size-1, Board_Current_Cursor_Y,1,1);
                            }
                        }; break;
                        case VK_RIGHT : {
                            if (Board_Current_Cursor_X+1<Board_Logical_Size) {
                                Play_MoveBoardCursor(boardx, boardy, Board_Current_Cursor_X+1, Board_Current_Cursor_Y,1,1);
                            }
                            else {
                                Play_MoveBoardCursor(boardx, boardy, 0, Board_Current_Cursor_Y,1,1);
                            }
                        }; break;
                        case VK_SPACE : {
                            AMove Move = {1, Current_Turn, Board_Current_Cursor_X, Board_Current_Cursor_Y};
                            if (isLegalMove(Move)) {
                                ///Đã có người thắng hay chưa?
                                ///Thắng, animation, còn thua thì vẫn luân phiên.
                                Board_Num_Cell_Placed ++;
                                if (isWinningMove(Move)) {
                                    Play_WinningAnimation (Current_Turn);

                                    //Statistic
                                    if (Screen_Mode > 0) {
                                        Number_of_PvC_Wins ++;
                                    }
                                    else if (Current_Turn==1) {
                                        Number_of_PvP_P1_Wins ++;
                                    }
                                    else {
                                        Number_of_PvP_P2_Wins ++;
                                    }
                                    Main_Menu_Signal = 1;
                                    break;
                                }
                                else {
                                    if (Board_Num_Cell_Placed == Board_Logical_Size*Board_Logical_Size) {
                                        Play_DrawAnimation ();
                                        Number_of_PvP_Draws ++;
                                        Main_Menu_Signal = 1;
                                        break;
                                    }
                                }
                                Move_List. push_back(Move);
                                board(Board_Current_Cursor_X, Board_Current_Cursor_Y) = (Current_Turn==1)?'1':'2';

                                CoordList Expanded;
                                Expand(Board_Current_Cursor_X, Board_Current_Cursor_Y, Expanded, 1);
                                for (Coord cEx : Expanded) {
                                    Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
                                    Child_List.push_back({cEx.x, cEx.y});
                                }

                                Play_UpdateCell(boardx, boardy, Board_Current_Cursor_X, Board_Current_Cursor_Y, 1);
                                Play_UpdateTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                                                        boardy+1,Current_Turn,3-Current_Turn);
                                Current_Turn = 3-Current_Turn;

                                //Draw last moves
                                if (Current_Game_Mode&PLACE_STONE_MODE) {
                                    Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                                    MAX_NAME_LENGTH+24,
                                    boardy+1);
                                }
                                else {
                                    Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                                    MAX_NAME_LENGTH+16,
                                    boardy+1);
                                }
                            } //isLegal
                            else {
                                Play_DrawIllegalMoveIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                              boardy+Board_Logical_Size*Board_Cell_Graphical_Size*5/6);
                            }
                        }; break;
                        case VK_TAB : {
                            if (Current_Game_Mode & PLACE_STONE_MODE) {
                                AMove Move = {0, Current_Turn, Board_Current_Cursor_X, Board_Current_Cursor_Y};
                                if (isLegalMove(Move) &&
                                    ((Current_Turn==1 && Player_1_Stone>0) || (Current_Turn==2 && Player_2_Stone>0))) {
                                    Move_List.push_back(Move);
                                    board(Board_Current_Cursor_X, Board_Current_Cursor_Y) = 'S';
                                    Play_UpdateCell(boardx, boardy,
                                                    Board_Current_Cursor_X, Board_Current_Cursor_Y, 1);
                                    if (Current_Turn==1) {
                                        Player_1_Stone--;
                                    }
                                    else {
                                        Player_2_Stone--;
                                    }

                                    CoordList Expanded;
                                    Expand(Board_Current_Cursor_X, Board_Current_Cursor_Y, Expanded, 1);
                                    for (Coord cEx : Expanded) {
                                        Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
                                        Child_List.push_back({cEx.x, cEx.y});
                                    }

                                    Play_DrawTurnIndicator(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3,
                                    boardy+1, Current_Game_Mode & PLACE_STONE_MODE);

                                    //Draw last moves
                                    if (Current_Game_Mode&PLACE_STONE_MODE) {
                                        Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                                        MAX_NAME_LENGTH+24,
                                        boardy+1);
                                    }
                                    else {
                                        Play_DrawLastMove(boardx+2+Board_Logical_Size*Board_Cell_Graphical_Size+3+DEFAULT_BOARD_CELL_GRAPHICAL_SIZE+
                                        MAX_NAME_LENGTH+16,
                                        boardy+1);
                                    }
                                } //if2
                            } //if1
                        }
                    }//switch
                } //case KEY_EVENT
            } //switch
        }
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }
}
void Stat_Load () {
    FILE *fStat = fopen(Stat_File_Path,"r");
    if (fStat==NULL) {
        Number_of_PvC_Draws=Number_of_PvC_Loses=Number_of_PvC_Wins=Number_of_PvP_Draws=Number_of_PvP_P1_Wins=Number_of_PvP_P2_Wins=0;
    }
    else {
    fscanf(fStat,"%d %d %d %d %d %d",&Number_of_PvP_P1_Wins,&Number_of_PvP_P2_Wins,&Number_of_PvP_Draws,
          &Number_of_PvC_Wins, &Number_of_PvC_Loses, &Number_of_PvC_Draws);
    }
    fclose(fStat);
}
void Stat_Save () {
    ofstream fout; fout.open(Stat_File_Path);
    fout<<Number_of_PvP_P1_Wins<<" "<<Number_of_PvP_P2_Wins<<" "<<Number_of_PvP_Draws<<" "<<
          Number_of_PvC_Wins<<" "<<Number_of_PvC_Loses<<" "<<Number_of_PvC_Draws;
    fout.close();
}
bool Data_ReadFromFile (const char fileName[], GameData& data) {
    ifstream fin;
    fin.open(fileName);

    if (fin.is_open()) {
        int num_moves; AMove tmp;
        data.Move_List.clear();
        fin>>data.Board_Logical_Size>>data.Screen_Mode>>data.Current_Game_Mode>>data.Player_1_Stone>>data.Player_2_Stone>>num_moves
        >>data.Enable_Undo>>data.Current_Turn;
        for (int i=0; i<num_moves; ++i) {
            fin>>tmp.TurnLost>>tmp.Maker>>tmp.x>>tmp.y;
            data.Move_List.push_back(tmp);
        }
    }
    else return 0;
    fin.close();
    return 1;
}
void Data_WriteToFile (const char fileName[], const GameData& data) {
    ofstream fout;
    fout.open(fileName);
    if (fout.is_open()) {
        fout<<data.Board_Logical_Size<<"\n"<<data.Screen_Mode<<"\n"<<data.Current_Game_Mode<<"\n"<<data.Player_1_Stone
        <<"\n"<<data.Player_2_Stone<<"\n"<<(int)data.Move_List.size()<<"\n"<<data.Enable_Undo<<"\n"<<data.Current_Turn<<"\n";
        for (AMove m : data.Move_List) {
            fout<<m.TurnLost<<" "<<m.Maker<<" "<<m.x<<" "<<m.y<<endl;
        }
    }
    fout.close();
}
void Data_InitializeSlots () {
    for (int i=0; i<Num_Data_Slot; ++i) {
        Slot_Non_Empty[i] = Data_ReadFromFile(Slot_Data_File[i], Slot_Data[i]);
    }
}
void Data_FinalizeSlots () {
    for (int i=0; i<Num_Data_Slot; ++i) {
        if (Slot_Non_Empty[i])
            Data_WriteToFile(Slot_Data_File[i], Slot_Data[i]);
    }
}
void Data_DrawSlotInfo (int x, int y, int slotId, bool NonEmpty, const GameData &data, bool selected) {
    if (selected) SetColor(Color::Black, Color::LightYellow);
    else SetColor(Color::Black, Color::White);
    PutFilledRectangle(x,y,x+Slot_Width-1,y+Slot_Height-1,' ');
    PutBorderedRectangle(x,y,x+Slot_Width-1,y+Slot_Height-1,218,191,192,217,196,179,179,196);
    GotoXY(x+1,y+1);
    cout<<"DATA "<<slotId<<"  ";

    if (NonEmpty) {
        switch (data.Screen_Mode) {
         case 0: cout<<"PvP"; break;;
         case 1: cout<<"PvC Easy";break;
         case 2: cout<<"PvC Norm";break;
         case 3: cout<<"PvC Hard";break;
         default: cout<<"WTF!!!";break;
        }

        GotoXY(x+1,y+2);
        cout<<Board_Logical_Size<<" X "<<Board_Logical_Size<<" board";

        GotoXY(x+1,y+3);
        if (data.Current_Game_Mode & CARO_MODE) {
            cout<<"Caro";
        }
        else cout<<"Gomoku";
        if (data.Current_Game_Mode & PLACE_STONE_MODE) cout<<"+Stone";
        if (data.Enable_Undo) cout<<"+Undo";

        GotoXY(x+1,y+4);
        cout<<data.Move_List.size()<<" moves.";
    }
    else {
        GotoXY(x+1,y+2);
        cout<<"No Data.";
    }
    cout.flush();
}
void Option_InitializeOption (const char OptionFile[]) {
    ifstream fin;
    fin.open(OptionFile);
    if (fin.is_open()) {
        fin>>Board_Logical_Size;
        Board_Cell_Graphical_Size = DEFAULT_BOARD_CELL_GRAPHICAL_SIZE;
        fin>>Current_Game_Mode>>P1_Move_First>>Enable_Undo;
        fin.ignore(9999,'\n');
        fin.get(Player_1_Name, MAX_NAME_LENGTH, '\n');
        fin.ignore(9999,'\n');
        fin.get(Player_2_Name, MAX_NAME_LENGTH, '\n');
    }
    else { //Load default setting.
        Board_Logical_Size = DEFAULT_BOARD_LOGICAL_SIZE;
        Board_Cell_Graphical_Size = DEFAULT_BOARD_CELL_GRAPHICAL_SIZE;
        Current_Game_Mode = 0; ///Gomoku - stone + undo
        P1_Move_First = 1;
        Enable_Undo = 1;

        strcpy(Player_1_Name, "Gryyna");
        strcpy(Player_2_Name, "Yellos");
    }
    fin.close();
}
void Option_FinalizeOption (const char OptionFile[]) {
    ofstream fout;
    fout.open(OptionFile);
    if (fout.is_open()) {
        fout<<Board_Logical_Size<<"\n";
        fout<<Current_Game_Mode<<"\n"<<P1_Move_First<<"\n"<<Enable_Undo<<"\n";
        fout<<Player_1_Name<<endl;
        fout<<Player_2_Name<<endl;
        fout<<"--End of option file--"<<endl;
    }
    fout.close();
}
void Option_GetCurrentChoice (int choice[]) {
    //choice[0]: Board size
    switch (Board_Logical_Size) {
        case 10: choice[0]=0; break;
        case 15: choice[0]=1; break;
        case 20: choice[0]=2; break;
    }
    //choice[1]: Gomoku or Caro?
    choice[1] = ((Current_Game_Mode&CARO_MODE) ? 4 : 3);
    //choice[2]: Player 1 goes first?
    choice[2] = (P1_Move_First?5:6);
    //choice[3]: Enable undo?
    choice[3] = (Enable_Undo?5:6);
    //choice[4]: Enable placing stone?
    choice[4] = ((Current_Game_Mode&PLACE_STONE_MODE)?5:6);
    //not relevant
    choice[5]=choice[6]=choice[7]=choice[8]=choice[9]=-1;
}
void Option_DrawOptions (int x, int y,int TitleId,const char*OptionTitleDescription[],const char*OptionDescriptionList[],
                         const int CurrentChoice, bool selecting) {
    GotoXY(x,y);
    if (selecting)
        SetColor(Color::Black, Color::LightYellow);
    else SetColor(Color::Black, Color::White);
    cout<<OptionTitleDescription[TitleId]<<"   ";

    if (CurrentChoice!=-1) {
        if (selecting)
            SetColor(Color::Green, Color::LightYellow);
        else SetColor(Color::Black, Color::White);
        cout<<OptionDescriptionList[CurrentChoice];
    }
}
void Option_DrawInstruction (int x, int y) {
    int curX=x, curY=y;SetColor(Color::Black, Color::White);
    PutBorderedRectangle(curX, curY, curX+42, curY+7,'/','\\','\\','/','-','|','|','-');
    curX+=2; curY++;
    curY++;GotoXY(curX,curY);
    cout<<"Up/Down/Left/Right:  change the options";
    curY++; GotoXY(curX,curY);
    cout<<"S:          (Quick) Save changes & Exit";
    curY++; GotoXY(curX,curY);
    cout<<"ESC:     (Quick) Discard changes & Exit";
    curY++; GotoXY(curX,curY);
    cout<<"Enter: (Does that) choose an option (?)";
//    curY++; GotoXY(curX, curY);
//    cout<<"???:                 Hmmm... Who knows?";
}
void Option_SetCurrentChoice (const int choice[]) {
    //choice[0]: Board size
    switch (choice[0]) {
        case 0: Board_Logical_Size=10; break;
        case 1: Board_Logical_Size=15; break;
        case 2: Board_Logical_Size=20; break;
    }
    //choice[1]: Gomoku or Caro?
    Current_Game_Mode = (choice[1]==3) ? 0 : CARO_MODE;
    //choice[2]: Player 1 goes first?
    P1_Move_First = (choice[2]==5);
    //choice[3]: Enable undo?
    Enable_Undo = (choice[3] == 5);
    //choice[4]: Enable placing stone?
    if (choice[4]==5) Current_Game_Mode |= PLACE_STONE_MODE;
}

void Option_ReloadGraphics (const int Num_Options, const int OptionTitle_X_coord[], const int OptionTitle_Y_coord[],
                            const char *OptionTitleDescription[], const char *OptionDescriptionList[],
                            const int OptionCurrentChoice[], const int Current_Title_Id) {
    const int Screen_Width = 85;
    const int Screen_Height = 20;
    AdjustScreenSize(Screen_Width, Screen_Height);
    SetColor(Color::Black, Color::White);

    //hide the cursor
    CONSOLE_CURSOR_INFO csnfo = {100,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csnfo);

    ClearScreen();

    GotoXY(0,0);
    cout<<"------OPTION SCREEN-------"<<endl;
    cout<<"Warning: Options set here only affect new game, but not saved game."<<endl;
    cout<<"It is encouraged to set the options again after playing a saved match.";

    //Draw overall
    for (int i=0; i<Num_Options; ++i) {
        Option_DrawOptions(OptionTitle_X_coord[i], OptionTitle_Y_coord[i],
                           i, OptionTitleDescription, OptionDescriptionList,
                           OptionCurrentChoice[i],i==Current_Title_Id);
    }
    Option_DrawInstruction(40, 5);

}
void OptionScreen () {
    const int Num_Options = 10;
    const char * OptionTitleDescription[Num_Options] = {"Board Size           ",
                                                        "Rule set             ",
                                                        "Player 1 goes first  ",
                                                        "Enable Undo          ",
                                                        "Enable placing stones",
                                                        "Save changes & Exit   ",
                                                        "Discard changes & Exit",
                                                        "View player 1 win animation",
                                                        "View player 2 win animation",
                                                        "View draw animation        "};
    const char * OptionDescriptionList[7] = {"  10 x 10  ",
                                             "  15 x 15  ",
                                             "  20 x 20  ",
                                             "  Gomoku   ",
                                             "   Caro    ",
                                             "   TRUE    ",
                                             "   FALSE   "};
    const int OptionDescriptionRange_L[Num_Options] = {0,3,5,5,5,-1,-1,-1,-1,-1};
    const int OptionDescriptionRange_R[Num_Options] = {2,4,6,6,6,-1,-1,-1,-1,-1};

    int OptionCurrentChoice[Num_Options];
    Option_GetCurrentChoice(OptionCurrentChoice);

    const int OptionTitle_Y_coord[Num_Options]={5,6,7,8,9,10,11,14,15,16};
    const int OptionTitle_X_coord[Num_Options]={0,0,0,0,0, 0, 0, 0, 0, 0};
    int Current_Title_Id = 0;
    Option_ReloadGraphics (Num_Options, OptionTitle_X_coord, OptionTitle_Y_coord,
                            OptionTitleDescription, OptionDescriptionList,
                            OptionCurrentChoice,Current_Title_Id);

    INPUT_RECORD InputRecord; bool Exit_Option_Screen_Signal = 0;
    DWORD numEventsRead;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    while (!Exit_Option_Screen_Signal) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &numEventsRead);
        if (InputRecord.EventType==KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown==true) {
            switch (InputRecord.Event.KeyEvent.wVirtualKeyCode) {
                case VK_LEFT: {
                    if (OptionCurrentChoice[Current_Title_Id]!=-1) {
                        if (OptionCurrentChoice[Current_Title_Id]-1>=OptionDescriptionRange_L[Current_Title_Id]) {
                            OptionCurrentChoice[Current_Title_Id]--;
                        }
                        else {
                            OptionCurrentChoice[Current_Title_Id]=OptionDescriptionRange_R[Current_Title_Id];
                        }
                        Option_DrawOptions(OptionTitle_X_coord[Current_Title_Id], OptionTitle_Y_coord[Current_Title_Id],
                                           Current_Title_Id,OptionTitleDescription,OptionDescriptionList,
                                           OptionCurrentChoice[Current_Title_Id], 1);
                    }
                }; break;
                case VK_RIGHT:{
                    if (OptionCurrentChoice[Current_Title_Id]!=-1) {
                        if (OptionCurrentChoice[Current_Title_Id]+1<=OptionDescriptionRange_R[Current_Title_Id]) {
                            OptionCurrentChoice[Current_Title_Id]++;
                        }
                        else {
                            OptionCurrentChoice[Current_Title_Id]=OptionDescriptionRange_L[Current_Title_Id];
                        }
                        Option_DrawOptions(OptionTitle_X_coord[Current_Title_Id], OptionTitle_Y_coord[Current_Title_Id],
                                           Current_Title_Id,OptionTitleDescription,OptionDescriptionList,
                                           OptionCurrentChoice[Current_Title_Id], 1);
                    }
                };break;
                case VK_UP: {
                    Option_DrawOptions(OptionTitle_X_coord[Current_Title_Id], OptionTitle_Y_coord[Current_Title_Id],
                                           Current_Title_Id,OptionTitleDescription,OptionDescriptionList,
                                           OptionCurrentChoice[Current_Title_Id], 0);
                    Current_Title_Id = (Current_Title_Id-1>=0) ? Current_Title_Id-1 : Num_Options-1;
                    Option_DrawOptions(OptionTitle_X_coord[Current_Title_Id], OptionTitle_Y_coord[Current_Title_Id],
                                           Current_Title_Id,OptionTitleDescription,OptionDescriptionList,
                                           OptionCurrentChoice[Current_Title_Id], 1);
                };break;
                case VK_DOWN: {
                    Option_DrawOptions(OptionTitle_X_coord[Current_Title_Id], OptionTitle_Y_coord[Current_Title_Id],
                                           Current_Title_Id,OptionTitleDescription,OptionDescriptionList,
                                           OptionCurrentChoice[Current_Title_Id], 0);
                    Current_Title_Id = (Current_Title_Id+1<Num_Options) ? Current_Title_Id+1 : 0;
                    Option_DrawOptions(OptionTitle_X_coord[Current_Title_Id], OptionTitle_Y_coord[Current_Title_Id],
                                           Current_Title_Id,OptionTitleDescription,OptionDescriptionList,
                                           OptionCurrentChoice[Current_Title_Id], 1);

                };break;
                case 0x53: {//VK_S
                    Option_SetCurrentChoice(OptionCurrentChoice);
                    Exit_Option_Screen_Signal = 1;
                };break;
                case VK_ESCAPE: {
                    Exit_Option_Screen_Signal = 1;
                };break;
                case VK_RETURN: {
                    switch (Current_Title_Id) {
                        case 5: { //"Save changes & Exit   ",5
                            Option_SetCurrentChoice(OptionCurrentChoice);
                            Exit_Option_Screen_Signal = 1;
                        }; break;
                        case 6: { //"Discard changes & Exit",6
                            Exit_Option_Screen_Signal = 1;
                        }; break;
                        case 7: {//"View player 1 win animation",7
                            Play_WinningAnimation(1);

                            Option_ReloadGraphics (Num_Options, OptionTitle_X_coord, OptionTitle_Y_coord,
                                OptionTitleDescription, OptionDescriptionList,
                                OptionCurrentChoice,Current_Title_Id);
                        }; break;
                        case 8: {//"View player 2 win animation",8
                            Play_WinningAnimation(2);

                            Option_ReloadGraphics (Num_Options, OptionTitle_X_coord, OptionTitle_Y_coord,
                                    OptionTitleDescription, OptionDescriptionList,
                                    OptionCurrentChoice,Current_Title_Id);
                        }; break;
                        case 9: {//"View draw animation        "};9
                            Play_DrawAnimation();

                            Option_ReloadGraphics (Num_Options, OptionTitle_X_coord, OptionTitle_Y_coord,
                                    OptionTitleDescription, OptionDescriptionList,
                                    OptionCurrentChoice,Current_Title_Id);
                        }; break;
                    }
                };break;
            } //switch
        }//if
    }
}
bool DataScreen (bool EnableSave) {
//    { ///Test DrawSlotInfo
//        Data_DrawSlotInfo(1,1,1,0,Slot_Data[1],1);
//        Slot_Data[2].Board_Logical_Size=15;
//        Slot_Data[2].Current_Game_Mode |= PLACE_STONE_MODE;
//        Slot_Data[2].Screen_Mode = 3;
//        Data_DrawSlotInfo(1,10,2,1,Slot_Data[2],0);
//    }

//    { ///Test Play_SimulateBoard
//        Move_List.push_back({1,1,7,7});
//        Move_List.push_back({1,2,6,7});
//        Play_SimulateBoard(30,1,Move_List,Board);
//    }
    AdjustScreenSize(3+Slot_Width+4+Board_Logical_Size*Board_Cell_Graphical_Size+12,
                     max(2+3*Num_Data_Slot+3*Slot_Height, 8+Board_Logical_Size*Board_Cell_Graphical_Size));
    SetColor(Color::Black, Color::White);
    ClearScreen();
    cout<<"--------DATA SCREEN--------"<<endl;
    cout<<"WARNING: NO OVERWRITE CONFIRMATION!"<<endl;

    GotoXY(40,0);
    cout<<"Up/Down: Select data";
    GotoXY(40,1);
    cout<<"L: Load current data";
    if (EnableSave) {
        GotoXY(40,2);
        cout<<"S: Save to current data";
        GotoXY(40,3);
        cout<<"ESC: Back to previous screen";
    }
    else {
        GotoXY(40,2);
        cout<<"ESC: Back to previous screen";
    }

    int Current_Selecting_Slot = 0;
    int slotX[Num_Data_Slot] = {3,3,3,3};
    int slotY[Num_Data_Slot] = {4,12,20,28};

    ///Temporary board for previewing.
    char TmpBoard[MAX_BOARD_LOGICAL_SIZE*MAX_BOARD_LOGICAL_SIZE];

    for (int i=0; i<Num_Data_Slot; ++i) {
        Data_DrawSlotInfo(slotX[i], slotY[i], i+1, Slot_Non_Empty[i], Slot_Data[i], i==Current_Selecting_Slot);
    }
    Play_SimulateBoard(3+Slot_Width+7,4,Slot_Data[Current_Selecting_Slot].Move_List,TmpBoard);

    CONSOLE_CURSOR_INFO csnfo = {100, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csnfo);

    INPUT_RECORD InputRecord; DWORD numEventsRead;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    while (1) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &numEventsRead);
        switch (InputRecord.EventType) {
            case (KEY_EVENT): {
                if (InputRecord.Event.KeyEvent.bKeyDown == TRUE) {
                    switch (InputRecord.Event.KeyEvent.wVirtualKeyCode) {
                        case VK_ESCAPE: {
                            return 0;
                        }; break;
                        case 0x4C: { //VK_L
                            if (Slot_Non_Empty[Current_Selecting_Slot]) {
                                Board_Logical_Size = Slot_Data[Current_Selecting_Slot].Board_Logical_Size;
                                Current_Game_Mode = Slot_Data[Current_Selecting_Slot].Current_Game_Mode;
                                Player_1_Stone = Slot_Data[Current_Selecting_Slot].Player_1_Stone;
                                Player_2_Stone = Slot_Data[Current_Selecting_Slot].Player_2_Stone;
                                Screen_Mode = Slot_Data[Current_Selecting_Slot].Screen_Mode;
                                Move_List = Slot_Data[Current_Selecting_Slot].Move_List;
                                Enable_Undo = Slot_Data[Current_Selecting_Slot].Enable_Undo;
                                Current_Turn = Slot_Data[Current_Selecting_Slot].Current_Turn;

                                ///Load complete!
                                GotoXY(slotX[Current_Selecting_Slot]+1,slotY[Current_Selecting_Slot]+2);
                                SetColor(Color::Black, Color::LightGreen);
                                cout<<"LOAD COMPLETE!";
                                return 1;
                            }
                            else {
                                ///Load failed!
                                GotoXY(slotX[Current_Selecting_Slot]+1,slotY[Current_Selecting_Slot]+2);
                                SetColor(Color::Black, Color::LightRed);
                                cout<<"NO DATA TO LOAD!";
                                Sleep(750);

                                Data_DrawSlotInfo(slotX[Current_Selecting_Slot],
                                              slotY[Current_Selecting_Slot],
                                              Current_Selecting_Slot+1,
                                              Slot_Non_Empty[Current_Selecting_Slot],
                                              Slot_Data[Current_Selecting_Slot],
                                              1);
                            }
                        }; break;
                        case 0x53: { //VK_S
                            if (EnableSave) {
                                Slot_Non_Empty[Current_Selecting_Slot] = 1;
                                Slot_Data[Current_Selecting_Slot].Board_Logical_Size = Board_Logical_Size;
                                Slot_Data[Current_Selecting_Slot].Current_Game_Mode = Current_Game_Mode;
                                Slot_Data[Current_Selecting_Slot].Player_1_Stone = Player_1_Stone;
                                Slot_Data[Current_Selecting_Slot].Player_2_Stone = Player_2_Stone;
                                Slot_Data[Current_Selecting_Slot].Screen_Mode = Screen_Mode;
                                Slot_Data[Current_Selecting_Slot].Move_List = Move_List;
                                Slot_Data[Current_Selecting_Slot].Enable_Undo = Enable_Undo;
                                Slot_Data[Current_Selecting_Slot].Current_Turn = Current_Turn;

                                ///Reload current slot
                                Data_DrawSlotInfo(slotX[Current_Selecting_Slot],
                                              slotY[Current_Selecting_Slot],
                                              Current_Selecting_Slot+1,
                                              Slot_Non_Empty[Current_Selecting_Slot],
                                              Slot_Data[Current_Selecting_Slot],
                                              1);
                                Play_SimulateBoard(3+Slot_Width+7,4,
                                               Slot_Data[Current_Selecting_Slot].Move_List,
                                               TmpBoard);
                            }
                        }; break;
                        case VK_UP: {
                            Data_DrawSlotInfo(slotX[Current_Selecting_Slot],
                                              slotY[Current_Selecting_Slot],
                                              Current_Selecting_Slot+1,
                                              Slot_Non_Empty[Current_Selecting_Slot],
                                              Slot_Data[Current_Selecting_Slot],
                                              0);

                            Current_Selecting_Slot = (Current_Selecting_Slot>0) ? Current_Selecting_Slot-1 : Num_Data_Slot-1;

                            Data_DrawSlotInfo(slotX[Current_Selecting_Slot],
                                              slotY[Current_Selecting_Slot],
                                              Current_Selecting_Slot+1,
                                              Slot_Non_Empty[Current_Selecting_Slot],
                                              Slot_Data[Current_Selecting_Slot],
                                              1);
                            Play_SimulateBoard(3+Slot_Width+7,4,
                                               Slot_Data[Current_Selecting_Slot].Move_List,
                                               TmpBoard);
                        }; break;
                        case VK_DOWN: {
                            Data_DrawSlotInfo(slotX[Current_Selecting_Slot],
                                              slotY[Current_Selecting_Slot],
                                              Current_Selecting_Slot+1,
                                              Slot_Non_Empty[Current_Selecting_Slot],
                                              Slot_Data[Current_Selecting_Slot],
                                              0);

                            Current_Selecting_Slot = (Current_Selecting_Slot+1<Num_Data_Slot) ? Current_Selecting_Slot+1 : 0;

                            Data_DrawSlotInfo(slotX[Current_Selecting_Slot],
                                              slotY[Current_Selecting_Slot],
                                              Current_Selecting_Slot+1,
                                              Slot_Non_Empty[Current_Selecting_Slot],
                                              Slot_Data[Current_Selecting_Slot],
                                              1);
                            Play_SimulateBoard(3+Slot_Width+7,4,
                                               Slot_Data[Current_Selecting_Slot].Move_List,
                                               TmpBoard);
                        }; break;
                    }
                } //if
            }; break; //case
        } //switch
    } //while
}

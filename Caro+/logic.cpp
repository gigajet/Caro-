#include "logic.h"
#include "globalVars.h"

///Nước đi vào tọa độ (x,y) có hợp lệ hay không?
bool isLegalMove (AMove Move) {
    int x=Move.x, y=Move.y;
    return (x>=0 && x<Board_Logical_Size && y>=0 && y<Board_Logical_Size
            && board(x,y)=='.');
}

bool isWinningGomokuMove (int x, int y, char sig) {
    board(x,y)=sig;
    { ///Horizontal checking
    int x0=x, x1=x;
    while (x0-1 >= 0 && board(x0-1,y)==sig) x0--;
    while (x1 < Board_Logical_Size && board(x1,y)==sig) x1++;
    if (x1-x0 == 5) {board(x,y) = '.';return 1;}
    }

    { ///Vertical checking
        int y0=y, y1=y;
        while (y0-1>=0 && board(x,y0-1)==sig) y0--;
        while (y1 < Board_Logical_Size && board(x,y1)==sig) y1++;
        if (y1-y0==5) {board(x,y) = '.';return 1;}
    }

    { ///Main diagonal checking (\)
        int x0=x, y0=y, x1=x, y1=y;
        while (x0-1>=0 && y0-1>=0 && board(x0-1,y0-1)==sig) --x0, --y0;
        while (x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)==sig) ++x1, ++y1;
        if (x1-x0 == 5) {board(x,y) = '.';return 1;}
    }

    { ///Sub diagonal checking (/)
        int x0=x, y0=y, x1=x, y1=y;
        while (x0-1>=0 && y0+1<Board_Logical_Size && board(x0-1,y0+1)==sig) --x0, ++y0;
        while (x1<Board_Logical_Size && y1>=0 && board(x1,y1)==sig) ++x1, --y1;
        if (x1-x0 == 5) {board(x,y) = '.';return 1;}
    }
    board(x,y) = '.';
    return 0;
}
bool isWinningCaroMove (int x, int y, char sig) {
    board(x,y)=sig;
    { ///Horizontal checking
    int x0=x, x1=x;
    while (x0-1 >= 0 && board(x0-1,y)==sig) x0--;
    while (x1 < Board_Logical_Size && board(x1,y)==sig) x1++;
    if (x1-x0 == 5) {
        int numHeadBlocked = 0;
        char anoSig = (sig=='1')?'2':'1';
        if (x0-1>=0 && board(x0-1,y)==anoSig) numHeadBlocked++;
        if (x1<Board_Logical_Size && board(x1,y)==anoSig) numHeadBlocked++;
        if (numHeadBlocked < 2) {
            board(x,y) = '.';
            return 1;}
        }
    }

    { ///Vertical checking
        int y0=y, y1=y;
        while (y0-1>=0 && board(x,y0-1)==sig) y0--;
        while (y1 < Board_Logical_Size && board(x,y1)==sig) y1++;
        if (y1-y0==5) {
        int numHeadBlocked = 0;
        char anoSig = (sig=='1')?'2':'1';
        if (y0-1>=0 && board(x,y0-1)==anoSig) numHeadBlocked++;
        if (y1<Board_Logical_Size && board(x,y1)==anoSig) numHeadBlocked++;
        if (numHeadBlocked < 2) {
            board(x,y) = '.';
            return 1;}
        }
    }

    { ///Main diagonal checking (\)
        int x0=x, y0=y, x1=x, y1=y;
        while (x0-1>=0 && y0-1>=0 && board(x0-1,y0-1)==sig) --x0, --y0;
        while (x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)==sig) ++x1, ++y1;
        if (x1-x0 == 5) {
        int numHeadBlocked = 0;
        char anoSig = (sig=='1')?'2':'1';
        if (x0-1>=0 && y0-1>=0 && board(x0-1,y0-1)==anoSig) numHeadBlocked++;
        if (x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)==anoSig) numHeadBlocked++;
        if (numHeadBlocked < 2) {
            board(x,y) = '.';
            return 1;}
        }
    }

    { ///Sub diagonal checking (/)
        int x0=x, y0=y, x1=x, y1=y;
        while (x0-1>=0 && y0+1<Board_Logical_Size && board(x0-1,y0+1)==sig) --x0, ++y0;
        while (x1<Board_Logical_Size && y1>=0 && board(x1,y1)==sig) ++x1, --y1;
        if (x1-x0 == 5) {
        int numHeadBlocked = 0;
        char anoSig = (sig=='1')?'2':'1';
        if (x0-1>=0 && y0+1<Board_Logical_Size && board(x0-1,y0+1)==anoSig) numHeadBlocked++;
        if (x1<Board_Logical_Size && y1>=0 && board(x1,y1)==anoSig) numHeadBlocked++;
        if (numHeadBlocked < 2) {
            board(x,y) = '.';
            return 1;}
        }
    }
    board(x,y) = '.';
    return 0;
}
bool isWinningIsoleteMove (int x, int y, char sig) {
    return 0;
}

///Nước đi vào tọa độ (x,y) có phải nước đi thắng hay không?
bool isWinningMove (AMove Move) {
    if (!Move.TurnLost) return 0;
    if (!isLegalMove(Move)) return 0;
    char MoveSignal = (Move.Maker==1)?'1':'2';
    if (Current_Game_Mode & CARO_MODE)
        return isWinningCaroMove(Move.x, Move.y, MoveSignal);
    else if (Current_Game_Mode & ISOLETE_MODE)
        return isWinningIsoleteMove(Move.x, Move.y, MoveSignal);
    else return isWinningGomokuMove(Move.x, Move.y, MoveSignal);
}

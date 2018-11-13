#include "globalVars.h"
#include "logic.h"
#include "dataStruct.h"

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

bool isWinningMove (AMove Move) {
    if (!Move.TurnLost) return 0;
    if (!isLegalMove(Move)) return 0;
    char MoveSignal = (Move.Maker==1)?'1':'2';
    if (Current_Game_Mode & CARO_MODE)
        return isWinningCaroMove(Move.x, Move.y, MoveSignal);
    else return isWinningGomokuMove(Move.x, Move.y, MoveSignal);
}

//parameter meaning: is this the turn of maximizer of not?
long long Evaluation_Gomoku (bool maximizer) {
    //number of patterns of type [second dimension] of player [first dimension]
    //first dimension: 1: maximizer (gryyna), 2: minimizer (yellos)
    //second dimension:
    //7=xxxxx, 6=-xxxx-, 5=xxxx-, 4=-xxx-, 3=xxx-, 2=-xx-, 1=-x-, 0=xx-
    long long score = 0ll;
    int pattern[3][8];
    for (int i=0; i<3; ++i) for (int j=0; j<8; ++j) pattern[i][j] = 0;
    for (int x=0; x<Board_Logical_Size; ++x)
    for (int y=0; y<Board_Logical_Size; ++y)
    if (board(x,y)!='.' && board(x,y)!='S') {
        char sig = board(x,y);
        int firstDim = (board(x,y)=='1') ? 1 : 2;
        { ///Horizontal
            int x0=x;
            while (x0-1>=0 && board(x0-1, y)==sig) x0--;
            int x1=x;
            while (x1<Board_Logical_Size && board(x1,y)==sig) x1++;

            int patternsize = x1-x0;
            switch (patternsize) {
            case 5: {
                pattern[firstDim][7] ++;
            }; break;
            case 4:{
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (x0-1>=0 && board(x0-1,y)=='.') pattern[firstDim][5] ++;
                if (x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][5]++;

            }; break;
            case 3:{
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][4]++;

                //Does it open on any side?
                if (x0-1>=0 && board(x0-1,y)=='.') pattern[firstDim][3] ++;
                if (x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][3]++;
            }; break;
            case 2:{
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][2]++;

                //Does it open on any side?
                if (x0-1>=0 && board(x0-1,y)=='.') pattern[firstDim][0] ++;
                if (x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][0]++;

            }; break;
            case 1: {
                pattern[firstDim][1]++;
            }
            } //switch
        } //block

        { ///Vertical
            int y0=y;
            while (y0-1>=0 && board(x, y0-1)==sig) y0--;
            int y1=y;
            while (y1<Board_Logical_Size && board(x,y1)==sig) y1++;

            int patternsize = y1-y0;
            switch (patternsize) {
            case 5: {
                pattern[firstDim][5] ++;
            }; break;
            case 4:{
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (y0-1>=0 && board(x,y0-1)=='.') pattern[firstDim][4] ++;
                if (y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][4]++;
            }; break;
            case 3:{
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][3]++;

                //Does it open on any side?
                if (y0-1>=0 && board(x,y0-1)=='.') pattern[firstDim][2] ++;
                if (y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][2]++;
            }; break;
            case 2:{
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][1]++;

                //Does it open on any side?
                if (y0-1>=0 && board(x,y0-1)=='.') pattern[firstDim][0] ++;
                if (y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block

        { ///Main diagonal
            int x0=x,y0=y;
            while (y0-1>=0 && x0-1>=0 && board(x0-1, y0-1)==sig) {x0--; y0--;}
            int x1=x,y1=y;
            while (x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)==sig) {x1++; y1++;}

            int patternsize = y1-y0;
            switch (patternsize) {
            case 5: {
                pattern[firstDim][5] ++;
            }; break;
            case 4:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (y0-1>=0 && x0-1>=0 && board(x0,y0-1)=='.') pattern[firstDim][4] ++;
                if (y1<Board_Logical_Size && x1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][4]++;
            }; break;
            case 3:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][3]++;

                //Does it open on any side?
                if (y0-1>=0 && x0-1>=0 && board(x0,y0-1)=='.') pattern[firstDim][4] ++;
                if (y1<Board_Logical_Size && x1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][2]++;
            }; break;
            case 2:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][1]++;

                //Does it open on any side?
                if (y0-1>=0 && x0-1>=0 && board(x0,y0-1)=='.') pattern[firstDim][4] ++;
                if (y1<Board_Logical_Size && x1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block

        { ///Sub diagonal
            int x0=x,y0=y;
            while (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1, y0-1)==sig) {x0++; y0--;}
            int x1=x,y1=y;
            while (x1>=0 && y1<Board_Logical_Size && board(x1,y1)==sig) {x1--; y1++;}

            int patternsize = y1-y0;
            switch (patternsize) {
            case 5: {
                pattern[firstDim][5] ++;
            }; break;
            case 4:{
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.') pattern[firstDim][4] ++;
                if (y1<Board_Logical_Size && x1>=0 && board(x1,y1)=='.') pattern[firstDim][4]++;
            }; break;
            case 3:{
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][3]++;

                //Does it open on any side?
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.') pattern[firstDim][2] ++;
                if (y1<Board_Logical_Size && x1>=0 && board(x1,y1)=='.') pattern[firstDim][2]++;
            }; break;
            case 2:{
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][1]++;

                //Does it open on any side?
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.') pattern[firstDim][0] ++;
                if (y1<Board_Logical_Size && x1>=0 && board(x1,y1)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block
    } //for

    //calculate score
    const long long inf=1e18;
    //coEfficient
    long long co[2];
    co[1] = P1_Move_First ? 10 : 1;
    co[2] = P1_Move_First ? 1 : 10;

    if (pattern[1][7] > 0) return inf;
    if (pattern[2][7] > 0) return -inf;
//    if ((pattern[1][6]>0 || pattern[1][4]>0) && maximizer) return inf;
//    if ((pattern[2][6]>0 || pattern[2][4]>0) && !maximizer) return -inf;
    score += pattern[1][0]*co[1]*1ll - pattern[2][0]*co[2]*1ll;
    score += pattern[1][1]*co[1]*100ll - pattern[2][1]*co[2]*100ll;
    score += pattern[1][2]*co[1]*10000ll - pattern[2][2]*co[2]*10000ll;
    score += pattern[1][3]*co[1]*1000000ll - pattern[2][3]*co[2]*1000000ll;
    score += pattern[1][4]*co[1]*1000000000ll - pattern[2][4]*co[2]*1000000000ll;
    score += pattern[1][5]*co[1]*1000000000000ll - pattern[2][5]*co[2]*1000000000000ll;
    score += pattern[1][6]*co[1]*1000000000000000ll - pattern[2][6]*co[2]*1000000000000000ll;
    return score;
}

//parameter meaning: is this the turn of maximizer of not?
long long Evaluation_Caro (bool maximizer) {
    //number of patterns of type [second dimension] of player [first dimension]
    //first dimension: 1: maximizer (gryyna), 2: minimizer (yellos)
    //second dimension:
    //7 = -xxxxx-, 6 = xxxxx-, 5=-xxxx-, 4=xxxx-, 3=-xxx-, 2=xxx-, 1=-xx-, 0=-x-
    long long score = 0ll;
    int pattern[3][8];
    for (int i=0; i<3; ++i) for (int j=0; j<8; ++j) pattern[i][j] = 0;
    for (int x=0; x<Board_Logical_Size; ++x)
    for (int y=0; y<Board_Logical_Size; ++y)
    if (board(x,y)!='.' && board(x,y)!='S') {
        char sig = board(x,y);
        int firstDim = (board(x,y)=='1') ? 1 : 2;
        { ///Horizontal
            int x0=x;
            while (x0-1>=0 && board(x0-1, y)==sig) x0--;
            int x1=x;
            while (x1<Board_Logical_Size && board(x1,y)==sig) x1++;

            int patternsize = x1-x0;
            switch (patternsize) {
            case 5: {
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (x0-1>=0 && board(x0-1,y)=='.') pattern[firstDim][5] ++;
                if (x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][5]++;
            }; break;
            case 4:{
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][4]++;

                //Does it open on any side?
                if (x0-1>=0 && board(x0-1,y)=='.') pattern[firstDim][3] ++;
                if (x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][3]++;

            }; break;
            case 3:{
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][2]++;

                //Does it open on any side?
                if (x0-1>=0 && board(x0-1,y)=='.') pattern[firstDim][1] ++;
                if (x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][1]++;
            }; break;
            case 2:{
                if (x0-1>=0 && board(x0-1,y)=='.' && x1<Board_Logical_Size && board(x1,y)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block

        { ///Vertical
            int y0=y;
            while (y0-1>=0 && board(x, y0-1)==sig) y0--;
            int y1=y;
            while (y1<Board_Logical_Size && board(x,y1)==sig) y1++;

            int patternsize = y1-y0;
            switch (patternsize) {
            case 5: {
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][6]++;
                //Does it open on any side?
                if (y0-1>=0 && board(x,y0-1)=='.') pattern[firstDim][5] ++;
                if (y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][5]++;
            }; break;
            case 4:{
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][4]++;

                //Does it open on any side?
                if (y0-1>=0 && board(x,y0-1)=='.') pattern[firstDim][3] ++;
                if (y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][3]++;
            }; break;
            case 3:{
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][2]++;

                //Does it open on any side?
                if (y0-1>=0 && board(x,y0-1)=='.') pattern[firstDim][1] ++;
                if (y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][1]++;
            }; break;
            case 2:{
                if (y0-1>=0 && board(x,y0-1)=='.' && y1<Board_Logical_Size && board(x,y1)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block

        { ///Main diagonal
            int x0=x,y0=y;
            while (y0-1>=0 && x0-1>=0 && board(x0, y0)==sig) {x0--; y0--;}
            int x1=x,y1=y;
            while (x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)==sig) {x1++; y1++;}

            int patternsize = y1-y0;
            switch (patternsize) {
            case 5:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (y0-1>=0 && x0-1>=0 && board(x0,y0-1)=='.') pattern[firstDim][5] ++;
                if (y1<Board_Logical_Size && x1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][5]++;
            }; break;
            case 4:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][4]++;

                //Does it open on any side?
                if (y0-1>=0 && x0-1>=0 && board(x0,y0-1)=='.') pattern[firstDim][3] ++;
                if (y1<Board_Logical_Size && x1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][3]++;
            }; break;
            case 3:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][2]++;

                //Does it open on any side?
                if (y0-1>=0 && x0-1>=0 && board(x0,y0-1)=='.') pattern[firstDim][1] ++;
                if (y1<Board_Logical_Size && x1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][1]++;
            }; break;
            case 2:{
                if (y0-1>=0 && x0-1>=0 && board(x0-1,y0-1)=='.'
                    && x1<Board_Logical_Size && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block

        { ///Sub diagonal
            int x0=x,y0=y;
            while (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1, y0-1)==sig) {x0++; y0--;}
            int x1=x,y1=y;
            while (x1>=0 && y1<Board_Logical_Size && board(x1,y1)==sig) {x1--; y1++;}

            int patternsize = y1-y0;
            switch (patternsize) {
            case 5: {
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][6]++;

                //Does it open on any side?
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.') pattern[firstDim][5] ++;
                if (y1<Board_Logical_Size && x1>=0 && board(x1,y1)=='.') pattern[firstDim][5]++;
            }; break;
            case 4:{
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][4]++;

                //Does it open on any side?
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.') pattern[firstDim][3] ++;
                if (y1<Board_Logical_Size && x1>=0 && board(x1,y1)=='.') pattern[firstDim][3]++;
            }; break;
            case 3:{
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][2]++;

                //Does it open on any side?
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.') pattern[firstDim][1] ++;
                if (y1<Board_Logical_Size && x1>=0 && board(x1,y1)=='.') pattern[firstDim][1]++;
            }; break;
            case 2:{
                if (y0-1>=0 && x0+1<Board_Logical_Size && board(x0+1,y0-1)=='.'
                    && x1>=0 && y1<Board_Logical_Size && board(x1,y1)=='.') pattern[firstDim][0]++;
            }; break;
            } //switch
        } //block
    } //for

    //calculate score
    const long long inf=1e18;
    if (pattern[1][6]>0 || pattern[1][5]>0) return inf;
    if (pattern[2][6]>0 || pattern[2][5]>0) return -inf;
    if ((pattern[1][4]>0 && maximizer)) return inf;
    if ((pattern[2][4]>0 && !maximizer)) return -inf;
    score += pattern[1][0]*5 - pattern[2][0]*1;
    score += pattern[1][1]*(long long)5e4 - pattern[2][1]*(long long)1e4;
    score += pattern[1][2]*(long long)5e7 - pattern[2][2]*(long long)1e7;
    score += pattern[1][3]*(long long)5e10 - pattern[2][3]*(long long)1e10;
    score += pattern[1][4]*(long long)5e13 - pattern[2][4]*(long long)1e13;
    return score;
}

long long Evaluation (bool maximizer) {
    if (Current_Game_Mode & CARO_MODE)
        return Evaluation_Caro(maximizer);
    else return Evaluation_Gomoku(maximizer);
}

void Expand (short x, short y, CoordList &The_List, short radius) {
    for (short w=x-radius; w<=x+radius; ++w)
    for (short h=y-radius; h<=y+radius; ++h)
    if (w>=0 && h>=0 && w<Board_Logical_Size && h<Board_Logical_Size && board(w,h)=='.'
        && !Already_in_Child_List[h*MAX_BOARD_LOGICAL_SIZE+w]) {
        The_List.push_back({w,h});
    }
}

//Call this when Child_List is empty and don't know what to do?
void Expand_Initialize (short radius) {
    CoordList Expanded;
    for (short w=0; w<Board_Logical_Size; ++w)
    for (short h=0; h<Board_Logical_Size; ++h)
    if (board(w,h)!='.') {
        Expand(w,h,Expanded,radius);
        for (Coord c : Expanded)
        if (!Already_in_Child_List[c.y*MAX_BOARD_LOGICAL_SIZE+c.x]) {
            Child_List.push_back(c);
            Already_in_Child_List[c.y*MAX_BOARD_LOGICAL_SIZE+c.x] = 1;
        }
    }

    //Still empty?
    if (Child_List.empty()) {
        Expand(Board_Logical_Size/2, Board_Logical_Size/2, Child_List, radius);
    }
}
//Call this to automatically expand into Child_List instead of a custom list
void ExpandSP (short x, short y, short radius) {
    for (short w=x-radius; w<=x+radius; ++w)
            for (short h=y-radius; h<=y+radius; ++h) {
                    if (h<0 || w<0 || h>=Board_Logical_Size || w>=Board_Logical_Size) continue;
                    if ((board(w,h)=='.') && (Already_in_Child_List[h*MAX_BOARD_LOGICAL_SIZE+w]==false)) {
                        Already_in_Child_List[h+MAX_BOARD_LOGICAL_SIZE+w] = 1;
                        Child_List.push_back({w,h});
                    }
                }
}

long long Minimax (short x, short y, int depth, long long alpha, long long beta, bool maximizer, short expandRadius) {
    using namespace std;

    board(x,y) = (maximizer)?'1':'2'; Board_Num_Cell_Placed++;
    //is this move terminate the game?
    AMove Move = {1, (maximizer)?'1':'2', x, y};
    if (depth==0 || isWinningMove(Move)) {
        board(x,y) = (maximizer)?'1':'2'; Board_Num_Cell_Placed++;
        long long e = Evaluation (maximizer);
        board(x,y) = '.'; Board_Num_Cell_Placed--;
        return e;
    }

    board(x,y) = (maximizer)?'1':'2'; Board_Num_Cell_Placed++;

    //draw: no one benefits
    if (Board_Num_Cell_Placed == Board_Logical_Size*Board_Logical_Size) {
        board(x,y) = '.'; Board_Num_Cell_Placed--;
        return 0;
    }

    long long ans;
    if (maximizer) {
        for (Coord v : Child_List) {
            if (board(v.x, v.y)!='.') continue;

            CoordList Expanded;
            Expand(v.x,v.y,Expanded,expandRadius);
            for (Coord vEx : Expanded) {
                Already_in_Child_List[vEx.y*MAX_BOARD_LOGICAL_SIZE+vEx.x] = 1;
                Child_List.push_back(vEx);
            }

            long long tmp = Minimax(v.x, v.y, depth-1, alpha, beta, 0, expandRadius);
            alpha = max(alpha, tmp);

            for (Coord vEx : Expanded) {
                Already_in_Child_List[vEx.y*MAX_BOARD_LOGICAL_SIZE+vEx.x] = 0;
                Child_List.pop_back();
            }
            Expanded.clear();

            if (beta <= alpha) break; //a-b pruning
        }
        ans = alpha;
    }
    else {
        for (Coord v : Child_List) {
            if (board(v.x, v.y)!='.') continue;

            CoordList Expanded;
            Expand(v.x,v.y,Expanded,expandRadius);
            for (Coord vEx : Expanded) {
                Already_in_Child_List[vEx.y*MAX_BOARD_LOGICAL_SIZE+vEx.x] = 1;
                Child_List.push_back(vEx);
            }

            long long tmp = Minimax(v.x, v.y, depth-1, alpha, beta, 1, expandRadius);
            beta = min(beta, tmp);

            for (Coord vEx : Expanded) {
                Already_in_Child_List[vEx.y*MAX_BOARD_LOGICAL_SIZE+vEx.x] = 0;
                Child_List.pop_back();
            }
            Expanded.clear();

            if (beta <= alpha) break; //a-b pruning
        }
        ans = beta;
    }

    Board_Num_Cell_Placed--;
    board(x,y)='.';
    return ans;
}

void NextMove_Easy (bool maximizer, short &x, short &y) {
    using namespace std;
    short expandRadius = 1;
    int depthToGo = 1;
    long long inf=1e18;

    long long optimalVal = (maximizer)? -inf-1 : inf+1;

    if (Child_List.empty()) {
        Expand_Initialize (expandRadius);
    }

    for (Coord c : Child_List) {
        //detele those moves that are already moved.
        while (!Child_List.empty() && board(c.x, c.y)!='.') {
            swap(c, Child_List.back());
            Child_List.pop_back();
        }

        if (board(c.x, c.y)!='.') continue;

        CoordList Expanded;
        Expand(c.x, c.y, Expanded, expandRadius);
        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
            Child_List.push_back({cEx.x, cEx.y});
        }

        long long tmp = Minimax(c.x, c.y, depthToGo, -inf, inf, maximizer, expandRadius);
        if ((maximizer && tmp>optimalVal) || (!maximizer && tmp<optimalVal)) {
            optimalVal = tmp;
            x = c.x;
            y = c.y;
        }

        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 0;
            Child_List.pop_back();
        }
        Expanded.clear();
    }//for
}
void NextMove_Normal (bool maximizer, short &x, short &y) {
    using namespace std;
    short expandRadius = 1;
    int depthToGo = 3;
    long long inf=1e18;

    long long optimalVal = (maximizer)? -inf-1 : inf+1;

    if (Child_List.empty()) {
        Expand_Initialize (expandRadius);
    }

    for (Coord c : Child_List) {
        //detele those moves that are already moved.
        while (!Child_List.empty() && board(c.x, c.y)!='.') {
            swap(c, Child_List.back());
            Child_List.pop_back();
        }
        if (Child_List.empty()) break;

        if (board(c.x, c.y)!='.') continue;

        CoordList Expanded;
        Expand(c.x, c.y, Expanded, expandRadius);
        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
            Child_List.push_back({cEx.x, cEx.y});
        }

        long long tmp = Minimax(c.x, c.y, depthToGo, -inf, inf, maximizer, expandRadius);

        if ((maximizer && tmp>optimalVal) || (!maximizer && tmp<optimalVal)) {
            optimalVal = tmp;
            x = c.x;
            y = c.y;
        }

        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 0;
            Child_List.pop_back();
        }
        Expanded.clear();
    }//for
}
void NextMove_Hard (bool maximizer, short &x, short &y) {
    using namespace std;
    short expandRadius = 1;
    int depthToGo = 3;
    long long inf=1e18;

    long long optimalVal = (maximizer)? -inf-1 : inf+1;

    if (Child_List.empty()) {
        Expand_Initialize (expandRadius);
    }

    for (Coord c : Child_List) {
        //detele those moves that are already moved.
        while (!Child_List.empty() && board(c.x, c.y)!='.') {
            swap(c, Child_List.back());
            Child_List.pop_back();
        }
        if (Child_List.empty()) break;

        if (board(c.x, c.y)!='.') continue;

        CoordList Expanded;
        Expand(c.x, c.y, Expanded, expandRadius);
        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 1;
            Child_List.push_back({cEx.x, cEx.y});
        }

        long long tmp = Minimax(c.x, c.y, depthToGo, -inf, inf, maximizer, expandRadius);

        if ((maximizer && tmp>optimalVal) || (!maximizer && tmp<optimalVal)) {
            optimalVal = tmp;
            x = c.x;
            y = c.y;
        }

        for (Coord cEx : Expanded) {
            Already_in_Child_List [cEx.y*MAX_BOARD_LOGICAL_SIZE+cEx.x] = 0;
            Child_List.pop_back();
        }
        Expanded.clear();
    }//for
}
void NextMove (int ScreenMode, bool maximizer, short &x, short &y) {
    switch (ScreenMode) {
    case 1: NextMove_Easy(maximizer, x,y); break;
    case 2: NextMove_Normal(maximizer, x,y); break;
    case 3: NextMove_Hard(maximizer, x,y); break;
    }
}


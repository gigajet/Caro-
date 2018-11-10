#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include <vector>

typedef long long ll;

///Một nước đi
struct AMove {
    bool TurnLost;
    short Maker;
    short x, y;
};

typedef std::vector<AMove> MoveList;

struct GameData {
    int Screen_Mode;
    int Board_Logical_Size;
    int Player_1_Stone, Player_2_Stone;
    int Current_Game_Mode;
    MoveList Move_List;
};
#endif // _DATASTRUCT_H_

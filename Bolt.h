#ifndef _BOLT
#define _BOLT

#include "Player.h"

class Bolt: public Object
{
public:
    float speed[2], real_pos[2];
    bool dead;
    Player* attacks;

    Bolt(int x, int y, float sx, float sy, Player* attacking);
    ~Bolt();
    bool move_back();
    void update();
};

#endif // _BOLT

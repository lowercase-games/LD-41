#ifndef _PLAYER
#define _PLAYER

#include "Object.h"

class Player: public Object
{
public:
    float accurate_pos[2], speed[2];

    Player();

    void move_back();
    void update();
};

#endif // _PLAYER

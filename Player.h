#ifndef _PLAYER
#define _PLAYER

#include "Object.h"

enum Animations
{
    idle,walk,attack
};

class Player: public Object
{
    float accurate_pos[2], speed[2];
    Animations animation;
public:
    int dash_cooldown;
    const int max_dash_cooldown=60;

    Player();

    void move_back();
    void update();
    void dash();
};

#endif // _PLAYER

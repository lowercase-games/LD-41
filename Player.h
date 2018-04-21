#ifndef _PLAYER
#define _PLAYER

#include "Object.h"

enum Animations
{
    idle,walk,attack,tbd
};

class Player: public Object
{
    float accurate_pos[2], speed[2];
    Animations animation;
public:
    int dash_cooldown, claw_cooldown;
    const int max_dash_cooldown=60, max_claw_cooldown=40;

    Player();

    void move_back();
    void update();
    void dash();
    void claw_attack();
};

#endif // _PLAYER

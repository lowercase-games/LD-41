#ifndef _PLAYER
#define _PLAYER

#include "Object.h"

enum Animations
{
    idle,walk,claw,sting_up,sting_down,tbd
};

class Player: public Object
{
    float accurate_pos[2], speed[2];
    bool facing_up;
    Animations animation;
public:
    int dash_cooldown, claw_cooldown, sting_cooldown;
    const int max_dash_cooldown=60, max_claw_cooldown=40, max_sting_cooldown=80;

    Player();
    ~Player();

    void move_back();
    void update();
    bool in_attack_animation();

    void dash();
    void claw_attack();
    void sting_attack();
};

#endif // _PLAYER

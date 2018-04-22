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

    SDL_Rect other_hitbox;
    bool using_walk_hitbox;

    Player();
    ~Player();

    void move_back();
    void move(int x, int y, bool relative=true, bool set_last_pos=true);
    void update();
    bool in_attack_animation();
    void switch_hitbox();

    void dash();
    void claw_attack();
    void sting_attack();
};

#endif // _PLAYER

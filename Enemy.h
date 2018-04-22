#ifndef _ENEMY
#define _ENEMY

#include "Object.h"

class Weapon;

class Enemy: public Object
{
public:
    int time_till_move, attack_cooldown;
    const int max_attack_cooldown=80;
    bool crossbow;
    Object *focus_on;
    Weapon *weapon;

    Enemy(int x, int y, Object* focus, bool xbow);
    ~Enemy();

    void update();
    void render();
};

#endif // _ENEMY

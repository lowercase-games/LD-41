#ifndef _ENEMY
#define _ENEMY

#include "Object.h"

class Weapon;

class Enemy: public Object
{
public:
    int time_till_move;
    Object *focus_on;
    Weapon *weapon;

    Enemy(int x, int y, Object* focus);
    ~Enemy();

    void update();
};

#endif // _ENEMY

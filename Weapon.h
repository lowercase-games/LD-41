#ifndef _WEAPON
#define _WEAPON

#include "Object.h"

class Enemy;

class Weapon: public Object
{
public:
    Enemy *follow;

    Weapon(Enemy* follows);

    void update();
};

#endif // _WEAPON

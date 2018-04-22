#ifndef _WEAPON
#define _WEAPON

#include "Object.h"

class Enemy;

class Weapon: public Object
{
public:
    Enemy *follow;
    int attacking_for;

    Weapon(Enemy* follows);

    void update();
    void start_attack();
};

#endif // _WEAPON

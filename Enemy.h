#ifndef _ENEMY
#define _ENEMY

#include "Object.h"

class Enemy: public Object
{
public:
    int time_till_move;
    Object *focus_on;

    Enemy(int x, int y, Object* focus);
    ~Enemy();

    void update();
};

#endif // _ENEMY

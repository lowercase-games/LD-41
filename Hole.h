#ifndef _HOLE
#define _HOLE

#include "Object.h"

class Player;

class Hole : public Object
{
public:
    Player* get_entered_by;
    bool colliding;

    Hole(int x, int y, Player* enterer);
    void update();
};

#endif // _HOLE

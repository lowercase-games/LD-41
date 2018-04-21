#ifndef _WALL
#define _WALL

#include "Object.h"

class Wall: public Object
{
public:
    Wall(int x, int y, int sx, int sy);

    void update();
    void render();
};

#endif // _WALL

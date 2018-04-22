#ifndef _WALL
#define _WALL

#include "Object.h"

class Wall: public Object
{
public:
    Wall(int x, int y, int sx=-1, int sy=-1, std::string s="");

    void update();
    void render();
};

#endif // _WALL

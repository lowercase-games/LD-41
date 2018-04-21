#include "Wall.h"
#include "Rendering.h"

Wall::Wall(int x, int y, int sx, int sy) : Object(x,y,"",0,0,sx,sy)
{
    size[0] = sx;
    size[1] = sy;
}

void Wall::update()
{
    for (Object* o: wallables)
    {
        while (SDL_HasIntersection(cur_hitbox(),o->cur_hitbox()) == SDL_TRUE)
            o->move_back();
    }
}

void Wall::render()
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Rect r={pos[0], pos[1], size[0], size[1]};

    SDL_RenderFillRect(renderer, &r);
}

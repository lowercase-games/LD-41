#include "Wall.h"

#include <iostream>
#include "Rendering.h"

Wall::Wall(int x, int y, int sx, int sy, std::string s, int animation_frames) : Object(x,y,s,0,0,sx,sy,animation_frames)
{
    if (sx != -1) size[0] = sx;
    if (sy != -1) size[1] = sy;
}

void Wall::update()
{
    for (Object* o: wallables)
    {
        if (o->is_player && !(static_cast<Player*>(o)->using_walk_hitbox)) static_cast<Player*>(o)->switch_hitbox();

        if (SDL_HasIntersection(cur_hitbox(),o->cur_hitbox()) == SDL_TRUE)
        {
            int px = o->pos[0], py = o->pos[1];
            while (SDL_HasIntersection(cur_hitbox(),o->cur_hitbox()) == SDL_TRUE)
            {
                if (!o->move_back())
                {
                    while ((SDL_HasIntersection(cur_hitbox(),o->cur_hitbox()) == SDL_TRUE) && (o->pos[0]!=px || o->pos[1]!=py))
                        o->move(o->pos[0]-px, o->pos[1]-py, true, false);

                    while (SDL_HasIntersection(cur_hitbox(),o->cur_hitbox()) == SDL_TRUE)
                        o->move(0,1, true, false);
                }
            }

            if      (o->pos[0]-pos[0] == -o->size[0] || o->pos[0]-pos[0] == size[0]) o->move(o->pos[0],py,false);
            else if (o->pos[1]-pos[1] == -o->size[1] || o->pos[1]-pos[1] == size[1]) o->move(px,o->pos[1],false);
        }
    }
}

void Wall::render()
{
    if (tex)
    {
        Object::render();
    }
    else
    {
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_Rect r={pos[0]-camera[0], pos[1]-64-camera[1], size[0], size[1]-64};

        SDL_RenderFillRect(renderer, &r);
    }
}

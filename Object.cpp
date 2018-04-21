#include "Object.h"

#include "Rendering.h"

std::deque<Object*> objects, wallables;

int sign(int x)
{
    return (x>0?1:(x<0?-1:0));
}

Object::Object(int x, int y, std::string s, int hitx, int hity, int hitw, int hith)
{
    pos[0] = last_pos[0] = x;
    pos[1] = last_pos[1] = y;

    if (s.empty())
    {
        tex = nullptr;
        size[0] = size[1] = 0;
    }
    else
    {
        tex = load_image(s);
        SDL_QueryTexture(tex, nullptr, nullptr, &size[0], &size[1]);
    }

    if (hitw==-1) hitw = size[0];
    if (hith==-1) hith = size[1];

    hitbox = {hitx,hity,hitw,hith};
    current_hitbox = new SDL_Rect(hitbox);
    cur_hitbox();

    objects.push_back(this);
}

Object::~Object()
{
    remove_it(&objects, this);
    delete current_hitbox;
}

SDL_Rect* Object::cur_hitbox()
{
    current_hitbox->x = hitbox.x+pos[0];
    current_hitbox->y = hitbox.y+pos[1];
    return current_hitbox;
}

void Object::move(int x, int y, bool relative, bool set_last_pos)
{
    if (set_last_pos)
    {
        last_pos[0] = pos[0];
        last_pos[1] = pos[1];
    }

    if (relative)
    {
        pos[0] += x;
        pos[1] += y;
    }
    else
    {
        pos[0] = x;
        pos[1] = y;
    }
}

void Object::move_back()
{
    move(sign(last_pos[0]-pos[0]),sign(last_pos[1]-pos[1]), true, false);
}

void Object::render()
{
    SDL_Rect r={pos[0], pos[1], size[0], size[1]};

    SDL_RenderCopy(renderer, tex, nullptr, &r);
}

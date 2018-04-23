#include "Item.h"

namespace collected_items
{
    bool lamp=false;
}

Item::Item(int x, int y, std::string s, bool* trigger, Object* collector) : Object(x,y,s)
{
    collected = trigger;
    get_collected_by = collector;
}

void Item::update()
{
    if (SDL_HasIntersection(cur_hitbox(),get_collected_by->cur_hitbox()))
    {
        *collected = true;
        to_delete.push_back(this);
    }
}

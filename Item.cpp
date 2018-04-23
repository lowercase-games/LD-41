#include "Item.h"

namespace collected_items
{
    bool lamp=false, tentacle=false, deep_one_flesh=false, artifact=false;
    bool kill_cassy_quest_token=false, kill_ysa_quest_token=false;
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

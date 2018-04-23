#ifndef _ITEM
#define _ITEM

#include "Object.h"

namespace collected_items
{
    extern bool lamp, tentacle, deep_one_flesh, artifact;
    extern bool kill_cassy_quest_token, kill_ysa_quest_token;
}

class Item: public Object
{
public:
    bool* collected;
    Object* get_collected_by;

    Item(int x, int y, std::string s, bool* trigger, Object* collector);
    void update();
};

#endif // _ITEM

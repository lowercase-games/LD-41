#ifndef _ITEM
#define _ITEM

#include "Object.h"

namespace collected_items
{
    extern bool lamp;
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

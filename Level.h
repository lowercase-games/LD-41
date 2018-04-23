#ifndef _LEVEL
#define _LEVEL

class Object;
class Player;

extern int level;
extern bool load_next_level;

Player* load_level();
void load_cultists(Object* player);

#endif // _LEVEL

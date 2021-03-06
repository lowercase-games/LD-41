#ifndef _NPC
#define _NPC

#include "Wall.h"

class Player;
class Npc;
extern std::deque<Npc*> npcs;

enum npc_type
{
    leeta, cassy, ysa, kasaobake, kitsune
};

class Npc : public Wall
{
public:
    std::string name;
    int progress;
    npc_type type;
    bool just_spawned_cultists;

    Npc(int x, int y, std::string nam, npc_type typ, int hitx=0, int hity=0, int hitw=-1, int hith=-1, int animation_frames=1);
    ~Npc();
    bool interact(Object* interacter);
    void update();
};

#endif // _NPC

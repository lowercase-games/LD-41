#include "Npc.h"

#include "Dialog.h"

std::deque<Npc*> npcs;

Npc::Npc(int x, int y, std::string nam, npc_type typ, int hitx, int hity, int hitw, int hith): Wall(x,y,-1,-1,nam)
{
    name = char(toupper(nam[0]))+nam.substr(1);
    npcs.push_back(this);
    hitbox = {hitx,hity,hitw,hith};
    type = typ;
    progress = 0;
}

Npc::~Npc()
{
    remove_it(&npcs,this);
}

bool Npc::interact(Object* interacter)
{
    if (std::pow(pos[0]-interacter->pos[0],2)+std::pow(pos[1]-interacter->pos[1],2) < 100*100)
    {
        VN_from_file(name);
        return true;
    }

    return false;
}

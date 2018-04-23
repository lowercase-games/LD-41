#include "Npc.h"

#include <iostream>
#include "Dialog.h"
#include "Level.h"
#include "Item.h"

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
        std::string special = "";
        if (type == leeta)
        {
            if (progress == 0) {}
            else if (progress == 1)
            {
                if (enemies.empty())
                {
                    if (collected_items::lamp) special = "both";
                    else special = "no_artefact";
                }
                else special = "no_quest";
            }
            else return false; //there's nothing else to say
        }

        VN_from_file(name, special);

        if (!(affection[name[0]]%100) && affection[name[0]]) //ending was triggered
        {
            std::cout << "!!!";
            progress = 100;
            if (type == leeta)
            {
                change_animation("leeta_dead");
            }
        }
        else if (type == leeta && progress == 0)
        {
            std::cout << "???";
            progress++;
            load_cultists(interacter);
        }

        return true;
    }

    return false;
}

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
        else if (type == ysa)
        {
            if (progress == 0) progress++;
            else if (progress == 1)
            {
                if (collected_items::tentacle) special = "killed_cassy";
                else special = "kill_cassy_quest";
            }
            else return false; //there's nothing else to say
        }
        else if (type == cassy)
        {
            if (progress == 0) progress++;
            else if (progress == 1)
            {
                if (collected_items::kill_ysa_quest_token)
                {
                    if (collected_items::artifact && enemies.empty())
                    {
                        if (collected_items::deep_one_flesh) special = "killed_ysa";
                        else if (collected_items::kill_cassy_quest_token) special = "did_stuff_ysa";
                        else special = "did_stuff";
                    }
                    else
                    {
                        if (collected_items::kill_cassy_quest_token) special = "did_nothing_ysa";
                        else special = "did_nothing";
                    }
                }
                else special = "no_quest";
            }
            else return false; //there's nothing else to say
        }
        else if (type == kasaobake)
        {
            if (progress == 0) {}
            else if (progress == 1)
            {
                if (enemies.empty()) special = "done";
                else special = "not_done";
            }
            else return false; //there's nothing else to say
        }

        bool was_isa_quest = collected_items::kill_ysa_quest_token;

        VN_from_file(name, special);

        if (!(affection[name[0]]%100) && affection[name[0]]) //ending was triggered
        {
            progress = 100;
            if (type == leeta && affection[name[0]]==negative_end)
            {
                change_animation("leeta_dead");
            }
            else if (type == ysa && affection[name[0]]==dead_end)
            {
                collected_items::deep_one_flesh = true;
                to_delete.push_back(this);
            }
            else if (type == cassy && affection[name[0]]==dead_end)
            {
                collected_items::tentacle = true;
                to_delete.push_back(this);
            }
        }
        else if ((type == leeta && progress == 0) || (type == kasaobake && progress == 0 && collected_items::kasaobake_quest_token))
        {
            progress++;
            load_cultists(interacter);
        }
        else if (was_isa_quest != collected_items::kill_ysa_quest_token)
        {
            new Npc(1565,80,"ysa",ysa,22,42,24,21);
            load_cultists(interacter);
        }

        return true;
    }

    return false;
}

#include "Savestate.h"

#include "Item.h"
#include "Player.h"
#include "Level.h"
#include "Dialog.h"

Savestate* save = nullptr;

void Savestate::save(Player* player)
{
    lamp = collected_items::lamp;
    tentacle = collected_items::tentacle;
    deep_one_flesh = collected_items::deep_one_flesh;
    artifact = collected_items::artifact;
    kill_cassy_quest_token = collected_items::kill_cassy_quest_token;
    kill_ysa_quest_token = collected_items::kill_ysa_quest_token;
    kasaobake_quest_token = collected_items::kasaobake_quest_token;
    kitsune_token = collected_items::kitsune_token;

    affection_map = affection;

    player_pos[0] = player->pos[0];
    player_pos[1] = player->pos[1];
    cur_level = level;

    for (Npc* n: npcs)
    {
        npc_progress[n->type] = n->progress;
    }
}

Player* Savestate::load()
{
    collected_items::lamp = lamp;
    collected_items::tentacle = tentacle;
    collected_items::deep_one_flesh = deep_one_flesh;
    collected_items::artifact = artifact;
    collected_items::kill_cassy_quest_token = kill_cassy_quest_token;
    collected_items::kill_ysa_quest_token = kill_ysa_quest_token;
    collected_items::kasaobake_quest_token = kasaobake_quest_token;
    collected_items::kitsune_token = kitsune_token;

    affection = affection_map;
    cur_level = level;

    Player* player = load_level();
    load_cultists(player,false);

    player->pos[0] = player->accurate_pos[0] = player->last_pos[0] = player_pos[0];
    player->pos[1] = player->accurate_pos[1] = player->last_pos[1] = player_pos[1];

    for (Npc* n: npcs)
    {
        n->progress = npc_progress[n->type];
    }

    return player;
}

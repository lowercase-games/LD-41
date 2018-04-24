#ifndef _SAVESTATE
#define _SAVESTATE

#include <map>
#include "Npc.h"


class Player;
class Savestate;
extern Savestate* save;

class Savestate
{
    bool lamp, tentacle, deep_one_flesh, artifact;
    bool kill_cassy_quest_token, kill_ysa_quest_token, kasaobake_quest_token, kitsune_token;
    std::map<char, int> affection_map;
    std::map<npc_type, int> npc_progress;

    int player_pos[2];
    int cur_level;

public:
    Savestate();
    void save(Player* player);
    Player* load();
};

#endif // _SAVESTATE

#include "Level.h"

#include <fstream>
#include "Wall.h"
#include "Enemy.h"
#include "Rendering.h"
#include "Font.h"
#include "Npc.h"
#include "Item.h"
#include "Hole.h"
#include "Savestate.h"
#include "Sound.h"

int level=1;
bool load_next_level=false;

Player* load_level()
{
    while (!objects_update.empty()) delete objects_update[0];

    Player* new_player;

    if (level == 1)
    {
        new_player = new Player(50,950);
        new Npc(190,500,"leeta",leeta,17,40,31,23);
        if (!collected_items::lamp) new Item(1207,900,"lamp",&collected_items::lamp, new_player);
        new Hole(35,95, new_player);
    }
    else if (level == 2)
    {
        new_player = new Player(35,95);
        new Hole(1800,95, new_player);
        new Npc(755,810,"cassy",cassy,18,41,27,22);
        if (!collected_items::artifact) new Item(1620,684,"artifact",&collected_items::artifact, new_player);
        if (collected_items::kill_ysa_quest_token) new Npc(1565,80,"ysa",ysa,22,42,24,21);
    }
    else
    {
        new_player = new Player(1800,95);
        new Hole(943,94, new_player);
        new Npc(255,810,"kasaobake_jump",kasaobake,29,39,10,25,9);
        if (collected_items::kitsune_token)  new Npc(255,610,"kitsune",kitsune,22,49,32,15);
    }

    std::fstream file;
    file.open((std::string("Data")+PATH_SEPARATOR+"Levels"+PATH_SEPARATOR+"level"+std::to_string(level)+".txt"));

    while (!file.eof())
    {
        std::string line;
        std::getline(file,line);

        auto splitted = split(line,',');

        if (!line.empty()) new Wall(std::stoi(splitted[0]),std::stoi(splitted[1]),std::stoi(splitted[2]),std::stoi(splitted[3]));
    }

    file.close();

    new Wall(0,0,1920,64);
    new Wall(-64,0,64,1144);
    new Wall(0,1080+64,1920,128);
    new Wall(1920,0,64,1144);

    load_new_bg();

    play_music(load_music("theme_floor"+std::to_string(level)));

    return new_player;
}

void load_cultists(Object* player, bool save_game)
{
    if (save_game) save->save((Player*) player);

    std::fstream file;
    file.open((std::string("Data")+PATH_SEPARATOR+"Levels"+PATH_SEPARATOR+"level"+std::to_string(level)+"_cultists.txt"));

    while (!file.eof())
    {
        std::string line;
        std::getline(file,line);

        auto splitted = split(line,',');

        if (!line.empty()) new Enemy(std::stoi(splitted[0]),std::stoi(splitted[1]),player,std::stoi(splitted[2]));
    }

    file.close();
}

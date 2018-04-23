#include "Level.h"

#include <fstream>
#include "Wall.h"
#include "Enemy.h"
#include "Rendering.h"
#include "Font.h"
#include "Npc.h"
#include "Item.h"
#include "Hole.h"

int level=1;
bool load_next_level=false;

Player* load_level()
{
    while (!objects_update.empty()) delete objects_update[0];

    Player* player;

    if (level == 1)
    {
        player = new Player(50,950);
        new Npc(190,500,"leeta",leeta,17,40,31,23);
        new Item(1207,900,"lamp",&collected_items::lamp, player);
        new Hole(35,95, player);
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

    new Wall(0,0,1920,64);
    new Wall(-64,0,64,1144);
    new Wall(0,1080+64,1920,128);
    new Wall(1920,0,64,1080);

    return player;
}

void load_cultists(Object* player)
{
    std::fstream file;
    file.open((std::string("Data")+PATH_SEPARATOR+"Levels"+PATH_SEPARATOR+"level"+std::to_string(level)+"_cultists.txt"));

    while (!file.eof())
    {
        std::string line;
        std::getline(file,line);

        auto splitted = split(line,',');

        if (!line.empty()) new Enemy(std::stoi(splitted[0]),std::stoi(splitted[1]),player,std::stoi(splitted[2]));
    }
}

#include "Weapon.h"

#include "Enemy.h"

Weapon::Weapon(Enemy* follows): Object(follows->pos[0],follows->pos[1],"cult_axe",0,0,0,0)
{
    follow = follows;
}

void Weapon::update()
{
    pos[0] = follow->pos[0];
    pos[1] = follow->pos[1];

    rotation = -90-std::atan2(pos[0]-follow->focus_on->pos[0], pos[1]-follow->focus_on->pos[1])/M_PI*180;

    if (rotation<-180) rotation=-180;
    else if (rotation>0) rotation=0;

    flipped = (rotation > -90);
    if (!flipped) rotation += 180;
    follow->flipped = flipped;
}

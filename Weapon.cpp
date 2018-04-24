#include "Weapon.h"

#include <iostream>
#include "Enemy.h"
#include "Player.h"
#include "Bolt.h"
#include "Sound.h"

Weapon::Weapon(Enemy* follows): Object(follows->pos[0],follows->pos[1],follows->crossbow?"cult_xbow":"cult_axe",0,0,0,0,4)
{
    follow = follows;
    attacking_for = 0;
}

void Weapon::start_attack()
{
    if (follow->crossbow) play_sound(load_sound("enemy_bow"));
    else play_sound(load_sound("enemy_axe"));

    change_animation(follow->crossbow?"xbow_fire":"axe_zoosh");
    attacking_for = 360;
}

bool Weapon::intersects(SDL_Rect* rect)
{
    int circleDistancex = abs(pos[0]+size[0]/2 - (rect->x+rect->w/2)),
    circleDistancey = abs(pos[1]+size[1]/2 - (rect->y+rect->h/2));

    if (circleDistancex > (rect->w/2 + size[0]/2)) { return false; }
    if (circleDistancey > (rect->h/2 + size[0]/2)) { return false; }

    if (circleDistancex <= (rect->w/2)) { return true; }
    if (circleDistancey <= (rect->h/2)) { return true; }

    int cornerDistance_sq = std::pow(circleDistancex - rect->w/2,2) +
                         std::pow(circleDistancey - rect->h/2,2);

    return (cornerDistance_sq <= std::pow(size[0]/2,2));
}

void Weapon::update()
{
    pos[0] = follow->pos[0];
    pos[1] = follow->pos[1]+1;

    if (attacking_for>0)
    {
        if (follow->crossbow)
        {
            animate(5);
            if (!cur_anim_time)
            {
                if (!cur_anim_frame)
                {
                    attacking_for=0;
                    change_animation("cult_xbow");
                }
                else if (cur_anim_frame == 4)
                {
                    int dx = follow->focus_on->pos[0]-pos[0], dy = follow->focus_on->pos[1]-pos[1];
                    int d = std::sqrt(dx*dx+dy*dy);
                    if (d) new Bolt(pos[0]+size[0]/2,pos[1]+size[1]/2,7*dx/d,7*dy/d,(Player*) follow->focus_on);
                    else new Bolt(pos[0]+size[0]/2,pos[1]+size[1]/2,7,0,(Player*) follow->focus_on);
                }
            }
        }
        else
        {
            attacking_for-=20;
            rotation+=20*(flipped?1:-1);

            if (attacking_for<=0) change_animation("cult_axe");
            else
            {
                if (follow->focus_on->is_player && ((Player*) follow->focus_on)->using_walk_hitbox) ((Player*) follow->focus_on)->switch_hitbox();

                if (intersects(follow->focus_on->cur_hitbox())) follow->focus_on->attack();
            }
        }
    }

    if (follow->crossbow || attacking_for<=0)
    {
        if (attacking_for<=0) cur_anim_frame = follow->cur_anim_frame;

        rotation = -90-std::atan2(pos[0]-follow->focus_on->pos[0], pos[1]-follow->focus_on->pos[1])/M_PI*180;

        if (!follow->crossbow)
        {
            if (rotation<-180) rotation=-180;
            else if (rotation>0) rotation=0;
        }

        flipped = (rotation > -90);
        if (!flipped) rotation += 180;
        follow->flipped = flipped;
    }
}

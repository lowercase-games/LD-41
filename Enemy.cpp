#include "Enemy.h"

#include "Weapon.h"
#include "Rendering.h"
#include "Dialog.h"

Enemy::Enemy(int x, int y, Object* focus, bool xbow) : Object(x,y,"cult_walk_"+std::to_string(random(1,5)),16,3,30,54,4)
{
    time_till_move = 0;
    attack_cooldown = 0;
    focus_on = focus;

    wallables.push_back(this);
    enemies.push_back(this);

    crossbow = xbow;
    max_hp = hp = crossbow?4:3;

    weapon = new Weapon(this);
}

Enemy::~Enemy()
{
    remove_it(&wallables, (Object*) this);
    remove_it(&enemies, (Object*) this);

    delete weapon;
}

void Enemy::update()
{
    if (iframes) iframes--;

    if (attack_cooldown)  attack_cooldown--;

    if (--time_till_move <= 0)
    {
        time_till_move = 2;
        animate(5);
        move(sign(focus_on->pos[0]-pos[0]),sign(focus_on->pos[1]-pos[1]));

        for (Object* e: enemies)
        {
            if (e == this) break;
            else
            {
                if (abs(e->pos[0]-pos[0])+abs(e->pos[1]-pos[1]) < 64)
                {
                    move(-sign(e->pos[0]-pos[0]),-sign(e->pos[1]-pos[1]),true,false);
                    e->move(sign(e->pos[0]-pos[0]),sign(e->pos[1]-pos[1]),true,false);
                }
            }
        }

        if (!attack_cooldown && !weapon->attacking_for && std::pow(pos[0]-focus_on->pos[0],2)+std::pow(pos[1]-focus_on->pos[1],2) < (crossbow?400*400:74*74))
        {
            attack_cooldown = max_attack_cooldown;
            weapon->start_attack();
        }
    }
}

void Enemy::render()
{
    Object::render();

    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_Rect r = {pos[0]+size[0]/4-camera[0],pos[1]-10-camera[1],size[0]/2*hp/max_hp,5};
    SDL_RenderFillRect(renderer,&r);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    r.w = size[0]/2;
    SDL_RenderDrawRect(renderer,&r);
}

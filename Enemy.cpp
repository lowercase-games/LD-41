#include "Enemy.h"

#include "Weapon.h"
#include "Rendering.h"

Enemy::Enemy(int x, int y, Object* focus) : Object(x,y,"cult_walk_1",16,3,30,54,4)
{
    time_till_move = 0;
    attack_cooldown = 0;
    focus_on = focus;

    wallables.push_back(this);
    enemies.push_back(this);

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

        if (!attack_cooldown && std::pow(pos[0]-focus_on->pos[0],2)+std::pow(pos[1]-focus_on->pos[1],2) < 74*74)
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
    SDL_Rect r = {pos[0]+size[0]/4,pos[1]-10,size[0]/2*hp/max_hp,5};
    SDL_RenderFillRect(renderer,&r);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    r.w = size[0]/2;
    SDL_RenderDrawRect(renderer,&r);
}

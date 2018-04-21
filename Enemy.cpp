#include "Enemy.h"

#include "Weapon.h"

Enemy::Enemy(int x, int y, Object* focus) : Object(x,y,"cult_walk_1",0,0,-1,-1,4)
{
    time_till_move = 0;
    focus_on = focus;

    wallables.push_back(this);
    enemies.push_back(this);

    new Weapon(this);
}

Enemy::~Enemy()
{
    remove_it(&wallables, (Object*) this);
    remove_it(&enemies, (Object*) this);
}

void Enemy::update()
{
    if (--time_till_move <= 0)
    {
        time_till_move = 2;
        animate(5);
        move(sign(focus_on->pos[0]-pos[0]),sign(focus_on->pos[1]-pos[1]));
    }
}

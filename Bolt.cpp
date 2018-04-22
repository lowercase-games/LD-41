#include "Bolt.h"

Bolt::Bolt(int x, int y, float sx, float sy, Player* attacking) : Object(x,y,"Bolt")
{
    dead = false;

    pos[0] -= size[0]/2;
    pos[1] -= size[1]/2;

    speed[0] = sx;
    speed[1] = sy;
    real_pos[0] = pos[0];
    real_pos[1] = pos[1];

    attacks = attacking;
    rotation = 180+std::atan2(speed[1], speed[0])/M_PI*180;

    wallables.push_back(this);
}

Bolt::~Bolt()
{
    remove_it(&wallables, (Object*) this);
}

bool Bolt::move_back()
{
    if (!dead)
    {
        to_delete.push_back(this);
        dead = true;
    }
    pos[0] = pos[1] = -50000;
    return true;
}

void Bolt::update()
{
    real_pos[0] += speed[0];
    real_pos[1] += speed[1];

    move(real_pos[0], real_pos[1], false);

    if ((pos[0] < -5000 || pos[0] > 5000 || pos[1] < -5000 || pos[1] > 5000) && !dead)
    {
        to_delete.push_back(this);
        dead = true;
    }

    if (attacks->using_walk_hitbox) attacks->switch_hitbox();
    if (SDL_HasIntersection(attacks->cur_hitbox(), cur_hitbox()))
    {
        attacks->attack();
        if (!dead)
        {
            to_delete.push_back(this);
            dead = true;
    }
    }
}

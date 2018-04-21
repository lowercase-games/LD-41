#include "Player.h"

const float lin_stop = 0.1, const_stop = 0.5, acceleration = 1.5;

Player::Player():Object(0,0,"Player")
{
    accurate_pos[0] = pos[0];
    accurate_pos[1] = pos[1];

    speed[0] = speed[1] = 0;
    wallables.push_back(this);
}

void Player::move_back()
{
    Object::move_back();
    accurate_pos[0] = pos[0];
    accurate_pos[1] = pos[1];
    speed[0] = speed[1] = 0;
}

void Player::update()
{
    auto keystate = SDL_GetKeyboardState(nullptr);
    int acc[2] = {0,0};
    if (keystate[SDL_SCANCODE_W]) acc[1] = -1;
    else if (keystate[SDL_SCANCODE_S]) acc[1] = 1;
    if (keystate[SDL_SCANCODE_A]) acc[0] = -1;
    else if (keystate[SDL_SCANCODE_D]) acc[0] = 1;

    if (acc[0] || acc[1])
    {
        for (int i=0;i<=1;i++)
        {
            speed[i] += acc[i]*acceleration;
            speed[i] -= speed[i]*lin_stop+sign(speed[i])*const_stop;
            accurate_pos[i] += speed[i];
        }

        move(accurate_pos[0], accurate_pos[1], false);
    }
    else
    {
        speed[0] = speed[1] = 0;
    }
}

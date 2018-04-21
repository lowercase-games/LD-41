#include "Player.h"

const float lin_stop = 0.1, const_stop = 0.5, acceleration = 1.5;
const int dash_speed = 20;

Player::Player():Object(0,0,"manticore_idle",0,0,-1,-1,4)
{
    accurate_pos[0] = pos[0];
    accurate_pos[1] = pos[1];

    dash_cooldown = claw_cooldown = sting_cooldown = 0;

    speed[0] = speed[1] = 0;
    wallables.push_back(this);

    animation = idle;
    facing_up=false;
}

Player::~Player()
{
    remove_it(&wallables, (Object*) this);
}

bool Player::in_attack_animation()
{
    return (animation == claw) || (animation == sting_up) || (animation == sting_down);
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

    if (in_attack_animation())
    {
        bool attacking = false;
        SDL_Rect attack_hitbox;

        if (animation == claw && (cur_anim_frame==2 || cur_anim_frame==3))
        {
            attack_hitbox.x = -10;
            attack_hitbox.y = 47;
            attack_hitbox.w = 30;
            attack_hitbox.h = 10;

            attacking = true;
        }
        else if (animation == sting_up && cur_anim_frame>=4)
        {
            attack_hitbox.x = 10;
            attack_hitbox.y = -10;
            attack_hitbox.w = 11;
            attack_hitbox.h = 30;

            attacking = true;
        }
        else if (animation == sting_down && cur_anim_frame>=4)
        {
            attack_hitbox.x = 21;
            attack_hitbox.y = 49;
            attack_hitbox.w = 11;
            attack_hitbox.h = 30;

            attacking = true;
        }

        if (attacking)
        {
            #include "Rendering.h"

            SDL_Rect real_hitbox = hitbox;
            hitbox = attack_hitbox;

            SDL_SetRenderDrawColor(renderer,255,0,0,255);
            SDL_RenderDrawRect(renderer,cur_hitbox());

            for (Object* o: enemies)
            {
                if (SDL_HasIntersection(cur_hitbox(),o->cur_hitbox())) o->attack();
            }

            hitbox = real_hitbox;
        }

        animate(5);
        if (!cur_anim_frame && !cur_anim_time)
            animation = tbd;
    }

    if (acc[0] || acc[1])
    {
        for (int i=0;i<=1;i++)
        {
            speed[i] += acc[i]*acceleration;
            speed[i] -= speed[i]*lin_stop+sign(speed[i])*const_stop;
            accurate_pos[i] += speed[i];
        }

        move(accurate_pos[0], accurate_pos[1], false);

        if (!in_attack_animation())
        {
            if (animation != walk)
            {
                animation = walk;
                change_animation("manticore_move");
            }

            if (int total_speed=(abs(speed[0])+abs(speed[1]))) animate(25/total_speed);

            if (speed[0]) flipped = speed[0] > 0;
            if (speed[1]) facing_up = speed[1] < 0;
        }
    }
    else
    {
        speed[0] = speed[1] = 0;

        if (!in_attack_animation())
        {
            if (animation != idle)
            {
                animation = idle;
                change_animation("manticore_idle");
            }
            animate(20);
        }
    }

    if (dash_cooldown) dash_cooldown--;
    if (claw_cooldown) claw_cooldown--;
    if (sting_cooldown) sting_cooldown--;
}

void Player::dash()
{
    if (dash_cooldown<=0)
    {
        auto keystate = SDL_GetKeyboardState(nullptr);

        if (keystate[SDL_SCANCODE_W]) speed[1] -= dash_speed;
        else if (keystate[SDL_SCANCODE_S]) speed[1] += dash_speed;
        if (keystate[SDL_SCANCODE_A]) speed[0] -= dash_speed;
        else if (keystate[SDL_SCANCODE_D]) speed[0] += dash_speed;

        dash_cooldown=max_dash_cooldown;
    }
}

void Player::sting_attack()
{
    if (claw_cooldown<=0 && !in_attack_animation())
    {
        auto keystate = SDL_GetKeyboardState(nullptr);

        if (keystate[SDL_SCANCODE_S]) facing_up = false;
        else if (keystate[SDL_SCANCODE_W]) facing_up = true;

        sting_cooldown=max_sting_cooldown;
        if (facing_up)
        {
            change_animation("manticore_sting_up");
            animation = sting_up;
        }
        else
        {
            change_animation("manticore_sting_down");
            animation = sting_down;
        }
    }
}

void Player::claw_attack()
{
    if (claw_cooldown<=0 && !in_attack_animation())
    {
        auto keystate = SDL_GetKeyboardState(nullptr);

        if (keystate[SDL_SCANCODE_A]) flipped = false;
        else if (keystate[SDL_SCANCODE_D]) flipped = true;

        claw_cooldown=max_claw_cooldown;
        animation = claw;
        change_animation("manticore_attack");
    }
}

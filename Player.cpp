#include "Player.h"

#include "Sound.h"

int easy_mode = 0;

const float lin_stop = 0.1, const_stop = 0.5, acceleration = 1.2;
const int dash_speed = 20;

Player::Player(int x, int y):Object(x,y,"manticore_idle",2,44,51,19,8)
{
    accurate_pos[0] = pos[0];
    accurate_pos[1] = pos[1];

    dash_cooldown = claw_cooldown = sting_cooldown = 0;

    speed[0] = speed[1] = 0;
    wallables.push_back(this);

    animation = idle;
    facing_up=false;

    using_walk_hitbox=true;
    other_hitbox = {2,26,51,25};

    hp = max_hp = easy_mode?30:10;

    is_player = true;
}

Player::~Player()
{
    remove_it(&wallables, (Object*) this);
}

bool Player::in_attack_animation()
{
    return (animation == claw) || (animation == sting_up) || (animation == sting_down);
}

bool Player::move_back()
{
    if (!Object::move_back()) return false;

    accurate_pos[0] = pos[0];
    accurate_pos[1] = pos[1];
    //speed[0] = speed[1] = 0;
    return true;
}

void Player::move(int x, int y, bool relative, bool set_last_pos)
{
    Object::move(x,y,relative,set_last_pos);
    accurate_pos[0] = pos[0];
    accurate_pos[1] = pos[1];
}

void Player::update()
{
    if (iframes) iframes--;

    auto keystate = SDL_GetKeyboardState(nullptr);
    int acc[2] = {0,0};
    if (keystate[SDL_SCANCODE_W]) acc[1] = -1;
    else if (keystate[SDL_SCANCODE_S]) acc[1] = 1;
    if (keystate[SDL_SCANCODE_A]) acc[0] = -1;
    else if (keystate[SDL_SCANCODE_D]) acc[0] = 1;

    if (in_attack_animation())
    {
        bool attacking = false;
        SDL_Rect attack_hitbox = {0,0,0,0};

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

        if (!in_attack_animation())
        {
            int real_speed[2] = {pos[0]-last_pos[0],pos[1]-last_pos[1]};

            if (animation != walk)
            {
                animation = walk;
                change_animation("manticore_move");
            }

            if (int total_speed=(abs(real_speed[0])+abs(real_speed[1]))) animate(25/total_speed);

            if (real_speed[0]) flipped = real_speed[0] > 0;
            if (real_speed[1]) facing_up = real_speed[1] < 0;
        }

        move(accurate_pos[0], accurate_pos[1], false);
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

void Player::switch_hitbox()
{
    SDL_Rect temp = hitbox;
    hitbox = other_hitbox;
    other_hitbox = temp;
    using_walk_hitbox = !using_walk_hitbox;
}

void Player::dash()
{
    if (dash_cooldown<=0)
    {
        auto keystate = SDL_GetKeyboardState(nullptr);

        bool moved_y=true;

        if (keystate[SDL_SCANCODE_W]) speed[1] -= dash_speed;
        else if (keystate[SDL_SCANCODE_S]) speed[1] += dash_speed;
        else moved_y=false;

        if (keystate[SDL_SCANCODE_A]) speed[0] -= dash_speed;
        else if (keystate[SDL_SCANCODE_D]) speed[0] += dash_speed;
        else if (!moved_y) return;

        play_sound(load_sound("dash"));
        dash_cooldown=max_dash_cooldown;
        iframes = 10;
    }
}

void Player::sting_attack()
{
    if (sting_cooldown<=0 && !in_attack_animation())
    {
        auto keystate = SDL_GetKeyboardState(nullptr);

        if (keystate[SDL_SCANCODE_S]) facing_up = false;
        else if (keystate[SDL_SCANCODE_W]) facing_up = true;

        play_sound(load_sound("attack_sting"));
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

        play_sound(load_sound("attack_claw"));

        claw_cooldown=max_claw_cooldown;
        animation = claw;
        change_animation("manticore_attack");
    }
}

#include "Object.h"

#include "Rendering.h"
#include "Sound.h"

std::deque<Object*> objects_update, objects_render, wallables, enemies, to_delete;

int sign(int x)
{
    return (x>0?1:(x<0?-1:0));
}

Object::Object(int x, int y, std::string s, int hitx, int hity, int hitw, int hith, int animation_frames)
{
    in_bg = false;

    hp = max_hp = 5;
    rotation = 0;
    iframes = 0;

    pos[0] = last_pos[0] = x;
    pos[1] = last_pos[1] = y;

    flipped = false;

    anim_frames = animation_frames;
    cur_anim_frame = cur_anim_time = 0;

    if (s.empty())
    {
        tex = nullptr;
        size[0] = size[1] = 0;
    }
    else
    {
        tex = load_image(s);
        SDL_QueryTexture(tex, nullptr, nullptr, &size[0], &size[1]);
        size[1] /= anim_frames;
    }

    if (hitw==-1) hitw = size[0];
    if (hith==-1) hith = size[1];

    hitbox = {hitx,hity,hitw,hith};
    current_hitbox = new SDL_Rect(hitbox);
    cur_hitbox();

    objects_render.push_back(this);
    objects_update.push_back(this);

    is_player = false;
}

Object::~Object()
{
    remove_it(&objects_render, this);
    remove_it(&objects_update, this);
    delete current_hitbox;
}

void Object::attack()
{
    if (!iframes)
    {
        if (--hp <= 0)
        {
            if (is_player)
            {
                load_save = true;
                //play_sound(load_sound("player_death"));
                show_screen("death_screen");
            }
            else
            {
                play_sound(load_sound("enemy_death"));
                to_delete.push_back(this);
            }
        }
        else
        {
            if (is_player) play_sound(load_sound("player_hit"));
            else play_sound(load_sound("enemy_hit"));
            iframes = 16;
        }
    }
}

void Object::change_animation(std::string s)
{
    tex = load_image(s);
    cur_anim_time = cur_anim_frame = 0;

    int sy;
    SDL_QueryTexture(tex, nullptr, nullptr, nullptr, &sy);
    anim_frames = sy/size[1];
}

void Object::animate(int frame_time)
{
    cur_anim_time++;
    if (cur_anim_time >= frame_time)
    {
        cur_anim_time = 0;
        cur_anim_frame = (cur_anim_frame+1)%anim_frames;
    }
}

SDL_Rect* Object::cur_hitbox()
{
    if (flipped)
    {
        current_hitbox->x = -hitbox.x-hitbox.w+pos[0]+size[0];
        current_hitbox->y = hitbox.y+pos[1];
    }
    else
    {
        current_hitbox->x = hitbox.x+pos[0];
        current_hitbox->y = hitbox.y+pos[1];
    }

    current_hitbox->w = hitbox.w;
    current_hitbox->h = hitbox.h;

    return current_hitbox;
}

void Object::move(int x, int y, bool relative, bool set_last_pos)
{
    if (set_last_pos)
    {
        last_pos[0] = pos[0];
        last_pos[1] = pos[1];
    }

    if (relative)
    {
        pos[0] += x;
        pos[1] += y;
    }
    else
    {
        pos[0] = x;
        pos[1] = y;
    }
}

bool Object::move_back()
{
    if (last_pos[0] == pos[0] && last_pos[1] == pos[1]) return false;

    move(sign(last_pos[0]-pos[0]),sign(last_pos[1]-pos[1]), true, false);
    return true;
}

void Object::render()
{
    SDL_Rect dest={pos[0]-camera[0], pos[1]-camera[1], size[0], size[1]}, src = {0, size[1]*cur_anim_frame, size[0], size[1]};

    SDL_RenderCopyEx(renderer, iframes?white_texture(tex):tex, &src, &dest, rotation, nullptr, flipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
}

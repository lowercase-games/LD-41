#include "Hole.h"

#include "Level.h"
#include "Dialog.h"
#include "Rendering.h"
#include "Player.h"

Hole::Hole(int x, int y, Player* enterer) : Object(x,y,"hole"+std::to_string(level))
{
    get_entered_by = enterer;
    colliding= false;
    in_bg = true;
}

void Hole::update()
{
    if (get_entered_by->using_walk_hitbox) get_entered_by->switch_hitbox();

    if (SDL_HasIntersection(cur_hitbox(),get_entered_by->cur_hitbox()))
    {
        if (!colliding)
        {
            colliding = true;
            if (level==1 && !(!(affection['L']%100) && affection['L']))
            {
                SDL_RenderSetLogicalSize(renderer, window[0]*scale, window[1]*scale);
                dialog("(I feel like someone is watching me.)","",make_dialog_bg());
                dialog("(I can't leave yet.)","",make_dialog_bg());
                SDL_RenderSetLogicalSize(renderer, window[0], window[1]);
            }
            else if (level==2 && !(!(affection['C']%100) && affection['C']) && !(!(affection['Y']%100) && affection['Y']))
            {
                SDL_RenderSetLogicalSize(renderer, window[0]*scale, window[1]*scale);
                dialog("(Something seems fishy around here.)","",make_dialog_bg());
                dialog("(I guess I should take another look around.)","",make_dialog_bg());
                SDL_RenderSetLogicalSize(renderer, window[0], window[1]);
            }
            else if (level==3 && !(!(affection['K']%100) && affection['K']))
            {
                SDL_RenderSetLogicalSize(renderer, window[0]*scale, window[1]*scale);
                dialog("Beware the umbrella shouldn't be left there forever.","",make_dialog_bg());
                dialog("(I have no idea where that just came from.)","",make_dialog_bg());
                SDL_RenderSetLogicalSize(renderer, window[0], window[1]);
            }
            else load_next_level = true;
        }
    }
    else colliding = false;
}

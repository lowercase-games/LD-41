#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Object.h"
#include "Rendering.h"
#include "Player.h"
#include "Wall.h"
#include "Font.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Dialog.h"
#include "Npc.h"
#include "Level.h"
#include "Item.h"
#include "Savestate.h"
#include "Sound.h"

#ifndef _STATIC
void *__gxx_personality_v0;
#endif

const int window[2] = {640,360}, scale=3;
int camera[2] = {0,0};
Player* player;

bool breakk = false, load_save = false;

void move_camera(Object* to)
{
    static int camera_speed[2] = {0,0};

    int dx = to->pos[0]-window[0]/2-camera[0], dy = to->pos[1]-window[1]/2-camera[1];

    if (abs(dx) > window[0]/3) camera[0] = to->pos[0]-window[0]/2-sign(dx)*window[0]/3;
    if (abs(dy) > window[1]/3) camera[1] = to->pos[1]-window[1]/2-sign(dy)*window[1]/3;

    if (abs(dx)/6 > camera_speed[0]) camera_speed[0]++;
    else camera_speed[0] = abs(dx)/6;

    if (abs(dy)/6 > camera_speed[1]) camera_speed[1]++;
    else camera_speed[1] = abs(dy)/6;

    camera[0] += sign(dx)*std::min(1+camera_speed[0],abs(dx));
    camera[1] += sign(dy)*std::min(1+camera_speed[1],abs(dy));
}

bool by_y(Object* a, Object* b)
{
    return a->in_bg || (!b->in_bg && a->pos[1]+a->size[1] < b->pos[1]+b->size[1]);
}

void load_options()
{
    std::fstream file;
    file.open("options.txt");

    while (!file.eof())
    {
        std::string line;
        std::getline(file,line);

        auto splitted = split(line,'=');

        if (splitted[0] == "fullscreen") fullscreen = std::stoi(splitted[1]);
        else if (splitted[0] == "vsync") vsync = std::stoi(splitted[1]);
        else if (splitted[0] == "instant_text") instant_text = std::stoi(splitted[1]);
        else if (splitted[0] == "waifu_mode") easy_mode = std::stoi(splitted[1]);
        //else if (splitted[0] == "zoom") zoom = std::stoi(splitted[1]);
        else if (splitted[0] == "sfx_volume") sfx_volume = std::stoi(splitted[1]);
        else if (splitted[0] == "music_volume") music_volume = std::stoi(splitted[1]);
    }
}

#define option_num 6
void menu()
{
    int log_w,log_h;
    SDL_RenderGetLogicalSize(renderer,&log_w,&log_h);
    SDL_RenderSetLogicalSize(renderer,window[0],window[1]);

    std::string text[option_num] = {"Fullscreen","WaifuMode","Instant Text","SFX Volume","Music Volume","Exit Game"};
    int *value[option_num] = {&fullscreen, &easy_mode, &instant_text, &sfx_volume, &music_volume, nullptr};
    int range_max[option_num] = {1,1,1,128,128,0};
    int range_min[option_num] = {0,0,0,0,0,0};
    int pointer = 0;

    int w,h;
    SDL_Texture* pointer_tex = load_image("pointer");
    SDL_QueryTexture(pointer_tex,nullptr,nullptr,&w,&h);


    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN)
			{
			    if (e.key.keysym.sym == SDLK_ESCAPE)
			    {
			        if (easy_mode && player->max_hp == 10)
                    {
                        player->max_hp = 30;
                        player->hp *= 3;
                    }
                    else if (!easy_mode && player->max_hp == 30)
                    {
                        player->max_hp = 10;
                        player->hp /= 3;
                        if (!player->hp) player->hp = 1;
                    }

                    SDL_RenderSetLogicalSize(renderer,log_w,log_h);
			        return;
			    }
			    else if (e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d)
                {
                    if (value[pointer])
                    {
                        if (e.key.keysym.sym == SDLK_a) --(*value[pointer]);
                        else                            ++(*value[pointer]);;

                        if (*value[pointer] > range_max[pointer]) *value[pointer] = range_min[pointer];
                        if (*value[pointer] < range_min[pointer]) *value[pointer] = range_max[pointer];

                        render_init_update();
                        sound_init_update();
                    }
                    else if (e.key.keysym.sym == SDLK_e) breakk = true;
                }
			    else if (e.key.keysym.sym == SDLK_w)
                {
                    pointer--;

                    if (pointer<0) pointer = option_num-1;
                }
			    else if (e.key.keysym.sym == SDLK_s)
                {
                    pointer++;

                    if (pointer>=option_num) pointer = 0;
                }
			}
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        for (int i=0; i<option_num;i++)
        {
            render_text(50,30+i*50,text[i]+(value[i]?": "+std::to_string(*value[i]):""),255,255,false,false);
        }

        SDL_Rect r = {65-w,38+pointer*50,w,h};
        SDL_RenderCopy(renderer,pointer_tex,nullptr,&r);

        SDL_RenderPresent(renderer);
        limit_fps();
    }
}

int main(int argc, char* args[])
{
    random_init();
    load_options();

    render_init();
    font_init();
    sound_init();

    show_screen("start_screen");

    player = load_level();

    save = new Savestate(player);

    camera[0] = player->pos[0]-window[0]/2;
    camera[1] = player->pos[1]-window[1]/2;

    //SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN)
			{
			    if (e.key.keysym.sym == SDLK_ESCAPE) menu();
			    else if (e.key.keysym.sym == SDLK_e)
			    {
			        for (Npc* n: npcs)
                    {
                        if (n->interact(player)) break;
                    }
			    }
			    //else if (e.key.keysym.sym == SDLK_r) end_screen();
			    //else if (e.key.keysym.sym == SDLK_r) load_next_level = true;
			    else if (e.key.keysym.sym == SDLK_l) player->dash();
			    else if (e.key.keysym.sym == SDLK_j) player->claw_attack();
			    else if (e.key.keysym.sym == SDLK_k) player->sting_attack();
			}
        }

        if (load_next_level)
        {
            load_next_level = false;
            level++;
            player = load_level();
        }
        else if (load_save)
        {
            load_save = false;
            player = save->load();
        }

        //Updating
        for (Object* o: objects_update) o->update();
        for (Object* o: to_delete) delete o;
        to_delete.clear();

        move_camera((Object*) player);

        //Rendering
        render_bg();

        std::stable_sort(objects_render.begin(), objects_render.end(), by_y);
        for (Object* o: objects_render) o->render();
        render_ui(player);

        SDL_RenderPresent(renderer);
        limit_fps();
    }

    return 0;
}

#include <SDL.h>
#include <string>
#include <iostream>

#include "Object.h"
#include "Rendering.h"
#include "Player.h"
#include "Wall.h"
#include "Font.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Dialog.h"
#include "Npc.h"

#ifndef _STATIC
void *__gxx_personality_v0;
#endif

const int window[2] = {640,360}, scale=3;
int camera[2] = {0,0};

bool breakk = false;

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
    return a->pos[1] < b->pos[1];
}

int main(int argc, char* args[])
{
    render_init();
    font_init();

    Player* player = new Player();
    new Enemy(200,800,player,true);
    new Enemy(800,200,player,false);
    //new Wall(80,50,100,50);
    new Npc(200,200,"leeta",leeta,17,40,31,23);

    camera[0] = player->pos[0]-window[0]/2;
    camera[1] = player->pos[1]-window[1]/2;

    SDL_Texture* bg = load_image("bg");
    int bg_x, bg_y;
    SDL_QueryTexture(bg, nullptr, nullptr, &bg_x, &bg_y);

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
			    else if (e.key.keysym.sym == SDLK_l) player->dash();
			    else if (e.key.keysym.sym == SDLK_j) player->claw_attack();
			    else if (e.key.keysym.sym == SDLK_k) player->sting_attack();
			}
        }

        //Updating
        for (Object* o: objects_update) o->update();
        for (Object* o: to_delete) delete o;
        to_delete.clear();

        move_camera((Object*) player);

        //Rendering
        render_bg(bg, bg_x, bg_y);

        std::stable_sort(objects_render.begin(), objects_render.end(), by_y);
        for (Object* o: objects_render) o->render();
        render_ui(player);

        SDL_RenderPresent(renderer);
        limit_fps();
    }

    return 0;
}

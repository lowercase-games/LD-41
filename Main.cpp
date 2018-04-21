#include <SDL.h>
#include <SDL_image.h>
#include <deque>
#include <map>
#include <string>
#include <iostream>
#include <random>
#include <time.h>

#include "Object.h"
#include "Rendering.h"
#include "Player.h"
#include "Wall.h"
#include "Font.h"

#ifndef _STATIC
void *__gxx_personality_v0;
#endif

const int window[2] = {500,500};

bool breakk = false;

int last_time;
float wait;
void limit_fps()
{
    wait = (100.0/6)-(SDL_GetTicks() - last_time);
    if (wait>0) SDL_Delay(wait);
    last_time = SDL_GetTicks();
}

std::default_random_engine generator;
int random(int x, int y)
{
    std::uniform_int_distribution<int> distribution(x,y);
    return distribution(generator);
}

void random_init()
{
    generator.seed(time(nullptr));
    random(0,1);
}

void menu()
{
    //TODO
    breakk = true;
}

bool movement=false;
int pos_x = -1;
#define direction (pos_x<window[0]/4 ? 1:(pos_x+me_w>window[0]*3/4 ? -1:(random(0,1) ? -1:1)))
void dialog(std::string text, std::string image="placeholder")
{
    int progress = 0, frames_since_progress=0;

    add_newlines(text , window[0]-20);

    SDL_Texture *me = load_image(image), *bg = load_image("bg");
    int me_h, me_w;
    SDL_QueryTexture(me,nullptr,nullptr,&me_w,&me_h);

    if (pos_x == -1) pos_x = (window[0]-me_w)/2;

    SDL_Rect me_rect = {pos_x, window[1]-150-me_h, me_w, me_h};
    SDL_Rect textbox_rect = {5,window[1]-150,window[0]-10, 150-5};

    if (text.empty()) me_rect.y = (window[1]-me_h)/2;

    int to_move = 35*direction;

    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN)
			{
			    if (e.key.keysym.sym == SDLK_ESCAPE) menu();
			    if (e.key.keysym.sym == SDLK_RETURN) return;
			    {
			        if (progress < text.length()) progress = text.length();
			        else return;
			    }
			}
        }

        SDL_RenderCopy(renderer,bg,nullptr,nullptr);

        if (movement && to_move)
        {
            int mov = to_move/5;

            pos_x += mov;
            to_move -= mov;
        }

        me_rect.x = pos_x;
        SDL_RenderCopy(renderer,me,nullptr,&me_rect);

        if (!text.empty())
        {
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer,&textbox_rect);
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderDrawRect(renderer,&textbox_rect);

            render_text(10,window[1]-150,text.substr(0,progress),255,255, false);

            if (progress < text.length())
            {
                frames_since_progress++;
                if (frames_since_progress>=5) progress++;
            }
        }

        SDL_RenderPresent(renderer);
        limit_fps();
    }
}

int main(int argc, char* args[])
{
    render_init();
    font_init();

    Player* player = new Player();
    new Wall(80,50,100,50);

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
			    else if (e.key.keysym.sym == SDLK_e) dialog("TEST! Test. test? abc123 kasdl kasnd aso dnas idwpo dasdpwodp adi","");
			    else if (e.key.keysym.sym == SDLK_l) player->dash();
			    else if (e.key.keysym.sym == SDLK_j) player->claw_attack();
			}
        }

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);

        for (Object* o: objects) o->update();

        for (Object* o: objects) o->render();
        render_ui(player);

        SDL_RenderPresent(renderer);
        limit_fps();
    }

    return 0;
}

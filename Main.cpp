#include <SDL.h>
#include <SDL_image.h>
#include <deque>
#include <map>
#include <string>
#include <iostream>

#include "Object.h"
#include "Rendering.h"
#include "Player.h"
#include "Wall.h"

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

int main(int argc, char* args[])
{
    render_init();

    new Player();
    new Wall(50,50,100,50);

    //SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) breakk = true;
        }

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);

        for (Object* o: objects) o->update();
        for (Object* o: objects) o->render();

        SDL_RenderPresent(renderer);
        limit_fps();
    }

    return 0;
}

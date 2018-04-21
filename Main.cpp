#include <SDL.h>
#include <SDL_image.h>
#include <deque>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

#ifndef _STATIC
void *__gxx_personality_v0;
#endif

const int window[2] = {500,500};
const float lin_stop = 0.1, const_stop = 0.5, acceleration = 1.5;

bool breakk = false;
SDL_Window* renderwindow;
SDL_Renderer* renderer;

int last_time;
float wait;
void limit_fps()
{
    wait = (100.0/6)-(SDL_GetTicks() - last_time);
    if (wait>0) SDL_Delay(wait);
    last_time = SDL_GetTicks();
}

template<class t> void remove_it(std::deque<t>* base, t thing)
{
    base->erase( std::remove( std::begin(*base), std::end(*base), thing ), std::end(*base) );
}

std::map<std::string,SDL_Texture*> loaded_textures;
SDL_Texture* load_image(std::string s)
{
    if (!loaded_textures.count(s)) loaded_textures[s] = IMG_LoadTexture(renderer,("Data\\Graphics\\"+s+".png").c_str());

    return loaded_textures[s];
}

class Object;
std::deque<Object*> objects;
class Object
{
public:
    int pos[2], size[2];
    SDL_Texture* tex;

    Object(int x, int y, std::string s)
    {
        pos[0] = x;
        pos[1] = y;

        tex = load_image(s);
        SDL_QueryTexture(tex, nullptr, nullptr, &size[0], &size[1]);

        objects.push_back(this);
    }

    virtual ~Object()
    {
        remove_it(&objects, this);
    }

    virtual void update()
    {

    }

    virtual void render()
    {
        SDL_Rect r={pos[0], pos[1], size[0], size[1]};

        SDL_RenderCopy(renderer, tex, nullptr, &r);
    }
};

int sign(int x)
{
    return (x>0?1:(x<0?-1:0));
}

class Player: public Object
{
public:
    float accurate_pos[2], speed[2];

    Player():Object(0,0,"Player")
    {
        accurate_pos[0] = pos[0];
        accurate_pos[1] = pos[1];

        speed[0] = speed[1] = 0;
    }

    void update()
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
                pos[i] = (int) accurate_pos[i];
            }
        }
        else
        {
            speed[0] = speed[1] = 0;
        }
    }
};

int main(int argc, char* args[])
{
    IMG_Init(IMG_INIT_PNG);

    renderwindow = SDL_CreateWindow("...", 50, 50, window[0], window[1], SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(renderwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    new Player();

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

        for (Object* o: objects)
        {
            o->update();
            o->render();
        }

        SDL_RenderPresent(renderer);
        limit_fps();
    }

    return 0;
}

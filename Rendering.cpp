#include "Rendering.h"

#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <map>
#include "Font.h"

SDL_Window* renderwindow;
SDL_Renderer* renderer;

void render_init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    renderwindow = SDL_CreateWindow("LD-41", 50, 50, window[0], window[1], SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(renderwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

std::map<std::string,SDL_Texture*> loaded_textures;
SDL_Texture* load_image(std::string s)
{
    if (!loaded_textures.count(s)) loaded_textures[s] = IMG_LoadTexture(renderer,(std::string("Data")+PATH_SEPARATOR+"Graphics"+PATH_SEPARATOR+s+".png").c_str());

    return loaded_textures[s];
}

void draw_pie(int x, int cur, int max, std::string key)
{
    filledPieRGBA(renderer,x,window[1]-40,25,-90,271-360*cur/max,0,255,0,255);
    circleRGBA(renderer,x,window[1]-40,25,0,0,0,255);
    render_text(x-32,window[1]-72,key,0,255,false);
}

void render_ui(Player* player)
{
    draw_pie(window[0]-40,player->dash_cooldown,player->max_dash_cooldown,"L");
}

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

void draw_pie(int x, int cur, int max, std::string key, std::string tex)
{
    filledPieRGBA(renderer,x,window[1]-30,17,-90,271-360*cur/max,36,137,176,255);
    circleRGBA(renderer,x,window[1]-30,17,0,0,0,255);
    render_text(x-23,window[1]-53,key,0,255,false);

    SDL_Rect r = {x-16,window[1]-46,32,32};
    SDL_RenderCopy(renderer,load_image(tex),nullptr,&r);
}

void render_ui(Player* player)
{
    draw_pie(window[0]-120,player->claw_cooldown,player->max_claw_cooldown,"J","claw");
    draw_pie(window[0]-80,player->sting_cooldown,player->max_sting_cooldown,"K","sting");
    draw_pie(window[0]-40,player->dash_cooldown,player->max_dash_cooldown,"L","dash");

    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_Rect r = {5,5,50*player->hp / 5,10};
    SDL_RenderFillRect(renderer,&r);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    r.w = 50;
    SDL_RenderDrawRect(renderer,&r);
}

#include "Rendering.h"

#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <map>
#include "Font.h"
#include "Item.h"

SDL_Window* renderwindow;
SDL_Renderer* renderer;

bool fullscreen=false, vsync=false;

int last_time;
float wait;
void limit_fps()
{
    wait = (100.0/6)-(SDL_GetTicks() - last_time);
    if (wait>0) SDL_Delay(wait);
    last_time = SDL_GetTicks();
}

void render_init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_DisplayMode current;
    SDL_GetDesktopDisplayMode(0, &current);

    renderwindow = SDL_CreateWindow("LD 41", 50, 50, fullscreen?current.w:window[0]*scale, fullscreen?current.h:window[1]*scale,
                                    SDL_WINDOW_SHOWN | (fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0));
    renderer = SDL_CreateRenderer(renderwindow, -1, SDL_RENDERER_ACCELERATED | (vsync?SDL_RENDERER_PRESENTVSYNC:0));

    //SDL_RenderSetScale(renderer,scale,scale);
    SDL_RenderSetLogicalSize(renderer, window[0], window[1]);
}

std::map<std::string,SDL_Texture*> loaded_textures;
std::map<SDL_Texture*,SDL_Texture*> white_textures;
SDL_Texture* load_image(std::string s)
{
    if (!loaded_textures.count(s))
    {
        loaded_textures[s] = IMG_LoadTexture(renderer,(std::string("Data")+PATH_SEPARATOR+"Graphics"+PATH_SEPARATOR+s+".png").c_str());

        int w,h;
        Uint32 format;
        SDL_QueryTexture(loaded_textures[s],&format,nullptr,&w,&h);
        white_textures[loaded_textures[s]] = SDL_CreateTexture(renderer,format,SDL_TEXTUREACCESS_TARGET,w,h);

        SDL_SetRenderTarget(renderer,white_textures[loaded_textures[s]]);
        SDL_SetTextureBlendMode(white_textures[loaded_textures[s]], SDL_BLENDMODE_NONE);
        SDL_RenderCopy(renderer,loaded_textures[s],nullptr,nullptr);

        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_ADD);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_Rect r = {0,0,w,h};
        SDL_RenderFillRect(renderer,&r);

        SDL_SetRenderTarget(renderer,nullptr);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(white_textures[loaded_textures[s]], SDL_BLENDMODE_BLEND);
    }

    return loaded_textures[s];
}

SDL_Texture* white_texture(SDL_Texture* tex)
{
    return white_textures[tex];
}

void draw_pie(int x, int cur, int max, std::string key, std::string tex)
{
    filledPieRGBA(renderer,x,window[1]-30,17,-90,271-360*cur/max,205,255,199,255);
    circleRGBA(renderer,x,window[1]-30,17,0,0,0,255);
    render_text(x-23,window[1]-53,key,0,255,false,true);

    SDL_Rect r = {x-16,window[1]-46,32,32};
    SDL_RenderCopy(renderer,load_image(tex),nullptr,&r);
}

void render_bg(SDL_Texture* bg, int bg_x, int bg_y)
{
    if (bg_x == -1) SDL_QueryTexture(bg, nullptr, nullptr, &bg_x, &bg_y);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_Rect src = {std::min(bg_x-window[0],std::max(0,camera[0])),std::min(bg_y-window[1],std::max(0,camera[1])),window[0],window[1]};
    SDL_Rect dest = {std::min(bg_x-camera[0]-window[0],std::max(0,-camera[0])),std::min(bg_y-camera[1]-window[1],std::max(0,-camera[1])),window[0],window[1]};
    SDL_RenderCopy(renderer,bg,&src,&dest);
}

void render_ui(Player* player)
{
    draw_pie(window[0]-120,player->claw_cooldown,player->max_claw_cooldown,"J","claw");
    draw_pie(window[0]-80,player->sting_cooldown,player->max_sting_cooldown,"K","sting");
    draw_pie(window[0]-40,player->dash_cooldown,player->max_dash_cooldown,"L","dash");

    {
        using namespace collected_items;
        SDL_Rect r = {window[0]-135,window[1]-30,1,1};
        SDL_Texture* tex;
        if (lamp)
        {
            tex = load_image("lamp");
            SDL_QueryTexture(tex,nullptr,nullptr,&r.w, &r.h);
            r.x -= r.w+5;
            r.y = window[1]-30-r.h/2;
            SDL_RenderCopy(renderer,tex,nullptr,&r);
        }
        if (tentacle)
        {
            tex = load_image("tentacle");
            SDL_QueryTexture(tex,nullptr,nullptr,&r.w, &r.h);
            r.x -= r.w+5;
            r.y = window[1]-30-r.h/2;
            SDL_RenderCopy(renderer,tex,nullptr,&r);
        }
        if (deep_one_flesh)
        {
            tex = load_image("deep_one_flesh");
            SDL_QueryTexture(tex,nullptr,nullptr,&r.w, &r.h);
            r.x -= r.w+5;
            r.y = window[1]-30-r.h/2;
            SDL_RenderCopy(renderer,tex,nullptr,&r);
        }
        if (artifact)
        {
            tex = load_image("artifact");
            SDL_QueryTexture(tex,nullptr,nullptr,&r.w, &r.h);
            r.x -= r.w+5;
            r.y = window[1]-30-r.h/2;
            SDL_RenderCopy(renderer,tex,nullptr,&r);
        }
    }

    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_Rect r = {13,31,181*player->hp / player->max_hp,14};
    SDL_RenderFillRect(renderer,&r);

    SDL_Rect re = {0,0,201,55};
    SDL_RenderCopy(renderer,load_image("healthbar"),nullptr,&re);
}

#ifndef _RENDERING
#define _RENDERING

#include <SDL.h>
#include <string>
#include "Player.h"

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

//extern SDL_Window* renderwindow;
extern SDL_Renderer* renderer;
extern const int window[2], scale;
extern bool breakk, load_save;
extern int camera[2];

extern bool vsync;
extern int fullscreen;

void limit_fps();

void render_init();
void render_init_update();
SDL_Texture* load_image(std::string s);
void show_screen(std::string tex);
void show_screen(SDL_Texture *bg);
void render_ui(Player* player);
void load_new_bg();
void render_bg();
void end_screen();

SDL_Texture* white_texture(SDL_Texture* tex);

#endif // _RENDERING

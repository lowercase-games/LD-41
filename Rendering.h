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
extern bool breakk;
extern int camera[2];

void limit_fps();

void render_init();
SDL_Texture* load_image(std::string s);
void render_ui(Player* player);
void render_bg(SDL_Texture* bg, int bg_x=-1, int bg_y=-1);

SDL_Texture* white_texture(SDL_Texture* tex);

#endif // _RENDERING

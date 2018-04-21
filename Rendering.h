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

extern SDL_Window* renderwindow;
extern SDL_Renderer* renderer;
extern const int window[2], scale;

void render_init();
SDL_Texture* load_image(std::string s);
void render_ui(Player* player);

#endif // _RENDERING

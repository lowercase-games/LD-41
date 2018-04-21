#ifndef _RENDERING
#define _RENDERING

#include <SDL.h>
#include <string>

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

extern SDL_Window* renderwindow;
extern SDL_Renderer* renderer;
extern const int window[2];

void render_init();
SDL_Texture* load_image(std::string s);

#endif // _RENDERING

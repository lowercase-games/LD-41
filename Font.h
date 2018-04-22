#ifndef _FONT
#define _FONT

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

void font_init();
SDL_Texture* text_to_texture(std::string s, Uint8 brightness, bool ui);
int add_newlines(std::string &s, int width);
void render_text(int posx, int posy, std::string s, Uint8 brightness, Uint8 alpha,
                 bool m_aligned, bool ui);

#endif // _FONT


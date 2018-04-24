#include "Font.h"

#include <map>
#include <deque>
#include <iostream>
#include "Rendering.h"

std::deque<std::string> split(std::string s, char seperator)
{
    std::deque<std::string> ret;
    ret.push_back("");
    for (char c: s)
    {
        if (c == seperator) ret.push_back("");
        else ret[ret.size()-1] += c;
    }

    return ret;
}

TTF_Font *font, *font_ui;
void font_init()
{
    TTF_Init();
    font_ui = TTF_OpenFont((std::string("Data")+PATH_SEPARATOR+"Fonts"+PATH_SEPARATOR+"Roboto-Light.ttf").c_str(),16);
    font = TTF_OpenFont((std::string("Data")+PATH_SEPARATOR+"Fonts"+PATH_SEPARATOR+"Roboto-Light.ttf").c_str(),36);
}

std::map<std::pair<std::string,Uint8>,SDL_Texture*> loaded_texts;
SDL_Texture* text_to_texture(std::string s, Uint8 brightness, bool ui)
{
    std::pair<std::string,Uint8> p = {s, brightness};
    if (!loaded_texts.count(p)) loaded_texts[p] = SDL_CreateTextureFromSurface(renderer,TTF_RenderText_Solid((ui?font_ui:font), s.c_str(), {brightness,brightness,brightness,255}));
    return loaded_texts[p];
}

int add_newlines(std::string &s, int width)
{
    std::deque<std::string> splitt = split(s,' ');
    std::string ret = "", part="";
    int cur_width, height=0;
    for (std::string st: splitt)
    {
        if (st[0] == '\n')
        {
            ret += part+"\n";
            part = st.substr(1);
            ++height;
        }
        else
        {
            TTF_SizeText(font,(part+" "+st).c_str(), &cur_width, nullptr);

            if (cur_width <= width)
                part += (part.empty()?"":" ")+st;
            else
            {
                ret += part+"\n";
                part = st;
                ++height;
            }
        }
    }

    ret += part;
    ++height;

    s = ret;
    return height*TTF_FontLineSkip(font);
}

void render_text(int posx, int posy, std::string s, Uint8 brightness, Uint8 alpha, bool m_aligned, bool ui)
{
    int offset = 0;
    std::deque<std::string> splitt = split(s,'\n');
    for (auto st: splitt)
    {
        SDL_Texture* tex =text_to_texture(st, brightness, ui);
        SDL_SetTextureAlphaMod(tex,alpha);
        int size[2];
        SDL_QueryTexture(tex, nullptr, nullptr, &size[0], &size[1]);
        SDL_Rect r = {posx-m_aligned*size[0]/2, posy+offset, size[0], size[1]};
        SDL_RenderCopy(renderer, tex, nullptr, &r);

        offset += TTF_FontLineSkip(ui?font_ui:font);
    }
}


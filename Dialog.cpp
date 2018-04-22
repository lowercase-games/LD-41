#include "dialog.h"

#include <fstream>
#include <random>
#include <time.h>
#include <map>
#include <stack>
#include <iostream>
#include "random"
#include "Rendering.h"
#include "Font.h"

void menu()
{
    //TODO
    breakk = true;
}

std::default_random_engine generator;
int random(int x, int y)
{
    std::uniform_int_distribution<int> distribution(x,y);
    return distribution(generator);
}

void random_init()
{
    generator.seed(time(nullptr));
    random(0,1);
}

const bool movement=true;
int pos_x = -1;
#define direction (pos_x<window[0]*scale/4 ? 1:(pos_x+me_w>window[0]*scale*3/4 ? -1:(random(0,1) ? -1:1)))
void dialog(std::string text, std::string image, SDL_Texture* bg)
{
    int progress = 0, frames_since_progress=0;

    add_newlines(text , window[0]*scale-20);

    SDL_Texture *me = load_image(image);
    int me_h, me_w;
    SDL_QueryTexture(me,nullptr,nullptr,&me_w,&me_h);

    if (pos_x == -1) pos_x = (window[0]*scale-me_w)/2;

    SDL_Rect me_rect = {pos_x, window[1]*scale-me_h, me_w, me_h};
    SDL_Rect textbox_rect = {5,window[1]*scale-150,window[0]*scale-10, 150-5};

    if (text.empty()) me_rect.y = (window[1]*scale-me_h)/2;

    int to_move = 60*direction;

    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN)
			{
			    if (e.key.keysym.sym == SDLK_ESCAPE) menu();
			    else if (!e.key.repeat)
			    {
			        if (progress < text.length()) progress = text.length();
			        else return;
			    }
			}
        }

        if (movement && to_move)
        {
            int mov = to_move/5;

            pos_x += mov;
            to_move -= mov;
        }

        SDL_RenderCopy(renderer,bg,nullptr,nullptr);

        me_rect.x = pos_x;
        SDL_RenderCopy(renderer,me,nullptr,&me_rect);

        if (!text.empty())
        {
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer,&textbox_rect);
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderDrawRect(renderer,&textbox_rect);

            render_text(10,window[1]*scale-150,text.substr(0,progress),255,255, false, false);

            if (progress < text.length())
            {
                frames_since_progress++;
                if (frames_since_progress>=5) progress++;
            }
        }

        SDL_RenderPresent(renderer);
        limit_fps();
    }
}

int mod(int a, int m)
{
    a %= m;
    while (a<0) a += m;
    return a;
}

int choice(std::deque<std::string> texts, std::string image, SDL_Texture* bg)
{
    std::string text;
    int cursor=0;

    #define make_text text.clear();for (int i=0;i<texts.size();i++) text += (cursor==i?'>':' ')+texts[i]+'\n';

    make_text

    add_newlines(text, window[0]*scale-20);

    SDL_Texture *me = load_image(image);
    int me_h, me_w;
    SDL_QueryTexture(me,nullptr,nullptr,&me_w,&me_h);

    if (pos_x == -1) pos_x = (window[0]*scale-me_w)/2;

    SDL_Rect me_rect = {pos_x, window[1]*scale-me_h, me_w, me_h};
    SDL_Rect textbox_rect = {5,window[1]*scale-150,window[0]*scale-10, 150-5};

    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN)
			{
			    if (e.key.keysym.sym == SDLK_ESCAPE) menu();
			    else if (e.key.keysym.sym == SDLK_w)
			    {
			        cursor = mod(cursor-1,texts.size());
			        make_text
			    }
			    else if (e.key.keysym.sym == SDLK_s)
			    {
			        cursor = mod(cursor+1,texts.size());
			        make_text
			    }
			    else if (!e.key.repeat) return cursor;
			}
        }

        SDL_RenderCopy(renderer,bg,nullptr,nullptr);
        SDL_RenderCopy(renderer,me,nullptr,&me_rect);

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderFillRect(renderer,&textbox_rect);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderDrawRect(renderer,&textbox_rect);

        render_text(10,window[1]*scale-150,text,255,255, false, false);

        SDL_RenderPresent(renderer);
        limit_fps();
    }
}

void remove_leading_whitespace(std::string* s)
{
    while ((*s)[0] == '\t' || (*s)[0] == ' ') s->erase(0,1);
}

int file_position=0;
void getline(std::fstream& file, std::string& line)
{
    std::getline(file,line);
    remove_leading_whitespace(&line);
    file_position++;
}

void go_to_pos(std::fstream& file, int pos)
{
    file.clear();
    file.seekg(0,std::ios::beg);
    std::string line;
    for (int i=0;i<pos;i++)
    {
        std::getline(file,line);
    }
    file_position=pos;
}

std::map<char, int> affection;
std::stack<int> return_positions;
void VN_from_file(std::string filename, std::string special)
{
    bool to_return = false;

    std::string filename_lower = filename;
    std::transform(filename_lower.begin(), filename_lower.end(), filename_lower.begin(), ::tolower);

    SDL_RenderSetScale(renderer,1,1);
    SDL_Texture* bg = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,window[0],window[1]);
    SDL_SetRenderTarget(renderer,bg);
    render_bg(load_image("bg"));
    for (Object* o: objects_render) o->render();
    SDL_SetRenderTarget(renderer,nullptr);

    std::fstream file;
    file.open((std::string("Data")+PATH_SEPARATOR+"Dialog"+PATH_SEPARATOR+filename_lower+(special.empty()?"":"_"+special)+".txt"));
    file_position=0;

    std::string line, talking="No one", emotion="normal";
    while (!file.eof())
    {
        getline(file,line);
        //std::cout << line << "\n";

        //skip to the end of the choices
        if (line[0] == '}')
        {
            go_to_pos(file,return_positions.top());
            return_positions.pop();
            continue;
        }

        //checks for commands in []
        int i;
        if ((i = line.find('[')) != line.npos)
        {
            int u = line.find(']',i);
            std::string command = line.substr(i+1,u-i-1);

            //commands with numbers
            if (isdigit(command[0]) || command[0] == '-')
            {
                int num = atoi(command.c_str());
                command.erase(0,std::to_string(num).size()+1);

                //A choice
                if (command == "choices")
                {
                    std::deque<std::string> choices;
                    std::deque<std::streampos> choice_targets;

                    for (int a=0;a<num;a++)
                    {
                        choices.push_back("");
                        while (line[0] != '{')
                        {
                            getline(file,line);
                            if (line[0] != '{')choices[a] += (choices[a].empty()?"":"\n  ")+line;
                        }
                        choice_targets.push_back(file_position);
                        std::cout << file_position<<";";

                        int opening_braces=1;
                        while (opening_braces)
                        {
                            getline(file,line);

                            if (line[0] == '}') opening_braces--;
                            else if (line[0] == '{') opening_braces++;
                        }
                    }
                    return_positions.push(file_position);

                    go_to_pos(file,choice_targets[choice(choices,filename+"_"+emotion,bg)]);
                    continue;
                }
                //A change in affection-points
                else if (command.empty())
                {
                    char c = filename[0];
                    if (affection.count(c)) affection[c] += num;
                    else affection[c] = num;
                }
            }
            else if (command == "break") to_return=true;
            else if (command == "end")
            {
                char c = filename[0];
                return VN_from_file(filename,(affection[c]<=-2?"negative":(affection[c]>=3?"positive":"neutral")));
            }
            //command is just a change of emotion
            else emotion = command;

            line.erase(i,u-i+1);
        }

        //changes the character that is currently speaking
        if (line[1] == ':')
        {
            if (line[0] == 'T' || line[0] == 'U') talking = "Tarkus";
            else talking = filename;
            line.erase(0,2);
        }

        if (!line.empty()) dialog(talking+":\n"+line,filename_lower+"_"+emotion, bg);

        if (to_return) return;
    }

    file.close();
}


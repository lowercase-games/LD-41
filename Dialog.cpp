#include "Dialog.h"

#include <fstream>
#include <random>
#include <time.h>
#include <stack>
#include <iostream>
#include "random"
#include "Rendering.h"
#include "Font.h"
#include "Level.h"
#include "Item.h"
#include "Sound.h"

std::map<char, int> affection;
bool instant_text=false;

#define option_num 5
void menu()
{
    int log_w,log_h;
    SDL_RenderGetLogicalSize(renderer,&log_w,&log_h);
    SDL_RenderSetLogicalSize(renderer,window[0],window[1]);

    std::string text[option_num] = {"Fullscreen","WaifuMode","SFX Volume","Music Volume","Exit Game"};
    int *value[option_num] = {&fullscreen, &easy_mode, &sfx_volume, &music_volume, nullptr};
    int range_max[option_num] = {1,1,128,128,0};
    int range_min[option_num] = {0,0,0,0,0};
    int pointer = 0;

    int w,h;
    SDL_Texture* pointer_tex = load_image("pointer");
    SDL_QueryTexture(pointer_tex,nullptr,nullptr,&w,&h);


    SDL_Event e;
	while (!breakk)
    {
        while(SDL_PollEvent(&e))
        {
			if (e.type == SDL_QUIT) breakk = true;

			else if (e.type == SDL_KEYDOWN)
			{
			    if (e.key.keysym.sym == SDLK_ESCAPE)
			    {
			        if (easy_mode && player->max_hp == 10)
                    {
                        player->max_hp = 30;
                        player->hp += 20;
                    }
                    else if (easy_mode && player->max_hp == 30)
                    {
                        player->max_hp = 10;
                        player->hp -= 20;
                    }

                    SDL_RenderSetLogicalSize(renderer,log_w,log_h);
			        return;
			    }
			    else if (e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d)
                {
                    if (value[pointer])
                    {
                        if (e.key.keysym.sym == SDLK_a) --(*value[pointer]);
                        else                            ++(*value[pointer]);;

                        if (*value[pointer] > range_max[pointer]) *value[pointer] = range_min[pointer];
                        if (*value[pointer] < range_min[pointer]) *value[pointer] = range_max[pointer];

                        render_init_update();
                        sound_init_update();
                    }
                    else if (e.key.keysym.sym == SDLK_e) breakk = true;
                }
			    else if (e.key.keysym.sym == SDLK_w)
                {
                    pointer--;

                    if (pointer<0) pointer = option_num-1;
                }
			    else if (e.key.keysym.sym == SDLK_s)
                {
                    pointer++;

                    if (pointer>=option_num) pointer = 0;
                }
			}
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        for (int i=0; i<option_num;i++)
        {
            render_text(50,50+i*50,text[i]+(value[i]?": "+std::to_string(*value[i]):""),255,255,false,false);
        }

        SDL_Rect r = {65-w,58+pointer*50,w,h};
        SDL_RenderCopy(renderer,pointer_tex,nullptr,&r);

        SDL_RenderPresent(renderer);
        limit_fps();
    }
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

bool movement=true;
int pos_x = -1;
#define direction (pos_x<window[0]*scale/4 ? 1:(pos_x+me_w>window[0]*scale*3/4 ? -1:(random(0,1) ? -1:1)))
void dialog(std::string text, std::string image, SDL_Texture* bg)
{
    int progress = instant_text?text.size():text.find(":")+1, frames_since_progress=0;

    SDL_Texture *me = load_image(image), *textbox = load_image("textbox");
    int me_h, me_w, textbox_h, textbox_w;
    SDL_QueryTexture(me,nullptr,nullptr,&me_w,&me_h);
    SDL_QueryTexture(textbox,nullptr,nullptr,&textbox_w,&textbox_h);

    if (pos_x == -1) pos_x = (window[0]*scale-me_w)/2;

    SDL_Rect me_rect = {pos_x, window[1]*scale-me_h, me_w, me_h};
    SDL_Rect textbox_rect = {window[0]*scale/2-textbox_w/2,window[1]*scale-textbox_h-5,textbox_w, textbox_h};

    add_newlines(text , textbox_rect.w-40);

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
            SDL_RenderCopy(renderer,textbox,nullptr,&textbox_rect);

            render_text(textbox_rect.x+20,textbox_rect.y+10,text.substr(0,progress),255,255, false, false);

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

    #define make_text text.clear();for (int i=0;i<texts.size();i++) text += (cursor==i?"> ":"-  ")+texts[i]+'\n';

    make_text

    SDL_Texture *me = load_image(image), *textbox = load_image("textbox");
    int me_h, me_w, textbox_h, textbox_w;
    SDL_QueryTexture(me,nullptr,nullptr,&me_w,&me_h);
    SDL_QueryTexture(textbox,nullptr,nullptr,&textbox_w,&textbox_h);

    if (pos_x == -1) pos_x = (window[0]*scale-me_w)/2;

    SDL_Rect me_rect = {pos_x, window[1]*scale-me_h, me_w, me_h};
    SDL_Rect textbox_rect = {window[0]*scale/2-textbox_w/2,window[1]*scale-textbox_h-5,textbox_w, textbox_h};

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

        SDL_RenderCopy(renderer,textbox,nullptr,&textbox_rect);

        render_text(textbox_rect.x+20,textbox_rect.y+10,text,255,255, false, false);

        SDL_RenderPresent(renderer);
        limit_fps();
    }

    return 0;
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

SDL_Texture* make_dialog_bg()
{
    SDL_Texture* bg = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,window[0],window[1]);
    SDL_SetRenderTarget(renderer,bg);
    render_bg();
    for (Object* o: objects_render) o->render();

    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,25,25,25,150);
    SDL_Rect r = {0,0,window[0]*scale, window[1]*scale};
    SDL_RenderFillRect(renderer,&r);
    SDL_SetRenderTarget(renderer,nullptr);

    return bg;
}

std::stack<int> return_positions;
void VN_from_file(std::string filename, std::string special) //TODO: friend-end
{
    play_music(load_music("dating"));

    if (!affection.count(filename[0])) affection[filename[0]] = 0;

    bool to_return = false;

    std::string filename_lower = filename;
    std::transform(filename_lower.begin(), filename_lower.end(), filename_lower.begin(), ::tolower);

    SDL_RenderSetLogicalSize(renderer, window[0]*scale, window[1]*scale);
    SDL_Texture* bg = make_dialog_bg();

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
        while ((i = line.find('[')) != line.npos)
        {
            int u = line.find(']',i);
            std::string command = line.substr(i+1,u-i-1);
            //std::cout << command;

            //commands with numbers
            if (isdigit(command[0]) || command[0] == '-' || command[0] == '+')
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
                            if (line[0] != '{')choices[a] += (choices[a].empty()?"":"\n   ")+line;
                        }
                        choice_targets.push_back(file_position);

                        int opening_braces=1;
                        while (opening_braces)
                        {
                            getline(file,line);

                            if (line[0] == '}') opening_braces--;
                            else if (line[0] == '{') opening_braces++;
                        }
                    }
                    return_positions.push(file_position);

                    go_to_pos(file,choice_targets[choice(choices,filename_lower+"_"+emotion,bg)]);
                    line = "";
                    break;
                }
                //A change in affection-points
                else if (command.empty())
                {
                    affection[filename[0]] += num;
                    //std::cout << affection[filename[0]] << " " << num << "\n";
                }
            }
            //End dialog
            else if (command == "break") to_return=true;
            //Play sound
            else if (command.substr(0,5) == "play:")
            {
                std::cout << command.substr(5);
                play_sound(load_sound(command.substr(5)));
            }
            //Trigger ending based on affection points
            else if (command == "end")
            {
                char c = filename[0];
                std::string end_type;
                if (affection[c]<=-2)
                {
                    end_type = "negative";
                    affection[c] = negative_end;
                }
                else if (affection[c]>=3)
                {
                    end_type = "positive";
                    affection[c] = positive_end;
                }
                else
                {
                    end_type = "neutral";
                    affection[c] = neutral_end;
                }

                //std::cout << affection[c] << " " << end_type << "\n";

                return VN_from_file(filename,end_type);
            }
            //Kill the character you are talking to
            else if (command == "kill")
            {
                affection[filename[0]] = dead_end;
                to_return=true;
                if (filename[0] == 'C') play_sound(load_sound("death_chthulu"));
                else if (filename[0] == 'Y') play_sound(load_sound("death_fish"));
            }
            else if (command == "trigger kill cassy quest") collected_items::kill_cassy_quest_token = true;
            else if (command == "abort kill cassy quest") collected_items::kill_cassy_quest_token = false;
            else if (command == "trigger kill ysa quest") collected_items::kill_ysa_quest_token = true;
            else if (command == "trigger kasaobake quest") collected_items::kasaobake_quest_token = true;
            else if (command == "spawn kitsune") collected_items::kitsune_token = true;
            else if (command == "end quest")
            {
                affection[filename[0]] = negative_end;
                to_return=true;
            }
            else if (command == "crash")
            {
                to_return=true;
                breakk=true;
            }
            else if (command == "max revive")
            {
                if (affection['C'] == dead_end) affection['C'] = neutral_end;
                if (affection['Y'] == dead_end) affection['Y'] = neutral_end;
            }
            else if (command == "kitsune end")
            {
                show_screen("kitsune_end");
                to_return=true;
                breakk=true;
            }
            //command is just a change of emotion
            else emotion = command;

            line.erase(i,u-i+1);
        }

        //changes the character that is currently speaking
        if (line[1] == ':')
        {
            if (line[0] == 'T' || line[0] == 'U')
            {
                talking = "Tarkus";
                movement = false;
            }
            else
            {
                talking = (filename=="Kasaobake")?"Kasa-Obake (mysterious)":filename;
                movement = true;
            }
            line.erase(0,2);
        }

        if (!line.empty()) dialog(talking+":\n"+line,filename_lower+"_"+emotion, bg);

        if (to_return)
        {
            file.close();

            play_music(load_music("theme_floor"+std::to_string(level)));
            SDL_RenderSetLogicalSize(renderer, window[0], window[1]);
            return;
        }
    }

    file.close();

    play_music(load_music("theme_floor"+std::to_string(level)));

    SDL_RenderSetLogicalSize(renderer, window[0], window[1]);
}


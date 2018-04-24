#ifndef _DIALOG
#define _DIALOG

#include <string>
#include <map>
#include <SDL.h>

enum Ending
{
    dead_end=-200,negative_end=-100,neutral_end=100, positive_end=200
};

extern std::map<char, int> affection;
extern bool instant_text;

class Player;
extern Player* player;

void random_init();
int random(int x, int y);
void menu();
void dialog(std::string text, std::string image, SDL_Texture* bg);
SDL_Texture* make_dialog_bg();
void VN_from_file(std::string filename, std::string special="");

#endif // _DIALOG

#ifndef _DIALOG
#define _DIALOG

#include <string>
#include <map>

enum Ending
{
    dead_end=-200,negative_end=-100,neutral_end=100, positive_end=200
};

extern std::map<char, int> affection;

void random_init();
int random(int x, int y);
void menu();
void VN_from_file(std::string filename, std::string special="");

#endif // _DIALOG

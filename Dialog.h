#ifndef _DIALOG
#define _DIALOG

#include "string"

void random_init();
int random(int x, int y);
void menu();
void VN_from_file(std::string filename, std::string special="");

#endif // _DIALOG

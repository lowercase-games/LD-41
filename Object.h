#ifndef _OBJECT
#define _OBJECT

#include <deque>
#include <string>
#include <SDL.h>
#include <algorithm>

template<class t> void remove_it(std::deque<t>* base, t thing)
{
    base->erase( std::remove( std::begin(*base), std::end(*base), thing ), std::end(*base) );
}

int sign(int x);

class Object;
extern std::deque<Object*> objects, wallables;

class Object
{
    SDL_Rect* current_hitbox;

protected:
    int pos[2], last_pos[2], size[2];
    SDL_Rect hitbox;
    SDL_Texture* tex;

public:
    Object(int x, int y, std::string s="", int hitx=0, int hity=0, int hitw=-1, int hith=-1);
    virtual ~Object();

    SDL_Rect* cur_hitbox();
    void move(int x, int y, bool relative=true, bool set_last_pos=true);
    virtual void move_back();
    virtual void update() {}
    virtual void render();
};

#endif // _OBJECT

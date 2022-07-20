#include "ILIGAME/ILIGAME.h"
#include "ILIGAME-SDL2.h"

int main(int, char**)
{
    ILIGAME::VM* game = new ILIGAME_SDL2();
    int res = game->run();
    delete game;
    return res;
}

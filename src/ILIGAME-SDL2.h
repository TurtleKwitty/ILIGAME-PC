#include <SDL2/SDL.h>

#include "ILIGAME/ILIGAME.h"

class ILIGAME_SDL2 : public ILIGAME::VM
{
    SDL_Window *window;
    SDL_Surface *windowSurface;
    SDL_Surface *gameSurface;

public:
    ILIGAME_SDL2();
    virtual ~ILIGAME_SDL2();

    int run() override;
    void flip() override;

    Uint32 lastFrameTime = SDL_GetTicks();
    float frameEndTimeError = 0;
    const Uint8 frameLengthMillisecond = 1 * 1000 / FRAME_RATE;
    const float frameLengthDrift = (1 * 1000 / FRAME_RATE) - frameLengthMillisecond;
};
#include <SDL2/SDL.h>

#include <iostream>

#include "ILIGAME-SDL2.h"

ILIGAME_SDL2::ILIGAME_SDL2() : ILIGAME::VM()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw("Could not initialize SDL");
    }
    else
    {
        window = SDL_CreateWindow("ILIGAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH * 4, GAME_HEIGHT * 4, SDL_WINDOW_SHOWN);
        windowSurface = SDL_GetWindowSurface(window);
        gameSurface = SDL_CreateRGBSurfaceWithFormat(0, GAME_WIDTH, GAME_HEIGHT, 24, SDL_PIXELFORMAT_RGB888);
        if (!gameSurface)
        {
            std::cout << "gameSurface not initalized(Error: " << SDL_GetError() << ")" << std::endl;
            throw("Could not initialize gameSurface");
        }
    }
}

ILIGAME_SDL2::~ILIGAME_SDL2()
{
    if (window)
        SDL_DestroyWindow(window);
    if (gameSurface)
        SDL_FreeSurface(gameSurface);

    SDL_Quit();
}

int ILIGAME_SDL2::run()
{
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        while (tickUpdate())
            ;

        while (tickDraw())
            ;

        flip();
    }

    ILIGAME::VM::clearScreen(14);

    for (int i = 0; i < 8; i++)
    {
        ILIGAME::VM::drawPixel(i, i, 15);
    }

    for (int i = 0; i < 8; i++)
    {
        ILIGAME::VM::drawPixel(GAME_WIDTH - i - 1, GAME_HEIGHT - i - 1, 15);
    }
    return 0;
}

void ILIGAME_SDL2::flip()
{
    // Wait til it's time to draw
    int currentTime = SDL_GetTicks();
    frameEndTimeError += frameLengthDrift;
    int drawDelay = (lastFrameTime + frameLengthMillisecond + floor(frameEndTimeError)) - currentTime;
    printf("Frame time: %u\n", currentTime - lastFrameTime);
    if (drawDelay > 0)
    {
        // Only delay if we're ahead
        SDL_Delay(drawDelay);
    }
    lastFrameTime += frameLengthMillisecond;
    if (frameEndTimeError >= 1)
    {
        lastFrameTime += floor(frameEndTimeError);
        frameEndTimeError -= floor(frameEndTimeError);
    }

    // Draw
    Uint32 *pixels = (Uint32 *)gameSurface->pixels;
    for (int y = 0; y < GAME_HEIGHT; y++)
    {
        for (int x = 0; x < GAME_WIDTH; x++)
        {
            Uint32 &pixel = pixels[y * GAME_WIDTH + x];
            ILIGAME::Color color = pallette[x % 2 ? gameScreen[y][x / 2] & 0b00001111 : gameScreen[y][x / 2] >> 4];
            pixel = color.r << 16 | color.g << 8 | color.b;
        }
    }

    if (SDL_BlitScaled(gameSurface, NULL, windowSurface, NULL))
    {
        std::cout << "gameSurface blit error(Error: " << SDL_GetError() << ")" << std::endl;
    }
    SDL_UpdateWindowSurface(window);
}
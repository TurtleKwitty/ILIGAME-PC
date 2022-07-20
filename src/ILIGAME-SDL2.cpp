#include <SDL2/SDL.h>
#include <boost/filesystem.hpp>

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
        window = SDL_CreateWindow("ILIGAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN);
        windowSurface = SDL_GetWindowSurface(window);
        gameSurface = SDL_CreateRGBSurfaceWithFormat(0, GAME_WIDTH, GAME_HEIGHT, 24, SDL_PIXELFORMAT_RGB888);
        if (!gameSurface)
        {
            std::cout << "gameSurface not initalized(Error: " << SDL_GetError() << ")" << std::endl;
            //throw("Could not initialize gameSurface");
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

	while(tickInit())
		;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
        	switch(event.type){
        		case SDL_QUIT:
        			quit = true;
        			break;
        		case SDL_MOUSEMOTION:
        			mousePos.x = event.motion.x;
        			mousePos.y = event.motion.y;
        			break;
        		case SDL_MOUSEBUTTONDOWN:
        		case SDL_MOUSEBUTTONUP:
        			char mouseActionMask = 0;
        			switch(event.button.button){
        				case SDL_BUTTON_LEFT:
        					mouseActionMask = ILIGAME::MouseActions::LEFT_CLICK;
        					break;
        				case SDL_BUTTON_RIGHT:
        					mouseActionMask = ILIGAME::MouseActions::RIGHT_CLICK;
        					break;
        				case SDL_BUTTON_MIDDLE:
        					mouseActionMask = ILIGAME::MouseActions::MIDDLE_CLICK;
        					break;
        			}

        			if(event.button.state == SDL_PRESSED){
        				mouseActions |= mouseActionMask;
        			} else {
        				mouseActions &= ~mouseActionMask;
        			}
       			
        	}
        }

        while (tickUpdate())
            ;

        while (tickDraw())
            ;

        flip();
    }

    while(tickClose())
    	;

    return 0;
}

void ILIGAME_SDL2::flip()
{
    // Wait til it's time to draw
    int currentTime = SDL_GetTicks();
    frameEndTimeError += frameLengthDrift;
    int drawDelay = (lastFrameTime + frameLengthMillisecond + floor(frameEndTimeError)) - currentTime;
    if (frameEndTimeError >= 1)
    {
        frameEndTimeError -= floor(frameEndTimeError);
    }
    //printf("Frame time: %u\t\tDelayed: %u\n\r", currentTime - lastFrameTime, drawDelay>0 ? drawDelay : 0);
    lastFrameTime = currentTime;
    if (drawDelay > 0)
    {
        // Only delay if we're ahead
        SDL_Delay(drawDelay);
        lastFrameTime += drawDelay;
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

    ILIGAME::VM::flip();
}

unsigned int ILIGAME_SDL2::readFile(const char* filePath, char*& fileData){
	SDL_RWops* file = SDL_RWFromFile(filePath, "rb");
	if(file == NULL) {
		std::cout << "Could not open file(Error: " << SDL_GetError() <<" )"<<std::endl;
		return 0;
	} else {
		std::cout <<"File opened" << std::endl;
	}
	
	Sint32 fileSize = file->size(file);
	std::cout << "Reading file with size " << fileSize << std::endl;
	if(fileSize == -1) return 0;
	
	fileData = new char[fileSize];
	file->read(file, fileData, fileSize, 1);
	file->close(file);

	return fileSize;
}

bool ILIGAME_SDL2::writeFile(const char* filePath, void* fileData, unsigned int fileSize){
	boost::filesystem::path path(filePath);
	boost::filesystem::create_directories(path.parent_path());
	SDL_RWops* file = SDL_RWFromFile(filePath, "wb");
		if(file == NULL) {
		std::cout << "Could not open file(Error: " << SDL_GetError() <<" )"<<std::endl;
		return false;
	}

	file->write(file, fileData, fileSize, 1);
	file->close(file);

	return true;
}

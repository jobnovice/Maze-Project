#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>


//screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//the window that we're going to be rendering on
SDL_Window* gWindow;

//the surface contained by the window
SDL_Surface* gScreenSurface;

//the texture(it's just an abstract) that we're going to render
SDL_Texture* loadTexture( std::string path );

bool init();
bool loadMedia();

void close();

bool init()
{
	
	bool success = true;
	// gWindow = SDL_CreatenewSurface
	//Intialize SDL Video
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL couldn't be intialized! SDL ERROR: %s\n", SDL_GetError());	
		success = false;
	}
	else{
		gWindow = SDL_CreateWindow("SDL TUTORIAL!",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL){
			printf("Window couldn't be created! SDL error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			
		}
	}

}
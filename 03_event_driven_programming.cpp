
#include <stdio.h>
#include <SDL2/SDL.h>

// screen dimension to be used

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool LoadMedia();

void close();

SDL_Window *gWindow = NULL;

SDL_Surface *gSurface = NULL;

SDL_Surface *gXOut = NULL;

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gSurface = SDL_GetWindowSurface(gWindow);
			success = true;
		}
	}

	return success;
}

bool LoadMedia()
{
	bool success = true;
	gXOut = SDL_LoadBMP("x.bmp");

	if (gXOut == NULL)
	{
		printf("couldn't Load media SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;	
}

void close()
{
	SDL_FreeSurface(gSurface);
	gXOut = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("not initialized");
	}
	else
	{

		if (!LoadMedia())
		{
			printf("media error");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					SDL_BlitSurface(gXOut, NULL, gSurface, NULL);
					SDL_UpdateWindowSurface(gWindow);
				}
			}
		}
	}
	close();
	return 0;
}
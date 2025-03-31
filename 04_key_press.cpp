#include <stdio.h>

#include <SDL2/SDL.h>
#include <string>

//screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

bool init();

bool loadMedia();

void close();

SDL_Surface* loadSurface( std::string path );

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface *gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL];

SDL_Surface* gCurrentSurface = NULL;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
		if (gWindow == NULL)
		{
			printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
			success = true;
		}
	}
	return success;
}

SDL_Surface* loadSurface( std:: string path)
{
	// load the image at the specified path
	SDL_Surface* loadedsurface = SDL_LoadBMP(path.c_str());
	if (loadedsurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n",path.c_str(), SDL_GetError());
	}
	return 	loadedsurface;
}
void close()
{
	SDL_FreeSurface(gCurrentSurface);
	SDL_FreeSurface(gScreenSurface);
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
		SDL_FreeSurface(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL;
	}
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}
bool loadmedia()
{
	bool  success = true;
	//load default surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Pics/press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		success = false;
	}
	//load the "down" Surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Pics/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Pics/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Pics/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		success = false;
	}


	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Pics/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		success = false;
	}
	return success;
}
int main(int argc, char *args[])
{
	if (!init())
	{
		printf("not initliazed");
	}
	else
	{
		if (!loadmedia())
		{
			printf("some or all media are not loaded");
		}
		bool quit = false;		
		SDL_Event e;
		gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
		while(!quit)
		{
			while(SDL_PollEvent(&e) != 0)
			{
				if(e.type == SDL_QUIT)
				{
					quit = true;
				}
				//user presses a key
				else if(e.type == SDL_KEYDOWN)
				{
					switch(e.key.keysym.sym)
					{
						case SDLK_UP:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
						break;
						
						
						case SDLK_DOWN:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
						break;
						
						
						case SDLK_LEFT:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
						break;
						
						
						case SDLK_RIGHT:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
						break;
						
						default:
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
						break;
					}
				}
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
				SDL_UpdateWindowSurface(gWindow);
			}
		}
		
	}
	

	close();
	return 0;
} 

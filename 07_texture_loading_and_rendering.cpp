#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>

// screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// the window that we're going to be rendering on
SDL_Window *gWindow;

// the renderer for our window
SDL_Renderer *gRenderer = NULL;

// the surface contained by the window
// the texture(it's just an abstract) that we're going to render

SDL_Texture *gTexture = NULL;

SDL_Texture *loadTexture(std::string path);

bool init();
bool loadMedia();

void close();

bool init()
{

	bool success = true;
	// gWindow = SDL_CreatenewSurface
	// Intialize SDL Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL couldn't be intialized! SDL ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL TUTORIAL!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window couldn't be created! SDL error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == NULL)
			{
				printf("SDL_Renderer could not be created SDL_GetError: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// initialise png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) * imgFlags))
				{

					printf("SDL_image could not be intialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia()
{
	bool success = true;

	gTexture = loadTexture("Pics/texture.png");

	if (gTexture == NULL)
	{
		printf("Failed to load texture! \n");
		success = false;
	}

	return success;
}

SDL_Texture *loadTexture(std::string path)
{
	SDL_Texture *newTexture = NULL;
	// load the surface from the path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("could not load the image IMG_Error: %s\n", IMG_GetError());
	}

	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

		if (newTexture == NULL)
		{
			printf("couldn't create Texture SDL_Error: %s\n", SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void close()
{
	// free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	// destroy window renderer
	SDL_DestroyRenderer(gRenderer);

	// destroy window
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("Failed to intialize");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while (!quit)
			{
				while (SDL_PollEvent(&e))
				{

					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
			
			//clear screen
			SDL_RenderClear(gRenderer);

			// render texture to screen
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			// update screen
			SDL_RenderPresent(gRenderer);
			}
			

		}
	}
	close();
	return 0;
}
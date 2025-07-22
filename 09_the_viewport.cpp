#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Texture *gTexture = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Window *gWindow = NULL;

bool init();

void close();

bool loadMedia();

SDL_Texture *loadTexture(std::string path);

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("couldnt intiliaze SDL video Library SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// define the window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("couldn't create the window SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("could not create the renderer SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				// initialize the img library with the PNG format
				int imgFlags = IMG_INIT_PNG;
				if (!IMG_Init(imgFlags) * imgFlags)
				{
					printf("could not initialize the PNG img format SDL_Error: %s\n", IMG_GetError());
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
	

	gTexture  = loadTexture("Pics/viewport.png");

	if (gTexture == NULL)
	{
		printf("could not load the media");
		success = false;
	}

	return success;
}

SDL_Texture *loadTexture(std::string path)
{
	SDL_Texture *temp_text = NULL;

	SDL_Surface *temp_sur = IMG_Load(path.c_str());

	if (temp_sur == NULL)
	{
		printf("could not load the image from the given source");
	}
	else
	{
		temp_text = SDL_CreateTextureFromSurface(gRenderer, temp_sur);
		if (temp_text == NULL)
		{
			printf("could not create the texture from the given source");
		}
	}

	return temp_text;
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("couldn't intialize the dependecies");
	}

	else
	{
		if (!loadMedia())
		{
			printf("couldnot load the media");
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
				//clear the renderer
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				
				//Top Left Viewport
				SDL_Rect topLeftViewport;
				topLeftViewport.x = 0;
				topLeftViewport.y = 0;
				topLeftViewport.w = SCREEN_WIDTH / 2;
				topLeftViewport.h = SCREEN_HEIGHT / 2;

				SDL_RenderSetViewport (gRenderer, &topLeftViewport);

				//render the teture to the screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Top Right Viewport
				SDL_Rect topRightViewport;
				topRightViewport.x = SCREEN_WIDTH / 2;
				topRightViewport.y = 0;
				topRightViewport.w = SCREEN_WIDTH / 2;
				topRightViewport.h = SCREEN_HEIGHT / 2;

				SDL_RenderSetViewport(gRenderer, &topRightViewport);

				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//bottom viewport
				SDL_Rect bottomViewport;
				bottomViewport.x = 0;
				bottomViewport.y = SCREEN_HEIGHT / 2;
				bottomViewport.w = SCREEN_WIDTH;
				bottomViewport.h = SCREEN_HEIGHT / 2;

				SDL_RenderSetViewport(gRenderer, &bottomViewport);

				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//update the screen
				SDL_RenderPresent(gRenderer);
			}

		}
	}
	close();
	return 0;
}
#include <stdio.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// the window renderer
SDL_Renderer *gRenderer = NULL;
SDL_Window *gWindow = NULL;
// bool loadMedia();
bool init();
void close();

void close(){
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gRenderer= NULL;
	gWindow = NULL;

	SDL_Quit();

	
}
bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("could not intialize the SDL_Init_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (gWindow == NULL)
		{
			printf("could not intialize the window SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == NULL)
			{
				printf("renderer could not be created SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}
	return success;
}
int main(int argc, char *args[])
{
	if (!init())
	{
		printf("could not intilaize some of the libraries");
	}
	else
	{
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}
			// clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// render red filled quad
			SDL_Rect fillRect = {
				SCREEN_WIDTH / 4,
				SCREEN_HEIGHT / 4,
				SCREEN_WIDTH / 2,
				SCREEN_HEIGHT / 2};
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0X00, 0X00, 0XFF);
			SDL_RenderFillRect(gRenderer, &fillRect);

			// render green outlined-rec
			SDL_Rect outlineRect = {SCREEN_WIDTH / 6,
									SCREEN_HEIGHT / 6,
									SCREEN_WIDTH * 2 / 3,
									SCREEN_HEIGHT * 2 / 3};

			SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
			SDL_RenderDrawRect(gRenderer, &outlineRect);

			// draw blue horizontal line
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

			// render yellow dotted points in the middle of the screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
			for (int i = 0; i < SCREEN_HEIGHT; i += 4)
			{
				SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
			}

			//update the screen
			SDL_RenderPresent(gRenderer);
		}
	}
	close();
	return 0;
}
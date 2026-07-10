// using SDL, SDL image, SDL_tff, IO, math and strings
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <cmath>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont = NULL;

bool loadMedia();
void close();
bool init();

class LTexture
{
public:
	// constuctor and deconstructor

	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);
	void setAlpha(Uint8 a);
	void setBlendMode(SDL_BlendMode blending);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	bool loadFromRenderedText(std::string texture_text, SDL_Color textColor);

	void render(int x, int y, SDL_Rect *clip = NULL, double angles = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();
	void free();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};

LTexture gTextTexture;

LTexture::LTexture()
{
	// let's initalize the class's constructor
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

void LTexture::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(mTexture, a);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::free()
{
	// frees all the resource used, if there is one
	if (mTexture)
	{
		SDL_DestroyTexture(mTexture);
	}
}
int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::loadFromRenderedText(std::string texture_text, SDL_Color text_color)
{
	// get rid of preexisting texture
	free();

	SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, texture_text.c_str(), text_color);
	if (!textSurface)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
 		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (!mTexture)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// get dimension
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		// get rid of unwanted old surface
		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angles, SDL_Point *center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	if (clip)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// clear the screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angles, center, flip);
}

bool loadMedia()
{
	// load the status
	bool success = true;

	gFont = TTF_OpenFont("Pics/lazy.ttf", 16);

	if (!gFont)
	{
		printf("could not laod the font, SDL_Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		// render text
		SDL_Color text_color = {0, 0, 0, 0};
		if (!gTextTexture.loadFromRenderedText("the quick brown fox jumps over the lazy dog", text_color))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}
	return success;
}

void close()
{
	// free loaded images
	gTextTexture.free();

	// free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	// destroy window
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	// quit SDL sub systems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("The video Library wasn't set up properly, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// if(!SDL_SetHintWithPriority("SDL_LINEAR_Texture_Rendering", 1, true))
		// {
		// 	printf("")
		// }
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!gWindow)
		{
			printf("window wasn't created successfully, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, 1, SDL_RENDERER_ACCELERATED);
			if (!gRenderer)
			{
				printf("the renderer wasn't created successfully, SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("Image library not properly ");
					success = false;
				}
				// initalzie SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("TTF wasn't inialzied properly, SDL_ttf_Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("All or some libraries weren't initalized ");
	}
	else
	{

		if (!loadMedia())
	
		{
			printf("there is some part of it that wasn't rightfully executed");
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
				}
	
				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
	
				//render current frame
				gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) /2, ( SCREEN_HEIGHT - gTextTexture.getHeight()) /2);
	
				SDL_RenderPresent(gRenderer);
			}
		}
		
	}
	close();
		
	return 0;
}
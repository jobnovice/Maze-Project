#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool loadMedia();
void close();
bool init();

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

class LTexture
{
	public:
		LTexture();
		~LTexture();

		void render(int x, int y, SDL_Rect* clip=NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		void setBlend(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		bool loadFromSource(std::string path);

		int mWidth();
		int mHeight();
		void free();


	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;

};
LTexture gFlip;

bool LTexture::loadFromSource(std::string path)
{
	bool success = false;
	SDL_Texture* tempText = NULL;

	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (!tempSurface)
	{
		printf("Couldn't get the image SDL_Error: %s\n", IMG_GetError());
		success = false;
	}
	else
	{
		//let's load to the texture , so that the GPU could handle it using the Renederer(relatively faster since it's on the gpu)
		tempText = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
		if(!tempText)
		{
			printf("couldn't load the image surface on to the texture, SDL_Error: %s\n",SDL_GetError());
			success = false;

		}
		else
		{

			SDL_FreeSurface(tempSurface);
			mTexture = tempText;
		}

	}
	return success;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	if(clip)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}


void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);

}

void LTexture::setBlend(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}


bool loadMedia()
{
	bool success = true;

	if(!gFlip.loadFromSource("Pics/flipTexture.png"))
	{
		printf("The Image couldnot load the image");
		success = false;

	}

	// else
	// {

	// }
}


bool init()
{
	bool success = true;

	if(!SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("The video library wasn't set properly, SDL_Error: %s\n",SDL_GetError());
		success = false;

	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(!gWindow)
		{
			printf("Could not create the Winodw, SDL_Error: %s\n", SDL_GetError());
			success  = false;

		}

		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, 1,	SDL_RENDERER_ACCELERATED);
			if(!gRenderer)
			{
				printf("could not initalize the renderer, SDL_Error: %s\n", SDL_GetError());
				success = false;
			}

			int imgFlags = IMG_INIT_PNG;

			if(!IMG_Init(imgFlags) & imgFlags)
			{
				printf("Could not initalize the image library properly, IMG_error: %s\n");
				success = false;
			}

		}
	}
	return success;
}

void close()
{
	//close all libraries and resources utiliazed
	gFlip.free();

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_Quit();
	IMG_Quit();
	

}
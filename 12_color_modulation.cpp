#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow;
SDL_Renderer *gRenderer;

bool init();
bool loadMedia();

class LTexture
{
	// constuctor and deconstructor
public:
	LTexture();

	~LTexture();

	bool loadFromSource(std::string path);

	void render(int x, int y, SDL_Rect *clip);

	int getWidth();

	int getHeight();

	void free();

private:
	int mWidth;
	int mHeight;

	SDL_Texture *mTexture;
};

LTexture::LTexture()
{
	mWidth = 0;
	mHeight = 0;
	mTexture = NULL;
}

LTexture::~LTexture()
{

	free();
}

void LTexture::free()
{
	if (mTexture)
	{
		SDL_DestroyTexture(mTexture);
	}
	mWidth = 0;
	mHeight = 0;
}

bool LTexture::loadFromSource(std::string path)
{
	bool success = true;
	SDL_Surface *image_me = IMG_Load(path.c_str());

	if (!image_me)
	{
		printf("Couldnot load the image: %s, SDL_Error: %s\n", path, IMG_GetError());
		success = false;
	}
	else
	{
		SDL_Texture *tempo = SDL_CreateTextureFromSurface(gRenderer, image_me);

		if (!tempo)
		{
			printf("cannot create Texture from the surface SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			mTexture = tempo;
		}
	}

	return success;
}

LTexture *newFund;

bool loadMedia()
{
	// we're going to be calling our trxture
	//..media loading functionlaity here

	bool success = true;

	if (!newFund->loadFromSource("12-"))
	{
		printf("Oops something got ");
		success = false;
	}
	// else
	// {

	// }
}

bool init()
{
	// allright let's initialize one by one

	bool success = false;

	if (!SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("could not initalize the video sub-system, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	else
	{
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("warning Linear Texure filtering not allowed");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!gWindow)
		{
			printf("couldnot create the window, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (!gRenderer)
		{
			printf("Renderer not intialized, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			
			int imgFlags;
			if (!IMG_Init(imgFlags) & imgFlags)
			{
				printf("coulnot initliaze the image library, SDL_Error:%s\n", IMG_GetError());
				success = false;
			}
		}
	}

	return success;
}

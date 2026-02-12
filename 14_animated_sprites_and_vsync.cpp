#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];

bool init();
bool loadMedia();
void close();

SDL_Renderer *gRenderer;
SDL_Window *gWindow;

class LTexture
{

public:
	// constructor  and destructor

	LTexture();
	~LTexture();

	bool loadFromSurface(std::string path);

	void setBlendMode(SDL_BlendMode blending);

	void render(int x, int y, SDL_Rect *clip = NULL);

	void setAlpha(Uint8 alpha);

	void free();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};
LTexture gAnimated;

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::render(int x, int y, SDL_Rect *clip)
{
	SDL_Rect tempRec = {x, y, mWidth, mHeight};
	if (clip)
	{
		tempRec.w = clip->w;
		tempRec.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &tempRec);
}

LTexture::LTexture()
{
	// constructor
	mTexture = NULL;

	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	// deconstrcut

	SDL_DestroyTexture(mTexture);
}

void LTexture::free()
{
	// free all the resources used
	if (mTexture)
	{
		SDL_DestroyTexture(mTexture);
	}
	mWidth = 0;
	mHeight = 0;
}

bool LTexture::loadFromSurface(std::string path)
{
	bool success = true;

	// create a surface first
	SDL_Texture *wellAnotherOne;
	SDL_Surface *tempOne = IMG_Load(path.c_str());
	if (!tempOne)
	{
		printf("could not create surface from the given image SDL_Error:%s\n", IMG_GetError());
		success = false;
	}
	else
	{
		SDL_SetColorKey(tempOne, SDL_TRUE, SDL_MapRGB(tempOne->format, 0, 0xFF, 0xFF));
		wellAnotherOne = SDL_CreateTextureFromSurface(gRenderer, tempOne);

		if (!wellAnotherOne)
		{
			printf("couldn't create the texture form the surface SDL_Error:%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			mTexture = wellAnotherOne;
			SDL_FreeSurface(tempOne);
		}
	}
	return success;
}

bool loadMedia()
{
	bool success = true;

	if (!gAnimated.loadFromSurface("Pics/foo.png"))
	{
		printf("Could not load the Pics from the source");
		success = false;
	}

	else
	{
		// set sprite clips
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 64;
		gSpriteClips[0].h = 205;

		gSpriteClips[1].x = 64;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 64;
		gSpriteClips[1].h = 205;

		gSpriteClips[2].x = 128;
		gSpriteClips[2].y = 0;
		gSpriteClips[2].w = 64;
		gSpriteClips[2].h = 205;

		gSpriteClips[3].x = 192;
		gSpriteClips[3].y = 0;
		gSpriteClips[3].w = 64;
		gSpriteClips[3].h = 205;
	}

	return success;
}

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("the video libray of SDL is not intilized properly SDL_Error:%s\n", SDL_GetError());
		success = false;
	}

	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear Texture filtering not enabled");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (!gWindow)
		{
			printf("Window couldn't be created SDL_Error: %s\n", SDL_GetError());
			success = false;
		}

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (!gRenderer)
		{
			printf("Renderer could not be created as wanted, SDL_Error:%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			int imgFlags = IMG_INIT_PNG;
			if (!IMG_Init(imgFlags) & imgFlags)
			{
				printf("could not initalize the IMG library SDL_Error:%s\n", IMG_GetError());
				success = false;
			}
		}
	}
	return success;
}

void close()
{
	gAnimated.free();
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);

	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("all or some libraries weren't set");
	}
	else
	{
		if (!loadMedia())
		{
			printf("could not load the media,");
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			// current frame
			int frame = 0;
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

				SDL_Rect* currentclip = &gSpriteClips[frame / 4 ];
				gAnimated.render((SCREEN_WIDTH - currentclip->w)/2, (SCREEN_HEIGHT - currentclip->h)/2, currentclip);

				//update screen
				SDL_RenderPresent(gRenderer);
				
				//increment the frame
				++frame;

				if(frame / 4 >= WALKING_ANIMATION_FRAMES)
				{
					frame = 0;
				}


			}
		}
	}
	//free resource
	close();

	return 0;
}

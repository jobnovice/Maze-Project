#include <iostream>
#include <SDL2/SDL.h>

#include <SDL_image.h>
#include <string.h>

const int SCREEN_WIDTH = 640;

const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow;
SDL_Renderer *gRenderer;

bool init();

bool loadMedia();
void close();
class LTexture
{
	// constructor and deconstuctor

public:
	LTexture();

	~LTexture();

	void render(int x, int y, SDL_Rect *clip = NULL);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	bool loadFromSource(std::string path);

	void free();
	int getWidth();

	int getHeight();

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
	// decontstuctor
	SDL_DestroyTexture(mTexture);
}

LTexture gModulatedTexture;
LTexture gBackgroundTexture;

bool LTexture::loadFromSource(std::string path)
{
	bool success = true;
	SDL_Surface *tempSpace = IMG_Load(path.c_str());
	SDL_Texture *tempText = NULL;
	if (tempSpace == NULL)
	{
		printf("couldn't load the image from the path: %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
		success = false;
	}
	else
	{
		SDL_SetColorKey(tempSpace, SDL_TRUE, SDL_MapRGB(tempSpace->format, 0, 0xFF, 0xFF));

		tempText = SDL_CreateTextureFromSurface(gRenderer, tempSpace);

		if (tempText == NULL)
		{
			printf("the texture could not be created from the given surface, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// ensure dimensions are set
			mWidth = tempSpace->w;
			mHeight = tempSpace->h;
		}
		SDL_FreeSurface(tempSpace);
	}
	mTexture = tempText;
	return success;
}

bool loadMedia()
{
	bool success = true;
	if (!gModulatedTexture.loadFromSource("Pics/fadeout.png"))
	{
		printf("Couldn't load the ");
		success = false;
	}
	else
	{
		gModulatedTexture.setBlendMode(SDL_BLENDMODE_BLEND);
	}
	// load background texture
	if (!gBackgroundTexture.loadFromSource("Pics/fadein.png"))
	{
		printf("couldnot load the background texture");
		success = false;
	}
	return success;
}
void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect *clip)
{

	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// draw it to the backbuffer
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

void LTexture::free()
{
	// free the libraries
	if (mTexture)
	{

		SDL_DestroyTexture(mTexture);
	}

	mWidth = 0;
	mHeight = 0;
}
void close()
{
	gBackgroundTexture.free();
	gModulatedTexture.free();

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);

	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
	IMG_Quit();
}

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Video was not properly set, SDL_Error: %s\n", SDL_GetError());
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
			printf("window was not created successfully, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

		if (!gRenderer)
		{
			printf("Rendere not intilaized , SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("could not initalize the image Library, SDL_Error: %s\n", IMG_GetError());
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
		printf("required libraries weren't initialized properly");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Could not load the image");
		}

		else
		{
			bool quit = false;

			SDL_Event e;
			Uint8 a = 255;
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						// increase alpha on w
						if (e.key.keysym.sym == SDLK_w)
						{
							// cap if over 255
							if (a + 32 > 255)
							{
								a = 255;
							}
							else
							{
								a += 32;
							}
						}
						// decrease alpha on s
						else if (e.key.keysym.sym == SDLK_s)
						{
							// cap on the lowest value the alpha could be set
							if (a - 32 < 0)
							{
								a = 0;
							}
							else
							{
								a -= 32;
							}
						}
					}
				}

				// clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				// Render background
				gBackgroundTexture.render(0, 0);

				// render front blended
				gModulatedTexture.setAlpha(a);
				gModulatedTexture.render(0, 0);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();

	return 0;
}
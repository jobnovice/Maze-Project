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

void close();
class LTexture
{
	// constuctor and deconstructor
public:
	LTexture();

	~LTexture();

	bool loadFromSource(std::string path);

	void render(int x, int y, SDL_Rect *clip = NULL);

	int getWidth();

	int getHeight();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
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
		mTexture = NULL;
	}
	mWidth = 0;
	mHeight = 0;
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
bool LTexture::loadFromSource(std::string path)
{
	bool success = true;
	SDL_Surface *image_me = IMG_Load(path.c_str());

	if (!image_me)
	{
		printf("Couldnot load the image: %s, SDL_Error: %s\n", path.c_str(), IMG_GetError());
		success = false;
	}
	else
	{
		// color key image
		SDL_SetColorKey(image_me, SDL_TRUE, SDL_MapRGB(image_me->format, 0, 0xFF, 0xFF));
		SDL_Texture *tempo = SDL_CreateTextureFromSurface(gRenderer, image_me);

		if (!tempo)
		{
			printf("cannot create Texture from the surface SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			mWidth = image_me->w;
			mHeight = image_me->h;
			mTexture = tempo;
		}
	}

	return success;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	// Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
LTexture newFund;

bool loadMedia()
{
	// we're going to be calling our trxture
	//..media loading functionlaity here

	bool success = true;

	if (!newFund.loadFromSource("Pics/colors.png"))
	{
		printf("Oops something got ");
		success = false;
	}
	return success;
}

void close()
{
	newFund.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
bool init()
{
	// allright let's initialize one by one

	bool success = true;

	if (!SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("could not initalize the video sub-system, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
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

int main(int argc, char *args[])
{
	if (!init())
	{
		printf("Oops some Library wasn't initalised properly");
	}

	else
	{
		if (!loadMedia())
		{
			printf("the media could not load the way we thought it would");
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

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
						switch (e.key.keysym.sym)
						{
						case SDLK_q:
							r += 32;
							break;

						case SDLK_w:
							g += 32;
							break;

						case SDLK_e:
							b += 32;
							break;

						case SDLK_a:
							r -= 32;
							break;

						case SDLK_s:
							g -= 32;
							break;

						case SDLK_d:
							b -= 32;
							break;
						}
					}
				}
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				newFund.setColor(r, g, b);
				newFund.render(0, 0);

				// update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}
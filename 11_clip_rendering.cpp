#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <SDL_image.h>

SDL_Window *gWindow;

SDL_Renderer *gRenderer;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public:
	// constructor and deconstructor
	LTexture();

	~LTexture();

	// loads image from a specfic path
	bool loadFromFile(std::string path);

	// deallocates texture
	void free();

	void render(int x, int y, SDL_Rect *clip = NULL);

	int getWidth();
	int getHeight();

private:
	// the actual hardware texture
	SDL_Texture *mTexture;

	int mWidth;
	int mHeight;
};

SDL_Rect gSpriteClips[4];

LTexture gSpriteSheetTexture;

LTexture::LTexture()
{
	// intilize the variables
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	// deallocate memory
	free();
}

int LTexture::getWidth()
{
	return mWidth;
}
int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::render(int x, int y, SDL_Rect *clip)
{
	// set space and then render
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

bool LTexture::loadFromFile(std::string path)
{
	bool success = true;

	SDL_Texture *newOne;

	SDL_Surface *actuImage = IMG_Load(path.c_str());
	if (actuImage == NULL)
	{
		printf("SDL Error: can not load the image from:%s,IMG_Error:%s\n", path.c_str(), IMG_GetError());
		success = false;
	}

	else
	{
		// color key image(the surface)
		SDL_SetColorKey(actuImage, SDL_TRUE, SDL_MapRGB(actuImage->format, 0, 0xFF, 0xFF));

		newOne = SDL_CreateTextureFromSurface(gRenderer, actuImage);
		if (newOne == NULL)
		{
			printf("Failed to create texture from the surface SDL_error:%s\n", SDL_GetError());
			success = false;
		}
		mWidth = actuImage->w;
		mHeight = actuImage->h;
		mTexture = newOne;
	}

	return success;
}

void LTexture::free()

{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool loadMedia();
bool init();
void close();

bool loadMedia()
{
	// loads the media
	bool success = true;
	if (!gSpriteSheetTexture.loadFromFile("Pics/dots.png"))
	{
		printf("Failed to load the texture(image ) from the specfied file");
		success = false;
	}
	else
	{
		// top left sprite
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 100;
		gSpriteClips[0].h = 100;

		// top right
		gSpriteClips[1].x = 100;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 100;
		gSpriteClips[1].h = 100;

		// bottom left
		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = 100;
		gSpriteClips[2].w = 100;
		gSpriteClips[2].h = 100;

		// bottom right
		gSpriteClips[3].x = 100;
		gSpriteClips[3].y = 100;
		gSpriteClips[3].w = 100;
		gSpriteClips[3].h = 100;
	}

	return success;
}

void close()
{
	gSpriteSheetTexture.~LTexture();

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
}

bool init()
{
	bool success = true;
	if (!SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("failed to initialize the Video library, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("warning: Linear texture filtering not enabled");
		}

		gWindow = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("couldn't create the window, SDL_ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("couldn't define the renderer SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// initialize PNG loading, and set Render color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("falied to initalize the img Library IMG_Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

int main(int argc, char* args[])
{

	if (!init())
	{
		printf("The sub systems aren't intialized properly");
	}
	else
	{
		// load media
		if (!loadMedia())
		{
			printf("Failed to load media! \n");
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

				// clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);

				gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

				gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

				gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

				//update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//close
	close();
	return 0;
}
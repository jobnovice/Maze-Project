#include <SDL2/SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow;

SDL_Renderer *gRenderer;

bool loadMedia();

bool init();
void close();

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);
	#if defined(SDL_TTF_MAJOR_VERSION)
	//creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color text_Color);
	#endif

	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 a);

	void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void free();

	int getWidth();
	int getHeight();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};

LTexture::LTexture()
{
	// constrcutor for the class
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
LTexture::~LTexture()
{
	// deconstruct
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

void LTexture::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(mTexture, a);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//set texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

bool LTexture::loadFromFile(std::string path)
{
	//load the status first
	bool success = true;
	SDL_Texture* tempText = NULL;

	SDL_Surface* tempOne = IMG_Load(path.c_str());
	if(!tempOne)
	{
		printf("Couldn't load the image to surface, SDL_Error: %s\n", IMG_GetError());
		success = false;
	}
	else
	{
		//Color key image
		SDL_SetColorKey(tempOne, SDL_TRUE, SDL_MapRGB(tempOne->format, 0, 0xFF, 0XFF));
		tempText = SDL_CreateTextureFromSurface(gRenderer, tempOne);
		
		if(!tempText)
		{
			printf("Unable to create texture from surface, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get immage dimensions
			mWidth = tempOne->w;
			mHeight = tempOne->h;

		}

		//Ger rid of all surface
		
		mTexture = tempText;



	}
	return success;

}
void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	if(clip)
	{
		renderQuad.w =clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

LTexture gUpTexture;
LTexture gDownTexture;
LTexture gLeftTexture;
LTexture gRightTexture;
LTexture gPressTexture;



bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("couldn't initalize the video library the right way, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!gWindow)
		{
			printf("The window wasn't created properly, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}

		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (!gRenderer)
			{
				printf("the Renderer wasn't created successfully, SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("Couldn't initalize the Image Library properly, SDL_Error: %s\n", SDL_GetError());
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

	if (!gUpTexture.loadFromFile("Pics/up.png"))
	{
		printf("Couldn't load the particular image at this path, Pics/gUp.png");
		success = false;
	}
	if (!gDownTexture.loadFromFile("Pics/down.png"))
	{
		printf("Failed to load Down image");
		success = false;
	}
	if (!gPressTexture.loadFromFile("Pics/press.png"))
	{
		printf("Failed to load press.png");
		success = false;
	}
	if (!gLeftTexture.loadFromFile("Pics/left.png"))
	{
		printf("Failed to load left image");
		success = false;
	}
	if (!gRightTexture.loadFromFile("Pics/right.png"))
	{
		printf("Failed to load Right image");
		success = false;
	}

	return success;
}
void close()
{
	//deconstruct and let go of the
	gRightTexture.free();
	gLeftTexture.free();
	gDownTexture.free();
	gPressTexture.free();
	gUpTexture.free();

	//free global resources
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);

	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char *args[])
{
	if (!init())
	{
		printf("could not initalize some ");
	}
	else
	{
		if (!loadMedia())
		{
			printf("The media wasn' loaded");
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			LTexture *currentTexture = NULL;

			while (!quit)
			{

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				// set texture
				const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					currentTexture = &gUpTexture;
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					currentTexture = &gDownTexture;
				}
				else if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					currentTexture = &gLeftTexture;
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					currentTexture = &gRightTexture;
				}
				else
				{
					currentTexture = &gPressTexture;
				}

				//clear
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//render current texture
				currentTexture->render(0,0);
				
				SDL_RenderPresent(gRenderer);


			}
		}
	}

	close();

	return 0;
}
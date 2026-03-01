#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

void close();

bool loadMedia();
bool init();

class LTexture
{
public:
	// constuctor and deconstructor
	LTexture();
	~LTexture();
	// free the resources
	void free();

	bool loadFromFile(std::string path);

	// set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setAlpha(Uint8 alpha);

	void setBlendMode(SDL_BlendMode blending);

	void render(int x, int y, SDL_Rect *Clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};
LTexture gArrowTexture;

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

LTexture::LTexture()
{
	mWidth = 0;
	mHeight = 0;
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
int LTexture::getHeight()
{
	return mHeight;
}

int LTexture::getWidth()
{
	return mWidth;
}

bool LTexture::loadFromFile(std::string path)
{
	bool success = true;
	SDL_Surface *new_one = IMG_Load(path.c_str());
	SDL_Texture *temp_Text = NULL;

	if (!new_one)
	{
		printf("could not load the image onto the surface, SDL_Error: %s\n", IMG_GetError());
		success = false;
	}

	else
	{
		SDL_SetColorKey(new_one, SDL_TRUE, SDL_MapRGB(new_one->format, 0, 0xFF, 0xFF));
		temp_Text = SDL_CreateTextureFromSurface(gRenderer, new_one);

		if (!temp_Text)
		{
			printf("could not create the texture from surface, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			mWidth = new_one -> w;
			mHeight = new_one -> h;
		}
			SDL_FreeSurface(new_one);

	}
	
	mTexture = temp_Text;

	return success;
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
	SDL_Rect Render_Quad = {x, y, mWidth, mHeight};
	if (clip != NULL)
	{
		Render_Quad.w = clip->w;
		Render_Quad.h = clip->h;
	}

	//clear the screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &Render_Quad, angle, center, flip);
}

bool loadMedia()
{
	bool success = true;

	if (!gArrowTexture.loadFromFile("Pics/arrow.png"))
	{
		printf("Could not the resource properly.");
		success = false;

	}
	return success;

}

bool init()
{
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("The video library wasn't initalized properly, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear Texture filtering not enabled");
		}
		gWindow = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(!gWindow)
		{
			printf("The window was not created successfully, SDL_Error:%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(!gRenderer)
			{
				printf("SDL renderer was not created successfully, SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(IMG_Init(imgFlags) != imgFlags)
				{
					printf("the image library wasn't initalized properly, SDL_Error:%s\n", SDL_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

void close()
{
	gArrowTexture.free();
	
	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);


}

int main(int argc, char *args[])
{
	if(!init())
	{
		printf("all or some libraries wasn't initalized properly");
		return 1;
	}
	else
	{
		if(!loadMedia())
		{
			printf("Failed to load media");

		}
		else
		{
			bool quit = false;
			SDL_Event e;
			double degrees = 0.0;
			SDL_RendererFlip flipType = SDL_FLIP_NONE;
			while(!quit)
			{
				while(SDL_PollEvent(&e) != 0)
				{
					//user requests to quit
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if(e.type == SDL_KEYDOWN)
					{
						switch(e.key.keysym.sym)
						{
							case SDLK_a:
							degrees -= 60;
							break;
							
							case SDLK_d:
							degrees += 60;
							break;

							case SDLK_q:
							flipType = SDL_FLIP_HORIZONTAL;
							break;

							case SDLK_w:
							flipType = SDL_FLIP_NONE;
							break;

							case SDLK_e:
							flipType = SDL_FLIP_VERTICAL;
							break;
						}
					}

				}
				//now what is left is probably going to be rendering it to the gRenderer(which is the paper according the explaination we got from The Agent, what's going to be rendered is defined on the texture -> mTexture(which is like a stamp ), or clips of it (if the full Texturenot needed to be rendered ))
				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				gArrowTexture.render((SCREEN_WIDTH - gArrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - gArrowTexture.getHeight()) /2 , NULL, degrees, NULL,  flipType );

				//render to the screen
				SDL_RenderPresent( gRenderer );
			}
		}
		
	}

	close();

	return 0;
}
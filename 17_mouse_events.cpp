#include <SDL2/SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

TTF_Font* gFont;
SDL_Window* gWindow;
SDL_Renderer* gRenderer;

bool init();
bool loadMedia();

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void setAlpha(Uint8 a);

	void setBlendMode(SDL_BlendMode blending);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	#if defined(SDL_TTF_MAJOR_VERSION)
	bool loadFromRenderedText(std::string texture_text, SDL_Color text_color);
	#endif

	void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center=NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void free();

	int getWidth();
	int getHeight();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};


class LButton
{
	public:
		//initalizes internal variables
		LButton();

		//Sets top left position
		void setPosition(int x, int y);

		//handles mouse event
		void handleEvent(SDL_Event* e);

		//show button sprite
		void render();
	private:
		//Top left position
		SDL_Point mPosition;

		//currently used global sprite
		LButtonSprite mCurrentSprite;

};

LTexture gButtonSpriteSheetTexture;
SDL_Rect gSpriteClips[4];

LButton gButtons[ TOTAL_BUTTONS ]; 

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string text_texture, SDL_Color text_color)
{
	free();

	SDL_Surface* text_surface = TTF_RenderText_Solid(gFont, text_texture.c_str(), text_color);


	if(!text_surface)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());

	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer,text_surface);
		if(!mTexture)
		{
			printf("Unable to create the rendering context from the given Surface, SDL_Error: %s\n", SDL_GetError());
			
		}
		else
		{
			mWidth = text_surface->w;
			mHeight = text_surface->h;

		}
		SDL_FreeSurface(text_surface);
	}
	return mTexture != NULL;
}
#endif
void LTexture::free()
{
	//free the resources used
	if(mTexture)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(mTexture, a);
}
void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}
//two getter methods
int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	bool success = true;
	SDL_Texture* temp_text = NULL;
	SDL_Surface* temp_surf = IMG_Load(path.c_str());
	if(!temp_surf)
	{
		printf("Could not load the image library, SDL_Error: %s\n", IMG_GetError());
		success = false;

	}
	else
	{
		SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 0, 0xFF, 0xFF));
		temp_text  = SDL_CreateTextureFromSurface(gRenderer, temp_surf);
		if(!temp_text)
		{
			printf("Couldn't create the texture from the surface, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			mTexture = temp_text;
			mWidth = temp_surf->w;
			mHeight = temp_surf->h;
		}
		SDL_FreeSurface(temp_surf);
	}

	return success;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center , SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	if(clip)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LButton::handleEvent(SDL_Event* e)
{
	//IF mouse event happened
	if(e->type ==SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get Mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if(x < mPosition.x)
		{
			inside = false;
		}
		else if(x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		else if(y < mPosition.y)
		{
			inside = false;
		}
		else if(y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}


		//check if outside
		if(!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

		}

		//mouse is inside button
		else
		{
			switch( e->type)
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
		
	}
}

void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ]);

}
bool init()
{
	bool success = true;
	//let's initalize the neccessary llibraries
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL video library not initalized properly, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	else
	{
		if(!SDL_SetHint("SDL_LINEAR_TEXTURE_RENDERING", "12"))
		{
			printf("Linear texture rendering not enabled ");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(!gWindow)
		{
			printf("couldn't create the window, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if(!gRenderer)
			{
				printf("The Renderer wasn't successfully initalized, SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) &imgFlags) )
				{
					printf("Image Library isn't initalized properly, SDL_Error: %s\n", IMG_GetError());
					success = false;
				}
				//we actually dont need TTF for this program since we're experimenting with mouse events , by checking where the mouth is at and then rendering differnt parts(sprites of texture) to dispaly 
				// if(!TTF_Init())
				// {
				// 	printf("Couldn't initalize the TTF library properly, SDL_Error: %s\n", TTF_GetError());
				// 	success = false;
				// }

				
			}
		}

	}
	return success;
}

bool loadMedia()
{
	// load the success
	bool success = true;

	if(!gButtonSpriteSheetTexture.loadFromFile("Pics/button.png"))
	{
		printf("Could not load the media with the above files and constraint ");
		success = false;
	}
	else
	{
		//set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}
		gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	return success;
}

void close()
{
	//free all the resources used
	gButtonSpriteSheetTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[])
{
	if(!init())
	{
		printf("all or some libraries were not initalized properly");

	}
	else
	{
		if(!loadMedia())
		{
			printf("Could not load the necessary medias");
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			while(!quit)
			{
				while(SDL_PollEvent(&e) != 0)
				{

					//if user requests to quit
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{
						gButtons[ i ].handleEvent(&e);
					}
				}

				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//render buttons
				for (int i = 0; i < TOTAL_BUTTONS;	++i)
				{
					gButtons[i].render();
				}

				//update screen
				SDL_RenderPresent(gRenderer);

			}
		}
	}

	close();
	return 0;
}
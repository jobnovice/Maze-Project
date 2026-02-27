#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;


bool loadMedia();
bool init();




class LTexture
{
	public:
		//constuctor and deconstructor	
		LTexture();
		~LTexture();
		//free the resources
		void free();

		bool loadFromFile(std::string path);

		//set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		void setAlpha(Uint8 alpha);

		void setBlendMode(SDL_BlendMode mine);

		void render(int x, int y, SDL_Rect* Clip =NULL ,double angle=0.0, SDL_Point* center = NULL, SDL_RendererFlip* flip);

		int getWidth();
		int getHeight();

	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;

		
};

LTexture::LTexture()
{
	mWidth = 0;
	mHeight = 0;

}

LTexture::~LTexture()
{
	SDL_DestroyTexture(mTexture);
}

void LTexture::free()
{
	if(mTexture)
	{
		SDL_DestroyTexture(mTexture);
	
	}
	mWidth = 0;
	mHeight = 0;
}

bool LTexture::loadFromFile(std::string path)
{

}
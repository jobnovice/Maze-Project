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


bool init()
{
	bool success = true;
	if(!SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Video was not properly set, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}


}
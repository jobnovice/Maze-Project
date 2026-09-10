#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <sstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

bool loadMedia();
bool init();
void close();
TTF_Font *gFont = NULL;

class LTexture
{
public:
	// constructor and deconstructor
	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	void render();
	void setAlphaMode();
	void setlBlendMode();

	int getWidth();
	int getHeight();

	void free();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};

// Scene Textures
LTexture gPromptTextTexture;
LTexture gTimeTextTexture;



bool loadMedia()
{
	// loading success flag
	bool success = true;

	// open the font
	gFont = TTF_OpenFont("Pictures/lazy.ttf", 28);
	if (!gFont)
	{
		printf("Well couldn't open the Font file , which is a repository of mathemtical designs and formulas to draw the characters, SDL_Error: %s\n", TTF_GetError());
	}
	else
	{

	}
}

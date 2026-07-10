#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow;
SDL_Renderer *gRenderer;

bool loadMedia();
void close();
bool init();

Mix_Music *gMusic = NULL;
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gLow = NULL;
Mix_Chunk *gMedium = NULL;

class LTexture
{
public:
	// construtor and deconstructor
	LTexture();
	~LTexture();

	int getWidth();
	int getHeight();

	#if defined(SDL_TTF_MAJOR_VERSION)
	//creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textcolor);
	#endif
	bool loadFromFile(std::string path);
	
	void render(int x, int y, SDL_Rect* clip=NULL, double angle = 0.0, SDL_Point* center=NULL, SDL_RendererFlip flip=SDL_FLIP_NONE);
	
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	
	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 a);

	void free();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};
LTexture gPromptTexture;
LTexture::LTexture()
{
	// intialize the texture and what not
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	// what worked may not neccessairly mean it's right for you
	free();
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//set the blend mode
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(mTexture, a);
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//set the color of the texture i think
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//let's render
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	//set clip rendering dimensions
	if(clip)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}
bool LTexture::loadFromFile(std::string path)
{
	bool success = true;
	
	SDL_Surface *tempOne = IMG_Load(path.c_str());

	//the final texture
	SDL_Texture* tempText = NULL;

	if (!tempOne)
	{
		printf("Failed to load the media from the source, SDL_Error:%s\n", IMG_GetError());
		success = false;
	}
	else
	{
		//set the color key first
		SDL_SetColorKey(tempOne, SDL_TRUE, SDL_MapRGB(tempOne->format, 0, 0xFF, 0xFF));
		tempText = SDL_CreateTextureFromSurface(gRenderer, tempOne);

		if(!tempText)
		{
			printf("Failed to create the texture from the surface, SDL_Error:%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			mTexture = tempText;
			mWidth = tempOne->w;
			mHeight = tempOne->h;
		}
		SDL_FreeSurface(tempOne);

	}
	return success;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//get rid of preexisting texture 
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface)
	{
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(!mTexture)
		{
			printf("Unable to create texture from rendered text, SDL_Error:%s\n", SDL_GetError());
			
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//free surfaces
		SDL_FreeSurface(textSurface);

	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());

	}
	return mTexture != NULL;
}
#endif 

void LTexture::free()
{
	//free the resources used during
	if(mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mWidth = 0;
		mHeight = 0;
	}
}

bool loadMedia()
{
	//let's the setup media
	bool success = true;

	if(!gPromptTexture.loadFromFile("Pics/prompt.png"))
	{
		printf("Failed to load the prompt texture!\n");
		success = false;
	}

	//Load music
	gMusic = Mix_LoadMUS("Pics/beat.wav");
	if(!gMusic)
	{
		printf("Failed to load the beat music, SDL_Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV("Pics/scratch.wav");
	if(!gScratch)
	{
		printf("Failed to scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gHigh = Mix_LoadWAV("Pics/high.wav");
	if(!gHigh)
	{
		printf("Failed to load High Frequency sound effect, SDL_mixer_Error:%s\n", Mix_GetError());
		success = false;
	}
	gMedium = Mix_LoadWAV("Pics/medium.wav");
	if(!gMedium)
	{
		printf("Failed to load the medium Freq sound effect, SDL_Mixer_Error:%s\n", Mix_GetError());
		success = false;
	}
	gLow = Mix_LoadWAV("Pics/medium.wav");
	if(!gLow)
	{
		printf("Failed to Load the effect to the low sound frequency, SDL_Mixer_Error:%s\n", Mix_GetError());
		success = false;
	}

	return success;

}

void close()
{
	gPromptTexture.free();

	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gLow);
	Mix_FreeChunk(gMedium);


	gScratch = NULL;
	gLow = NULL;
	gMedium = NULL;
	gHigh = NULL;

	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	//quit SDL Subsystems
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}
bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
	{
		printf("The video or The audio isn't properly initalized: %s\n", SDL_GetError());
		success = false;
	}
	else
	{

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!gWindow)
		{
			printf("The Window wasn't properly initalized and what not, SDL_Error:%s\n", SDL_GetError());
		}
		else
		{
			// create vsynced renderer for winodw
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (!gRenderer)
			{
				printf("The Renderer wasn't created as Planned, SDL_Error:%s\n ", SDL_GetError());
				success = false;
			}
			else
			{
				// Initalized the renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				
				//initalize the image library
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL image could not be initalized!SDL_image error: %s\n", IMG_GetError());
					success = false;
				}
				// let's now intialize the SDL_Mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL mixer couldnot initalize properly, SDL_Error: %s", Mix_GetError());
					success = false;
				}
			}
		}

	}
	return success;
}

int main(int argc, char* args[])
{
	if(!init())
	{
		//the sub systems weren't initalized properly
		printf("the subsystems weren't properly Initalized\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to Load media\n");
		}
		else
		{
			bool quit = false;

			//Event Handler
			SDL_Event e;

			while(!quit)
			{
				while(SDL_PollEvent(&e) != 0)
				{
					//user requests to quit
					if(e.type == SDL_QUIT)
					{
						quit = true;

					}
					//handle key press
					else if(e.type == SDL_KEYDOWN)
					{
						switch(e.key.keysym.sym)
						{
							//play high sound effects
							case SDLK_1:
							Mix_PlayChannel(-1, gHigh, 0);
							break;

							//play medium sound effects
							case SDLK_2:
							Mix_PlayChannel(-1, gMedium, 0);
							break;

							//play low sound effects
							case SDLK_3:
							Mix_PlayChannel(-1, gLow, 0);
							break;

							case SDLK_4:
							Mix_PlayChannel(-1, gScratch, 0);
							break;

							case SDLK_9:
							//if there is no music playing
							if(Mix_PlayingMusic() == 0)
							{
								//play the music
								Mix_PlayMusic(gMusic, -1);
							}
							//If music is being played
							else
							{
								//if the music is paused
								if(Mix_PausedMusic() == 1)
								{
									//resume the music
									Mix_ResumeMusic();
								}
								else
								{
									//pause the music
									Mix_PauseMusic();
								}
							}
							break;

							case SDLK_0:
							//stop the music
							Mix_HaltMusic();
							break;

						}
					}
				}
				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//render prompt
				gPromptTexture.render(0, 0);

				//update screen
				SDL_RenderPresent(gRenderer);

			}
		}
	}
	//free resource
	close();

	return 0;
}
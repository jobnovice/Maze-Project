/* 
This program demonstrates color modulation in SDL2.

Summary:
1. Creates a window and renderer using SDL2
2. Loads an image ("colors.png") as a texture
3. Allows user to modify the RGB values of the texture using keyboard keys:
   - Q/A: Increase/decrease red
   - W/S: Increase/decrease green
   - E/D: Increase/decrease blue
4. The texture's color is modulated in real-time based on these RGB values

Key Components:
- LTexture: A class that handles loading and rendering textures with color modulation
- init(): Initializes SDL2 and creates window/renderer
- loadMedia(): Loads the image file
- main(): Handles the event loop and color adjustments
*/

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
	// Constructor and destructor
public:
	LTexture();
	~LTexture();

	// Core functionality
	bool loadFromSource(std::string path);
	void render(int x, int y, SDL_Rect *clip = NULL);
	void free();

	// Color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	
	// Getters
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
	// Draw texture to the backbuffer
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

bool LTexture::loadFromSource(std::string path)
{
	bool success = true;
	SDL_Surface *image_me = IMG_Load(path.c_str());

	if (!image_me)
	{
		printf("Could not load the image: %s, SDL_Error: %s\n", path.c_str(), IMG_GetError());
		success = false;
	}
	else
	{
		// Make cyan (0, 255, 255) transparent
		SDL_SetColorKey(image_me, SDL_TRUE, SDL_MapRGB(image_me->format, 0, 0xFF, 0xFF));
		SDL_Texture *tempo = SDL_CreateTextureFromSurface(gRenderer, image_me);

		if (!tempo)
		{
			printf("Cannot create texture from the surface SDL_Error: %s\n", SDL_GetError());
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
	// Modulate texture color (tints the texture with specified RGB values)
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

// Global texture to be displayed
LTexture newFund;

bool loadMedia()
{
	bool success = true;

	if (!newFund.loadFromSource("Pics/colors.png"))
	{
		printf("Failed to load colors texture");
		success = false;
	}
	return success;
}

void close()
{
	// Clean up resources
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
	bool success = true;

	if (!SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialize the video sub-system, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled");
		}

		// Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!gWindow)
		{
			printf("Could not create the window, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}

		// Create renderer
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (!gRenderer)
		{
			printf("Renderer not initialized, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			// Initialize SDL_image
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("Could not initialize the image library, SDL_Error: %s\n", IMG_GetError());
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
		printf("Failed to initialize\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			// RGB color values for modulation (initially white)
			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

			while (!quit)
			{
				// Handle events
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						// Adjust RGB values with keyboard
						switch (e.key.keysym.sym)
						{
						case SDLK_q: // Increase red
							r += 32;
							break;
						case SDLK_w: // Increase green
							g += 32;
							break;
						case SDLK_e: // Increase blue
							b += 32;
							break;
						case SDLK_a: // Decrease red
							r -= 32;
							break;
						case SDLK_s: // Decrease green
							g -= 32;
							break;
						case SDLK_d: // Decrease blue
							b -= 32;
							break;
						}
					}
				}
                
				// Clear screen with white
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				// Set color modulation and render texture
				newFund.setColor(r, g, b);
				newFund.render(0, 0);

				// Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
    
	close();
	return 0;
}
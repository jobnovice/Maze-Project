#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window will be rendering to
SDL_Window* gWindow = NULL;

//the Window renderer
SDL_Renderer* gRenderer = NULL;


//texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image from specfied file
		bool loadFromFile( std::string path );

		//Deallocates memory
		void free();
		//renders texture at given point
		void render( int x, int y);
		//gets image dimnsions
		int getWidth();
		int getHeight();

	private:
		//the actual hardware texture
		SDL_Texture* mTexture;

		//image dimensions
		int mWidth;
		int mHeight;


};


//Scene Textures 
LTexture gFooTexture;
LTexture gBackgroundTexture;


LTexture::LTexture()
{
	//Intilaize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std:: string path)
{
	//Get rid of preexisting texture
	free();

	//the final Texture
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadSurface = IMG_Load(path.c_str());

	if (loadSurface == NULL)
	{
		printf("couldnot load the image at the specified path: %s SDL_Error: %s\n", path.c_str(), IMG_GetError());
	
	}

	else
	{
		SDL_SetColorKey( loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface ->format,0, 0XFF,0XFF));
		
		//create the new texture with the color keyed surface
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadSurface);
		
		if (newTexture == NULL)
		{
			printf("could not create the new texture SDL_Error: %s\n",SDL_GetError());

		}
		else
		{
			//Get image dimensions
			mWidth = loadSurface ->w;
			mHeight = loadSurface ->h;
		}
		//Get rid of the old surface
		SDL_FreeSurface(loadSurface);

	}
	mTexture = newTexture;
	return mTexture != NULL;
}


//deallocated the texture if there was something to begin with
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

void LTexture::render(int x, int y)
{
	//Set rendering space and render to the screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight};
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth()
{
	//get method to retreive the width of the texture
	return mWidth;
}

int LTexture::getHeight()
{
	//retrieves the height
	return mHeight;
}


bool loadMedia()
{
	bool success = true;

	if (!gFooTexture.loadFromFile("Pics/foo.png"))
	{
		printf("could not load the gFoo\n");
		success = false;
	}

	if (!gBackgroundTexture.loadFromFile("Pics/background.png"))
	{
		printf("couldn't laod the background image\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gFooTexture.free();
	gBackgroundTexture.free();

	//destroy the window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit the SDL libraries(subsytems)
	IMG_Quit();
	SDL_Quit();
}



bool init()
{
	// initlaize the library
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("couldnot intialize the SDL Video library: SDL_Error: %s\n",SDL_GetError());
		success = false;
	}

	else
	{
		
		//set texture filtering to Linear
		if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("warning: Linear texture filtering not enabled!");
		}


		gWindow = SDL_CreateWindow("SDL_Tutorial",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
		{
			printf("window couldn't be created: SDL_Error: %s\n",SDL_GetError());
			success = false;
		}
		else 
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL)
			{
				printf("couldn't define the rednerer SDL_Error: %s\n",SDL_GetError());
				success = false;
			}
			else
			{
				//intialize the renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF,0xFF);

				//intialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init( imgFlags	) & imgFlags))
				{
					printf("couldn't define the IMG library IMG_Error: %s\n",IMG_GetError());
					success = false;
				}
			}
		}
	}
	
	return success;
}
int main(int argc, char *args[])
{
	if(!init())
	{
		printf("couldn't intilaize all the neccessary libraries for our program\n");
	}
	
	else
	{
		//load meadia
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while(!quit)
			{
				while(SDL_PollEvent(&e))
				{
					if(e.type == quit)
					{
						quit = true;
					}
		
				}

				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				SDL_RenderClear(gRenderer);

				//render the background texture to screen
				gBackgroundTexture.render(0, 0);

				//render the foo texture to the screen
				gFooTexture.render(240, 190);

				//update the screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//free and close resources
	close();
	return 0;
}
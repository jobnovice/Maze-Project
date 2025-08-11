#include <iostream>
#include <SDL2/SDL.h>
#include <string>


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
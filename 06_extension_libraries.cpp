#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>


bool init();


bool init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL couldn't be intialized SDL_Error:");
	}
}
#include "image.h"

Image::Image(SDL_Renderer *renderer, const char *path)
{

	surface = SDL_LoadBMP(path);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

}

void Image::destroy()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);	
}

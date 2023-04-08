#include <SDL2/SDL.h>

class Image
{
	public:
		SDL_Texture *texture;
		SDL_Surface *surface;
		Image(SDL_Renderer *renderer, const char *path);
		void destroy();
};

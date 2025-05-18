#include "AddTexture.h"
#include <SDL2/SDL_image.h>
#include "Coregame.h"
SDL_Texture* AddTexture::addTexture(const char* filename)
{
	SDL_Surface* temp = IMG_Load(filename);
	SDL_Texture* txt = SDL_CreateTextureFromSurface(Coregame::renderer, temp);
	SDL_FreeSurface(temp);
	return txt;
}
void AddTexture::drawTexture(SDL_Texture* tex, SDL_Rect* src)
{
	SDL_RenderCopy(Coregame::renderer, tex, nullptr, src);
	//SDL_RenderCopyEx(Coregame::renderer, tex, &src, &des, NULL, NULL, flip);
}

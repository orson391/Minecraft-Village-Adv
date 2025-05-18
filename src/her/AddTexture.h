#pragma once
#include <SDL2/SDL.h>
class AddTexture
{
public:
	static SDL_Texture* addTexture(const char* filename);
	static void drawTexture(SDL_Texture* tex, SDL_Rect* src);
};


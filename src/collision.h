#pragma once
#include <SDL2/SDL.h>
class collision
{
	public:
		static bool checkCollision(SDL_Rect* a, SDL_Rect* b);
		static bool checkCollision(const SDL_Rect* a, const SDL_Rect* b);
};


#include "collision.h"
#include <SDL2/SDL.h>
#include <stdbool.h>


bool  collision::checkCollision(SDL_Rect* a, SDL_Rect* b) {
//    // If one rectangle is on the left side of the other
//    if (a->x + a->w <= b->x || b->x + b->w <= a->x) {
//        return false;
//    }
//
//    // If one rectangle is above the other
//    if (a->y + a->h <= b->y || b->y + b->h <= a->y) {
//        return false;
//    }
//
//    return true; // Rectangles are colliding
	return SDL_HasIntersection(a, b);
}

bool collision::checkCollision(const SDL_Rect* a, const SDL_Rect* b)
{
	return SDL_HasIntersection(a, b);
}
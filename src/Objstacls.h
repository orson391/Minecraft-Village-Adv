#pragma once
#include <SDL2/SDL.h>
class Objstacls
{
public:

	SDL_Texture *Texture;
	SDL_Rect ObsticleRect;
	SDL_Rect ObsticleHitbox;
	SDL_Rect Shadow;
	int health;
	bool isAlive;
	bool showHealthBar;		   // Controls health bar visibility
	float healthBarTimer;	   // Time to show health bar
	const int maxHealth = 100; // Maximum health
	
};

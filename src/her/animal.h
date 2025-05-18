#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>


enum class animalState {
	Idle,
	Wander,
	Flee
};

class animal
{
public:
	float x, y;
	float ve_x, ve_y;
	animalState state;
	float stateTimer;
	SDL_Texture* skin;
	SDL_Rect rect;

	animal(float startX, float startY);

	//void update(float deltaTime, float playerX, float playerY);
	void update(float deltaTime, float playerX, float playerY, float cam_x, float cam_y);


	void draw(SDL_Renderer* renderer);
	void drawimage();

};


#pragma once

#include <SDL2/SDL.h>
#include "Objstacls.h"
class rock
{
public:
	int x, y;
	static const int tILE_SIZE = 32;
	// int health;
	// bool isAlive;
	// bool showHealthBar;		   // Controls health bar visibility
	// float healthBarTimer;	   // Time to show health bar
	// const int maxHealth = 100; // Maximum health

	void takeDamage(int damage);
	void render(float cam_x, float cam_y);
	void spawnNear(float player_x, float player_y);
	void init();
	Objstacls myobject;
};

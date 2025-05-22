#pragma once
#include <SDL2/SDL.h>
#include "Objstacls.h"
class tree
{
public:
	
	int x, y;
	static const int tILE_SIZE = 32;
	
	void takeDamage(int damage);
	void render(float cam_x, float cam_y);
	void spawnNear(float player_x, float player_y);
	void init();
	Objstacls myobject;
};

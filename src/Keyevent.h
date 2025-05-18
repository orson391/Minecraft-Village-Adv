#pragma once
#include <SDL2/SDL.h>

class Keyevent {
public:
    // Remove the first update method as it's redundant
    void update(float deltaTime); // Keep only this one
};
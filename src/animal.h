#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>



enum class animalState {
	Idle,
	Wander,
	Flee
};

class animal {
public:
    enum class animalState { Idle, Wander, Flee };

    animal(float startX, float startY);
    ~animal();
    void update(float deltaTime, float playerX, float playerY, float cam_x, float cam_y);
    void draw(SDL_Renderer* renderer, float cam_x, float cam_y);
    void takeDamage(int damage);
    void drawimage();

    SDL_Rect hitbox; // World-space hitbox
    int health;

private:
    float x, y; // World position
    float ve_x, ve_y; // Velocity
    animalState state;
    float stateTimer;
    SDL_Texture* skin;
    bool isAlive;
    bool showHealthBar; // Controls health bar visibility
    float healthBarTimer; // Time to show health bar
    const int maxHealth = 100; // Maximum health
};
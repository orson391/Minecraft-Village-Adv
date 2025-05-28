#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

class Player;

enum class enemyState
{
    Idle,
    Pursue,
    Attack,
    Wander
};

class enemy
{
public:
    enemy(float startX, float startY);
    ~enemy();

    void update(float deltaTime, float playerX, float playerY, float camX, float camY, Player &player);
    void draw(SDL_Renderer *renderer, float camX, float camY);
    void takeDamage(int damage,float attackerX, float attackerY);

    SDL_Rect hitbox; // World-space hitbox
    int health;
    bool isAlive;

    float knockbackX = 0.0f;
    float knockbackY = 0.0f;
    float knockbackTimer = 0.0f;
    const float knockbackDuration = 0.2f;   // seconds
    const float knockbackStrength = 200.0f; // pixels per second

private:
    float x, y;       // World position
    float ve_x, ve_y; // Velocity
    enemyState state;
    float stateTimer;     // Timer for state transitions
    float attackCooldown; // Cooldown for attacks

    // Animation properties
    int currentFrame = 0;
    int totalFrames = 8;
    int frameWidth = 32;
    int frameHeight = 32;
    Uint32 frameTime = 120;
    Uint32 lastFrameTime = 0;

    SDL_Texture *walkUp;
    SDL_Texture *walkDown;
    SDL_Texture *walkLeft;
    SDL_Texture *walkRight;
    SDL_Rect rect; // Screen-space rectangle for rendering
    SDL_Rect attackBox;
    bool showHealthBar;        // Controls health bar visibility
    float healthBarTimer;      // Time to show health bar
    const int maxHealth = 100; // Maximum health
    bool attacking = false;
    bool isMoving = false;

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        IDLE
    };
    Direction currentDirection = Direction::IDLE;

    void updateAnimation();
};
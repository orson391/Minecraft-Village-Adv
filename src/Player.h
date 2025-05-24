#pragma once
#include <SDL2/SDL.h>
#include "Keyevent.h"
#include "tree.h"
#include "rock.h"
#include "animal.h"
#include "enemy.h"


class enemy;  // 👈 Forward declaration
class tree;
class rock;
class animal;
class Player {
public:
    Player(int x, int y, int w, int h);
    ~Player();

    SDL_Texture* walkUp;
    SDL_Texture* walkDown;
    SDL_Texture* walkLeft;
    SDL_Texture* walkRight;
    SDL_Rect rect;
    SDL_Rect attackBox;


    int health;
	bool isAlive;
		   // Controls health bar visibility
	
	const int maxHealth = 100; // Maximum health


    static float player_x; // Declaration only
    static float player_y;
    static int currentFrame;
    static bool isMoving;
    enum Direction {
        DIR_UP,
        DIR_DOWN,
        DIR_LEFT,
        DIR_RIGHT,
        DIR_IDLE
    };
    static Direction currentDirection;

    void update(float& cam_x, float& cam_y, tree& mytree ,rock&myrock,animal& myanimal ,enemy &myenemy);
    void updateAnimation();
    void renderCharacter(SDL_Renderer* renderer, float camX, float camY);

    void takeDamage(int damage);

    int totalFrames = 8;
    int frameWidth = 32;
    int frameHeight = 32;
    Uint32 frameTime = 120;
    Uint32 lastFrameTime = 0;


    static bool attacking;
    void attack();



    // Jumping
    static float jumpZ;           // How high the character is "in the air"
    static float jumpVelocity;
    static bool isJumping;

    static constexpr float gravity = 400.0f;
    static constexpr float jumpStrength = -170.0f;


private:
    Keyevent keyevent;
};
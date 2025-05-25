#include "Keyevent.h"
#include "Coregame.h"
#include "Player.h"
#include "mouse.h"

void Keyevent::update(float deltaTime) {
    
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    mouse::HoldLeftClick = false;
    mouse::HoldRightClick = false;
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        mouse::mouseX = mouseX;
        mouse::mouseY = mouseY;
        mouse::HoldLeftClick = true;
        //printf("Left mouse HELD at (%d, %d)\n", mouseX, mouseY);
        // Handle continuous left-click actions
    }

    if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        //printf("Right mouse HELD\n");
        mouse::HoldRightClick = true;
        // Handle continuous right-click actions
    }


    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    const float speed = 70.0f;
    const float dashSpeed = 25.0f;
    float totalspeed = speed;
    Player::isRunning = false;
    Player::isMoving = false;

    if (keys[SDL_SCANCODE_LSHIFT])
    {
        Player::isRunning = true;
        totalspeed += dashSpeed;
    }
    

    if (keys[SDL_SCANCODE_W]) {
        Player::player_y -= totalspeed * deltaTime;
        Player::currentDirection = Player::DIR_UP;
        Player::isMoving = true;
        //printf("W pressed, player_y: %.2f\n", Player::player_y); // Debug
    }
    else if (keys[SDL_SCANCODE_S]) {
        Player::player_y += totalspeed * deltaTime;
        Player::currentDirection = Player::DIR_DOWN;
        Player::isMoving = true;
        //printf("S pressed, player_y: %.2f\n", Player::player_y); // Debug
    }
    else if (keys[SDL_SCANCODE_A]) {
        Player::player_x -= totalspeed * deltaTime;
        Player::currentDirection = Player::DIR_LEFT;
        Player::isMoving = true;
        //printf("A pressed, player_x: %.2f\n", Player::player_x); // Debug
    }
    else if (keys[SDL_SCANCODE_D]) {
        Player::player_x += totalspeed * deltaTime;
        Player::currentDirection = Player::DIR_RIGHT;
        Player::isMoving = true;
        //printf("D pressed, player_x: %.2f\n", Player::player_x); // Debug
    }

    if (!Player::isMoving) {
        Player::currentFrame = 0; // Reset animation frame when idle
    }

    // Handle jump
    if (keys[SDL_SCANCODE_SPACE] && !Player::isJumping && Player::jumpZ == 0.0f) {
        printf("PRessed Jump\n");
        Player::isJumping = true;
        Player::jumpVelocity = Player::jumpStrength;
    }

    // Debug
    if (Player::isMoving) {
        //printf("DeltaTime: %.6f, Speed: %.2f\n", deltaTime, speed); // Debug
    }
}
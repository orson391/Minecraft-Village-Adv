#include "Player.h"
#include <SDL2/SDL.h>
#include "Map.h"
#include "AddTexture.h"
#include "Coregame.h"
#include "collision.h"
#include "mouse.h"

// Static member definitions
float Player::player_x = 0.0f;
float Player::player_y = 0.0f;
int Player::currentFrame = 0;
bool Player::isMoving = false;
Player::Direction Player::currentDirection = Player::DIR_IDLE;
// jump
float Player::jumpZ = 0.0f;
float Player::jumpVelocity = 0.0f;
bool Player::isJumping = false;
bool Player::attacking = false;

Player::Player(int x, int y, int w, int h)
{
    rect = {x, y, w, h};
    player_x = static_cast<float>(x);
    player_y = static_cast<float>(y);
    walkUp = AddTexture::addTexture("assets/PLAYERWALKINGUP.png");
    walkDown = AddTexture::addTexture("assets/PLAYERWALKINGDOWN.png");
    walkLeft = AddTexture::addTexture("assets/PLAYERWALKINGLEFT.png");
    walkRight = AddTexture::addTexture("assets/PLAYERWALKINGRIGHT.png"); // Fixed typo
}

Player::~Player()
{
    // Clean up textures
    if (walkUp)
        SDL_DestroyTexture(walkUp);
    if (walkDown)
        SDL_DestroyTexture(walkDown);
    if (walkLeft)
        SDL_DestroyTexture(walkLeft);
    if (walkRight)
        SDL_DestroyTexture(walkRight);
}

void Player::update(float &cam_x, float &cam_y, tree &mytree, rock &myrock, animal &myanimal)
{                            // Add tree parameter
    float prev_x = player_x; // Store previous position
    float prev_y = player_y;

    keyevent.update(Coregame::deltaTime); // Update movement
    attack();                             // Update attack state

    updateAnimation();

    // Update rendering rect (screen-space)
    rect = {
        static_cast<int>(player_x - cam_x),
        static_cast<int>(player_y - cam_y),
        frameWidth,
        frameHeight};

    // Jump
    if (isJumping)
    {
        jumpZ += jumpVelocity * Coregame::deltaTime;
        jumpVelocity += gravity * Coregame::deltaTime;
        if (jumpZ >= 0.0f)
        {
            jumpZ = 0.0f;
            isJumping = false;
            jumpVelocity = 0.0f;
        }
    }

    // Collision check in world-space
    static bool lastCollisionState = false;
    SDL_Rect playerWorldRect = {
        static_cast<int>(player_x),
        static_cast<int>(player_y),
        frameWidth,
        frameHeight};

    bool colliding = false;
    if (!isJumping || jumpZ > -10.0f)
    { // Skip collisions when jumping high
        if (collision::checkCollision(&playerWorldRect, &mytree.myobject.ObsticleHitbox))
        {
            colliding = true;
            player_x = prev_x; // Revert movement
            player_y = prev_y;
        }
        if (collision::checkCollision(&playerWorldRect, &myrock.myobject.ObsticleHitbox))
        {
            colliding = true;
            player_x = prev_x; // Revert movement
            player_y = prev_y;
        }
        if (collision::checkCollision(&playerWorldRect, &myanimal.hitbox))
        {
            colliding = true;
        }
    }

    if (colliding != lastCollisionState)
    {
        printf(colliding ? "Collision detected!\n" : "No collision.\n");
        lastCollisionState = colliding;
    }

    if (attacking)
    {
        if (collision::checkCollision(&attackBox, &mytree.myobject.ObsticleHitbox))
        {
            printf("Attack hit tree!\n");
        }
        if (collision::checkCollision(&attackBox, &myrock.myobject.ObsticleHitbox))
        {
            printf("Attack hit rock!\n");
        }
        if (collision::checkCollision(&attackBox, &myanimal.hitbox))
        {
            printf("Attack hit animal!\n");
            myanimal.takeDamage(10);
        }
    }
}

void Player::attack() {
    if (mouse::LeftClick) {
        attacking = true;
        switch (currentDirection) {
        case DIR_UP:
            attackBox = { static_cast<int>(player_x), static_cast<int>(player_y - 20), frameWidth, 20 };
            break;
        case DIR_DOWN:
            attackBox = { static_cast<int>(player_x), static_cast<int>(player_y + frameHeight), frameWidth, 20 };
            break;
        case DIR_LEFT:
            attackBox = { static_cast<int>(player_x - 20), static_cast<int>(player_y), 20, frameHeight };
            break;
        case DIR_RIGHT:
            attackBox = { static_cast<int>(player_x + frameWidth), static_cast<int>(player_y), 20, frameHeight };
            break;
        default:
            attackBox = { 0, 0, 0, 0 };
            break;
        }
    } else {
        attacking = false;
        attackBox = { 0, 0, 0, 0 };
    }
}

void Player::updateAnimation()
{
    if (!isMoving)
    {
        currentFrame = 0;
        return;
    }

    Uint32 now = SDL_GetTicks();
    if (now > lastFrameTime + frameTime)
    {
        currentFrame = (currentFrame + 1) % totalFrames;
        lastFrameTime = now;
    }
}

void Player::renderCharacter(SDL_Renderer *renderer, float camX, float camY)
{
    SDL_Texture *currentTexture = walkDown; // Default to down

    switch (currentDirection)
    {
    case DIR_UP:
        currentTexture = walkUp;
        break;
    case DIR_DOWN:
        currentTexture = walkDown;
        break;
    case DIR_LEFT:
        currentTexture = walkLeft;
        break;
    case DIR_RIGHT:
        currentTexture = walkRight;
        break;
    default:
        break;
    }

    SDL_Rect srcRect = {
        currentFrame * frameWidth,
        0,
        frameWidth,
        frameHeight};

    // SDL_RenderCopy(renderer, currentTexture, &srcRect, &rect);

    // jump

    // Draw shadow before player
    // Draw shadow
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_Rect shadow = {
        rect.x + 8,
        rect.y + rect.h - 10,
        rect.w - 16,
        10};
    SDL_RenderFillRect(renderer, &shadow);

    // Adjust for jump height visually
    SDL_Rect renderRect = rect;
    renderRect.y += static_cast<int>(jumpZ);
    SDL_RenderCopy(renderer, currentTexture, &srcRect, &renderRect);

    // Render attackBox for debugging
    if (attacking)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for attackBox
        SDL_Rect attackRenderRect = {
            attackBox.x - static_cast<int>(camX),
            attackBox.y - static_cast<int>(camY) + static_cast<int>(jumpZ),
            attackBox.w,
            attackBox.h};
        SDL_RenderDrawRect(renderer, &attackRenderRect);
    }

    // debug
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderDrawRect(renderer, &renderRect);
}
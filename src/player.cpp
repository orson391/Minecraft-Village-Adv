#include "Player.h"
#include <SDL2/SDL.h>
#include "Map.h"
#include "AddTexture.h"
#include "Coregame.h"
#include "collision.h"
#include "mouse.h"
#include "tree.h"
#include "rock.h"
#include "animal.h"
#include "enemy.h"

// Static member definitions
float Player::player_x = 0.0f;
float Player::player_y = 0.0f;
int Player::currentFrame = 0;
bool Player::isMoving = false;
bool Player::isRunning =  false;;
Player::Direction Player::currentDirection = Player::DIR_IDLE;
// jump
float Player::jumpZ = 0.0f;
float Player::jumpVelocity = 0.0f;
bool Player::isJumping = false;
bool Player::attacking = false;

Player::Player(int x, int y, int w, int h)
{
    health = maxHealth; // Initialize health to max
    isAlive = true;

    rect = {x, y, w, h};
    player_x = static_cast<float>(x);
    player_y = static_cast<float>(y);
    

    //walkUp = AddTexture::addTexture("assets/ZombieWALKINGDOWN.png");
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

void Player::takeDamage(int damage)
{
    if (!isAlive)
        return;

    health -= damage;

    if (health <= 0)
    {
        health = 0;
        isAlive = false;
        printf("Player broke\n");
        // Drop item
        // SDL_Texture* itemTexture = AddTexture::addTexture("assets/Raw_Mutton_JE3_BE2.png");
        // ItemDrop drop(hitbox.x, hitbox.y, "Meat", itemTexture, 1);
        // printf("Item Droped at %d %d\n", hitbox.x, hitbox.y);
        // ItemDrop drop(x+70, y+40, "Rock", itemTexture, 10);
        // printf("Item Droped at %f %f\n",x,y);

        // getItemDrops().push_back(drop);
    }
}

void Player::update(float &cam_x, float &cam_y, tree &mytree, rock &myrock, animal &myanimal , enemy &myenemy)
{ // Add tree parameter

    if (!isAlive)
    {
        return;
    }

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
        static_cast<int>(player_x)+5,
        static_cast<int>(player_y)+10,
        frameWidth-10,
        frameHeight-10};

    bool colliding = false;
    if (!isJumping || jumpZ > -10.0f)
    { // Skip collisions when jumping high
        if (collision::checkCollision(&playerWorldRect, &mytree.myobject.ObsticleHitbox))
        {
            if (!mytree.myobject.isAlive)
            {
                colliding = false;
            }
            else
            {
                colliding = true;
                player_x = prev_x; // Revert movement
                player_y = prev_y;
            }
        }
        if (collision::checkCollision(&playerWorldRect, &myrock.myobject.ObsticleHitbox))
        {
            if (!myrock.myobject.isAlive)
            {
                colliding = false;
            }
            else
            {

                colliding = true;
                player_x = prev_x; // Revert movement
                player_y = prev_y;
            }
        }
        if (collision::checkCollision(&playerWorldRect, &myanimal.hitbox))
        {

            colliding = true;
        }
        if (collision::checkCollision(&playerWorldRect, &myenemy.hitbox))
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
            mytree.takeDamage(5);
        }
        if (collision::checkCollision(&attackBox, &myrock.myobject.ObsticleHitbox))
        {
            printf("Attack hit rock!\n");
            myrock.takeDamage(5);
        }
        if (collision::checkCollision(&attackBox, &myanimal.hitbox))
        {
            printf("Attack hit animal!\n");
            myanimal.takeDamage(10);
        }
        if (collision::checkCollision(&attackBox, &myenemy.hitbox))
        {
            printf("Attack hit enemy!\n");
            myenemy.takeDamage(10);
        }

    }
}

void Player::attack()
{
    if (!isAlive)
    {
        return;
    }
    if (mouse::RightClick)
    {
        //health -=5;
        takeDamage(5);
    }
    

    if (mouse::LeftClick)
    {
        attacking = true;
        switch (currentDirection)
        {
        case DIR_UP:
            attackBox = {static_cast<int>(player_x), static_cast<int>(player_y - 20), frameWidth, 20};
            break;
        case DIR_DOWN:
            attackBox = {static_cast<int>(player_x), static_cast<int>(player_y + frameHeight), frameWidth, 20};
            break;
        case DIR_LEFT:
            attackBox = {static_cast<int>(player_x - 20), static_cast<int>(player_y), 20, frameHeight};
            break;
        case DIR_RIGHT:
            attackBox = {static_cast<int>(player_x + frameWidth), static_cast<int>(player_y), 20, frameHeight};
            break;
        default:
            attackBox = {0, 0, 0, 0};
            break;
        }
    }
    else
    {
        attacking = false;
        attackBox = {0, 0, 0, 0};
    }
}

void Player::updateAnimation()
{
    if (!isAlive)
    {
        return;
    }
    
    if (!isMoving)
    {
        currentFrame = 0;
        return;
    }

    Uint32 now = SDL_GetTicks();
    int running = 0;
    if(isRunning)
    {
        running = 60;
    }
    if (now > lastFrameTime + frameTime - running)
    {
        currentFrame = (currentFrame + 1) % totalFrames;
        lastFrameTime = now;
    }
}

void Player::renderCharacter(SDL_Renderer *renderer, float camX, float camY)
{

    if (!isAlive)
        return;

    /* code */

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


    //healthbar
    int totalBlocks = 8;
    int blockWidth = 10;
    int blockHeight = 10;
    int blockSpacing = 13; // space between blocks
    int maxHealth = 100;   // for example
    int healthPerBlock = maxHealth / totalBlocks;

    SDL_Rect HealthRect{0, 0, blockWidth, blockHeight};

    for (int i = 0; i < totalBlocks; ++i)
    {
        HealthRect.x = 250 + i * blockSpacing;
        HealthRect.y = 530;

        int blockThreshold = (i + 1) * healthPerBlock;

        if (health >= blockThreshold)
        {
            // Full red block
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else if (health > i * healthPerBlock)
        {
            // Partial block (fade color for visual effect)
            float fillPercent = (float)(health - i * healthPerBlock) / healthPerBlock;
            SDL_SetRenderDrawColor(renderer, 255, static_cast<Uint8>(255 * (1 - fillPercent)), static_cast<Uint8>(255 * (1 - fillPercent)), 255);
        }
        else
        {
            // Empty block (dark grey)
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        }

        SDL_RenderFillRect(renderer, &HealthRect);
    }

    // debug
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderDrawRect(renderer, &renderRect);
    //SDL_RenderDrawRect(renderer, &woorld);
}
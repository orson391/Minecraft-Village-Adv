
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
#include <math.h>

// Static member definitions
float Player::player_x = 0.0f;
float Player::player_y = 0.0f;
int Player::currentFrame = 0;
bool Player::isMoving = false;
bool Player::isRunning = false;
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

    walkUp = AddTexture::addTexture("assets/PLAYERWALKINGUP.png");
    walkDown = AddTexture::addTexture("assets/PLAYERWALKINGDOWN.png");
    walkLeft = AddTexture::addTexture("assets/PLAYERWALKINGLEFT.png");
    walkRight = AddTexture::addTexture("assets/PLAYERWALKINGRIGHT.png");

    crossHair = AddTexture::addTexture("assets/crosshair.png");
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

    if(crossHair)
        SDL_DestroyTexture(crossHair);
}

void Player::takeDamage(int damage, float attackerX, float attackerY)
{
    if (!isAlive)
        return;

    health -= damage;

    if (health <= 0)
    {
        health = 0;
        isAlive = false;
        printf("Player broke\n");
    }
    // Calculate knockback direction
    float dx = player_x - attackerX;
    float dy = player_y - attackerY;
    float length = sqrtf(dx * dx + dy * dy);

    if (length != 0.0f)
    {
        knockbackX = (dx / length) * knockbackStrength;
        knockbackY = (dy / length) * knockbackStrength;
        knockbackTimer = knockbackDuration;
    }
}

void Player::update(float &cam_x, float &cam_y, tree &mytree, rock &myrock, animal &myanimal, enemy &myenemy, int mouseX, int mouseY)
{
    if (!isAlive)
    {
        return;
    }

    float prev_x = player_x; // Store previous position
    float prev_y = player_y;

    keyevent.update(Coregame::deltaTime); // Update movement
    attack(mouseX, mouseY, cam_x, cam_y); // Update attack state with mouse and camera position

    updateAnimation();

    // Apply knockback if active
    if (knockbackTimer > 0.0f)
    {
        float dt = Coregame::deltaTime;
        player_x += knockbackX * dt;
        player_y += knockbackY * dt;

        knockbackTimer -= dt;

        // Gradually reduce knockback force (optional)
        knockbackX *= 0.9f;
        knockbackY *= 0.9f;
    }

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
        static_cast<int>(player_x) + 5,
        static_cast<int>(player_y) + 10,
        frameWidth - 10,
        frameHeight - 10};

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
            myenemy.takeDamage(10, static_cast<float>(attackBox.x), static_cast<float>(attackBox.y));
        }
    }
}

void Player::attack(int mouseX, int mouseY, float camX, float camY)
{
    if (!isAlive)
    {
        return;
    }

    if (mouse::LeftClick || mouse::RightClick)
    {
        attacking = true;
        
        // Center of the player in world space
        float centerX = player_x + frameWidth / 2;
        float centerY = player_y + frameHeight / 2;
        
        // Convert mouse position (screen space) to world space
        float worldMouseX = mouseX + camX;
        float worldMouseY = mouseY + camY;
        
        // Vector from player center to mouse position
        float dx = worldMouseX - centerX;
        float dy = worldMouseY - centerY;
        float distance = sqrtf(dx * dx + dy * dy);
        
        // Normalize direction
        float dirX = 0.0f;
        float dirY = 0.0f;
        if (distance != 0.0f)
        {
            dirX = dx / distance;
            dirY = dy / distance;
        }
        
        // Define attack box size and offset
        const int attackWidth = 30;
        const int attackHeight = 30;
        const float attackOffset = 10.0f; // Distance from player center to start of attack box
        
        // Position attack box in the direction of the crosshair in world space
        attackBox = {
            static_cast<int>(centerX + dirX * attackOffset) -15,
            static_cast<int>(centerY + dirY * attackOffset) -15,
            attackWidth,
            attackHeight
        };
        
        // Debug output to check positions
        printf("Player center: (%.1f, %.1f), Mouse world: (%.1f, %.1f), Attack box: (%d, %d, %d, %d)\n",
               centerX, centerY, worldMouseX, worldMouseY,
               attackBox.x, attackBox.y, attackBox.w, attackBox.h);
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
    if (isRunning)
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

    // Draw shadow before player
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

    // Healthbar
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

    DrawCrosshair(renderer, mouse::mouseX, mouse::mouseY);

    // Debug
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderDrawRect(renderer, &renderRect);
}

void Player::DrawCrosshair(SDL_Renderer* renderer, int mouseX, int mouseY) {
    // Center of the rect
    float centerX = rect.x + rect.w / 2;
    float centerY = rect.y + rect.h / 2;

    // Vector from center to mouse
    float dx = mouseX - centerX;
    float dy = mouseY - centerY;

    // Distance
    float distance = sqrtf(dx * dx + dy * dy);

    // Clamp to 50-pixel radius
    float maxDistance = 50.0f;
    if (distance > maxDistance) {
        float scale = maxDistance / distance;
        dx *= scale;
        dy *= scale;
    }

    // Crosshair position
    int crossX = (int)(centerX + dx);
    int crossY = (int)(centerY + dy);

    crossHairRect = { crossX - 5, crossY - 5, 16, 16 }; // Centered 16x16 square

    // Draw the crosshair texture centered on this point
    SDL_RenderCopy(renderer, crossHair, NULL, &crossHairRect);
}
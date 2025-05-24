#include "enemy.h"
#include "AddTexture.h"
#include "ItemDrop.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include "Coregame.h"

extern std::vector<ItemDrop>& getItemDrops();

enemy::enemy(float startX, float startY)
    : x(startX), y(startY), ve_x(0), ve_y(0), state(enemyState::Idle), stateTimer(2.0f),
      health(maxHealth), isAlive(true), showHealthBar(false), healthBarTimer(0.0f)
{
    walkUp = AddTexture::addTexture("assets/ZombieWALKINGUP.png");
    walkDown = AddTexture::addTexture("assets/ZombieWALKINGDOWN.png");
    walkLeft = AddTexture::addTexture("assets/ZombieWALKINGLEFT.png");
    walkRight = AddTexture::addTexture("assets/ZombieWALKINGRIGHT.png");
    hitbox = {static_cast<int>(x), static_cast<int>(y), frameWidth, frameHeight};
    rect = hitbox; // Initialize rect for rendering
}

enemy::~enemy()
{
    if (walkUp) SDL_DestroyTexture(walkUp);
    if (walkDown) SDL_DestroyTexture(walkDown);
    if (walkLeft) SDL_DestroyTexture(walkLeft);
    if (walkRight) SDL_DestroyTexture(walkRight);
}

void enemy::takeDamage(int damage)
{
    if (!isAlive) return;

    health -= damage;
    showHealthBar = true;
    healthBarTimer = 2.0f; // Show health bar for 2 seconds

    if (health <= 0)
    {
        health = 0;
        isAlive = false;
        printf("Zombie Died\n");

        SDL_Texture* itemTexture = AddTexture::addTexture("assets/Raw_Mutton_JE3_BE2.png");
        if (itemTexture)
        {
            ItemDrop drop(hitbox.x, hitbox.y, "Rotten Flesh", itemTexture, 2);
            getItemDrops().push_back(drop);
        }
    }
}

void enemy::update(float deltaTime, float playerX, float playerY, float camX, float camY)
{
    if (!isAlive) return;

    // Determine direction based on velocity
    if (ve_x > 0) currentDirection = Direction::RIGHT;
    else if (ve_x < 0) currentDirection = Direction::LEFT;
    else if (ve_y > 0) currentDirection = Direction::DOWN;
    else if (ve_y < 0) currentDirection = Direction::UP;
    else currentDirection = Direction::IDLE;

    isMoving = (ve_x != 0 || ve_y != 0);

    // Update attack box
    if (state == enemyState::Attack)
    {
        attacking = true;
        switch (currentDirection)
        {
        case Direction::UP:
            attackBox = {static_cast<int>(x), static_cast<int>(y - 20), frameWidth, 20};
            break;
        case Direction::DOWN:
            attackBox = {static_cast<int>(x), static_cast<int>(y + frameHeight), frameWidth, 20};
            break;
        case Direction::LEFT:
            attackBox = {static_cast<int>(x - 20), static_cast<int>(y), 20, frameHeight};
            break;
        case Direction::RIGHT:
            attackBox = {static_cast<int>(x + frameWidth), static_cast<int>(y), 20, frameHeight};
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

    updateAnimation();

    // Update health bar timer
    if (showHealthBar)
    {
        healthBarTimer -= deltaTime;
        if (healthBarTimer <= 0.0f)
        {
            showHealthBar = false;
        }
    }

    float dx = playerX - x;
    float dy = playerY - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 30.0f && state != enemyState::Attack)
    {
        state = enemyState::Attack;
        float angle = std::atan2(dy, dx);
        ve_x = -std::cos(angle) * 150.0f; // Move toward player
        ve_y = -std::sin(angle) * 150.0f;
        stateTimer = 1.5f;
    }

    stateTimer -= deltaTime;

    switch (state)
    {
    case enemyState::Idle:
        ve_x = ve_y = 0;
        isMoving = false;
        if (stateTimer <= 0)
        {
            state = enemyState::Wander;
            float angle = static_cast<float>(rand() % 360) * M_PI / 180.0f;
            ve_x = std::cos(angle) * 50.0f;
            ve_y = std::sin(angle) * 50.0f;
            stateTimer = 3.0f;
        }
        break;
    case enemyState::Wander:
        if (stateTimer <= 0)
        {
            state = enemyState::Idle;
            ve_x = ve_y = 0;
            stateTimer = 2.0f;
        }
        break;
    case enemyState::Attack:
        if (stateTimer <= 0 || distance > 30.0f)
        {
            state = enemyState::Idle;
            ve_x = ve_y = 0;
            stateTimer = 2.0f;
        }
        break;
    }

    x += ve_x * deltaTime;
    y += ve_y * deltaTime;

    hitbox = {static_cast<int>(x), static_cast<int>(y), frameWidth, frameHeight};
    rect = {static_cast<int>(x - camX), static_cast<int>(y - camY), frameWidth, frameHeight};
}

void enemy::updateAnimation()
{
    if (!isAlive || !isMoving)
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

void enemy::draw(SDL_Renderer *renderer, float camX, float camY)
{
    if (!isAlive) return;

    SDL_Texture *currentTexture = walkDown; // Default to down
    switch (currentDirection)
    {
    case Direction::UP:
        currentTexture = walkUp;
        break;
    case Direction::DOWN:
        currentTexture = walkDown;
        break;
    case Direction::LEFT:
        currentTexture = walkLeft;
        break;
    case Direction::RIGHT:
        currentTexture = walkRight;
        break;
    default:
        break;
    }

    SDL_Rect srcRect = {
        currentFrame * frameWidth,
        0,
        frameWidth,
        frameHeight
    };

    SDL_RenderCopy(renderer, currentTexture, &srcRect, &rect);

    // Draw shadow
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_Rect shadow = {
        rect.x + 8,
        rect.y + rect.h - 10,
        rect.w - 16,
        10
    };
    SDL_RenderFillRect(renderer, &shadow);

    // Render attackBox for debugging
    if (attacking)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
        SDL_Rect attackRenderRect = {
            attackBox.x - static_cast<int>(camX),
            attackBox.y - static_cast<int>(camY),
            attackBox.w,
            attackBox.h
        };
        SDL_RenderDrawRect(renderer, &attackRenderRect);
    }

    // Draw health bar if visible
    if (showHealthBar)
    {
        const int totalBlocks = 8;
        const int blockWidth = 4;
        const int blockHeight = 2;
        const int blockSpacing = 5;
        const int healthPerBlock = maxHealth / totalBlocks;

        SDL_Rect healthRect = {rect.x, rect.y - 10, blockWidth, blockHeight};

        for (int i = 0; i < totalBlocks; ++i)
        {
            healthRect.x = rect.x + i * blockSpacing;
            int blockThreshold = (i + 1) * healthPerBlock;

            if (health >= blockThreshold)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            }
            else if (health > i * healthPerBlock)
            {
                float fillPercent = (float)(health - i * healthPerBlock) / healthPerBlock;
                SDL_SetRenderDrawColor(renderer, 255, static_cast<Uint8>(255 * (1 - fillPercent)),
                                       static_cast<Uint8>(255 * (1 - fillPercent)), 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255); // Dark grey
            }
            SDL_RenderFillRect(renderer, &healthRect);
        }
    }

    // Debug hitbox
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_Rect renderRect = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderDrawRect(renderer, &renderRect);
}
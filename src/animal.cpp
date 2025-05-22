#include "animal.h"
#include "AddTexture.h"
#include "ItemDrop.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include "Coregame.h"

extern std::vector<ItemDrop>& getItemDrops(); // Forward declaration for global item drop list

animal::animal(float startX, float startY) {
    x = startX;
    y = startY;
    ve_x = ve_y = 0;
    state = animalState::Idle;
    stateTimer = 2.0f;
    health = maxHealth; // Initialize health to max
    isAlive = true;
    showHealthBar = false;
    healthBarTimer = 0.0f;
    skin = AddTexture::addTexture("assets/sheep2.png"); // Load texture
    //hitbox = { static_cast<int>(x), static_cast<int>(y), 32, 32 };
}

animal::~animal() {
    if (skin) SDL_DestroyTexture(skin);
}

void animal::update(float deltaTime, float playerX, float playerY, float cam_x, float cam_y) {
    if (!isAlive) return;

    // Update health bar timer
    if (showHealthBar) {
        healthBarTimer -= deltaTime;
        if (healthBarTimer <= 0.0f) {
            showHealthBar = false;
        }
    }

    float dx = x - playerX;
    float dy = y - playerY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 10.f && state != animalState::Flee) {
        state = animalState::Flee;
        float angle = std::atan2(dy, dx);
        ve_x = std::cos(angle) * 150.0f;
        ve_y = std::sin(angle) * 150.0f;
        stateTimer = 1.5f;
    }

    stateTimer -= deltaTime;

    switch (state) {
    case animalState::Idle:
        ve_x = ve_y = 0;
        if (stateTimer <= 0) {
            state = animalState::Wander;
            float angle = static_cast<float>(rand() % 360) * M_PI / 180.0f;
            ve_x = std::cos(angle) * 50.0f;
            ve_y = std::sin(angle) * 50.0f;
            stateTimer = 3.0f;
        }
        break;
    case animalState::Wander:
        if (stateTimer <= 0) {
            state = animalState::Idle;
            ve_x = ve_y = 0;
            stateTimer = 2.0f;
        }
        break;
    case animalState::Flee:
        if (stateTimer <= 0) {
            state = animalState::Idle;
            ve_x = ve_y = 0;
            stateTimer = 2.0f;
        }
        break;
    }

    x += ve_x * deltaTime;
    y += ve_y * deltaTime;
    //printf("animal location %f %f\n",x,y);

    hitbox = { static_cast<int>(x), static_cast<int>(y), 32, 32 };
}

void animal::takeDamage(int damage) {
    if (!isAlive) return;

    health -= damage;
    showHealthBar = true; // Show health bar on hit
    healthBarTimer = 3.0f; // Display for 3 seconds
    if (health <= 0) {
        health = 0;
        isAlive = false;
        printf("Animal dead\n");
         // Drop item
        SDL_Texture* itemTexture = AddTexture::addTexture("assets/Raw_Mutton_JE3_BE2.png");
        //ItemDrop drop(hitbox.x, hitbox.y, "Meat", itemTexture, 1);
        //printf("Item Droped at %d %d\n", hitbox.x, hitbox.y);
        ItemDrop drop(x, y, "Meat", itemTexture, 1);
        //printf("Item Droped at %f %f\n",x,y);
        
        getItemDrops().push_back(drop);
    }
}

void animal::draw(SDL_Renderer* renderer, float cam_x, float cam_y) {
    if (!isAlive) return;

    SDL_Rect renderRect = {
        hitbox.x - static_cast<int>(cam_x),
        hitbox.y - static_cast<int>(cam_y),
        hitbox.w,
        hitbox.h
    };

    // Render shadow
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &renderRect);

    // Render texture
    AddTexture::drawTexture(skin, &renderRect);

    // Render health bar if visible
    if (showHealthBar) {
        // Health bar dimensions
        const int barWidth = 32; // Same width as animal
        const int barHeight = 6; // Thin bar
        const int barOffsetY = -10; // 10 pixels above animal

        // Background (red, full width)
        SDL_Rect bgRect = {
            renderRect.x,
            renderRect.y + barOffsetY,
            barWidth,
            barHeight
        };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &bgRect);

        // Foreground (green, proportional to health)
        int fillWidth = static_cast<int>((static_cast<float>(health) / maxHealth) * barWidth);
        SDL_Rect fillRect = {
            renderRect.x,
            renderRect.y + barOffsetY,
            fillWidth,
            barHeight
        };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &fillRect);
    }

    

    // Render hitbox (debug)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &renderRect);
}

void animal::drawimage() {
    // Placeholder for additional image rendering if needed
}
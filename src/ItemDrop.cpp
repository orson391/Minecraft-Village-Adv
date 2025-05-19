#include "ItemDrop.h"
#include "inventory.h"
#include "AddTexture.h"

ItemDrop::ItemDrop(float x, float y, const std::string& name, SDL_Texture* texture, int quantity)
    : x(x), y(y), name(name), texture(texture), quantity(quantity), markedForDeletion(false) {
    hitbox = { static_cast<int>(x), static_cast<int>(y), 16, 16 }; // Small hitbox for item
}

ItemDrop::~ItemDrop() {
    // Texture is managed externally (e.g., by AddTexture or inventory)
}

void ItemDrop::update(float deltaTime) {
    // No movement for now; items stay stationary
}

void ItemDrop::draw(SDL_Renderer* renderer, float cam_x, float cam_y) {
    if (markedForDeletion) return;

    SDL_Rect renderRect = {
        hitbox.x - static_cast<int>(cam_x),
        hitbox.y - static_cast<int>(cam_y),
        hitbox.w,
        hitbox.h
    };

    // Render item texture
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &renderRect);
    } else {
        // Fallback: draw a blue square if no texture
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &renderRect);
    }

    // Debug hitbox
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
    SDL_RenderDrawRect(renderer, &renderRect);
}

bool ItemDrop::pickUp(inventory& inv) {
    if (markedForDeletion) return false;
    if (inv.addItem(name, texture, quantity)) {
        markedForDeletion = true;
        return true;
    }
    return false;
}
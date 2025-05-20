#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "inventory.h"

class ItemDrop {
public:
    ItemDrop(float x, float y, const std::string& name, SDL_Texture* texture, int quantity);
    ~ItemDrop();

    void update(float deltaTime);
    void draw(SDL_Renderer* renderer, float cam_x, float cam_y);
    bool pickUp(inventory& inv); // Adds item to inventory and marks for deletion

    //SDL_Rect getHitbox() const { return hitbox; }
    const SDL_Rect& getHitbox() const { return hitbox; }
    bool isMarkedForDeletion() const { return markedForDeletion; }
    const std::string& getName() const { return name; }
//private:
    float x, y; // World position
    SDL_Rect hitbox; // World-space hitbox
    std::string name; // Item name (e.g., "Meat")
    SDL_Texture* texture; // Item icon
    int quantity; // Quantity to add to inventory
    bool markedForDeletion; // Flag for cleanup
};
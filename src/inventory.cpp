#include "inventory.h"

inventory::inventory() : selected_index(0), is_open(false) {
    for (auto& item : items) {
        item = Item();
    }
}

bool inventory::addItem(const std::string& name, SDL_Texture* icon, int quantity) {
    // Check for existing item
    for (auto& item : items) {
        if (item.name == name) {
            item.quantity += quantity;
            return true;
        }
    }

    // Find empty slot
    for (auto& item : items) {
        if (item.name.empty()) {
            item.name = name;
            item.icon = icon;
            item.quantity = quantity;
            return true;
        }
    }
    return false;
}

void inventory::render(SDL_Renderer* renderer, TTF_Font* font) const {
    if (!is_open) return;

    SDL_Rect slot{ 0, 0, 20, 20 };

    for (size_t i = 0; i < INVENTORY_SIZE; ++i) {
        slot.x = 250 + static_cast<int>(i) * 30;
        slot.y = 550;

        // Draw slot background
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &slot);

        // Draw item icon
        if (items[i].icon != nullptr) {
            SDL_RenderCopy(renderer, items[i].icon, nullptr, &slot);
        }

        // Optionally draw quantity (requires SDL_ttf)
        if (items[i].quantity > 0) {
            std::string qtyText = std::to_string(items[i].quantity);
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(font, qtyText.c_str(), white);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect qtyRect = {slot.x + 10, slot.y + 10, 10, 10};
            SDL_RenderCopy(renderer, texture, nullptr, &qtyRect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}

void inventory::handleInput(const SDL_Event& event) {
    is_open = true;
    //printf("Inventory toggled: %s\n", is_open ? "open" : "closed");
   
    // if (event.type == SDL_KEYDOWN) {
    //     if (event.key.keysym.sym == SDLK_i) {
    //         is_open = !is_open;
     //     }
    //         
    // }
}

void inventory::setSelectedIndex(int index) {
    if (index >= 0 && index < static_cast<int>(INVENTORY_SIZE)) {
        selected_index = index;
    }
}
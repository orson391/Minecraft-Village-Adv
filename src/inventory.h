#pragma once
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class inventory {
private:
    static constexpr size_t INVENTORY_SIZE = 10;

    struct Item {
        std::string name;    // Name of the item
        SDL_Texture* icon;   // Icon of the item (optional)
        int quantity;

        Item() : name(""), icon(nullptr), quantity(0) {}
    };

    std::array<Item, INVENTORY_SIZE> items;
    int selected_index;
    bool is_open;

public:
    // Constructor
    inventory() : selected_index(0), is_open(false) {
        for (auto& item : items) {
            item = Item();
        }
    }

    // Add item to inventory
    bool addItem(const std::string& name, SDL_Texture* icon, int quantity) {
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

    // Render inventory UI
    void render(SDL_Renderer* renderer, TTF_Font* font) const {
        if (!is_open) return;

        SDL_Rect slot{ 0, 0, 20, 20 };

        for (size_t i = 0; i < INVENTORY_SIZE; ++i) {
            slot.x = 250 + static_cast<int>(i) * 30;
            slot.y = 550;

            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, &slot);

            if (items[i].icon != nullptr) {
                SDL_RenderCopy(renderer, items[i].icon, nullptr, &slot);
            }
        }
    }

    // Handle input
    void handleInput(const SDL_Event& event) {
        is_open = true;
        // if (event.type == SDL_KEYDOWN) {
        //     if (event.key.keysym.sym == SDLK_i) {
        //         printf("Presesd I");
        //         is_open = !is_open;
        //     }

        //     if (is_open) {
        //         // Add navigation or selection logic here if needed
        //     }
        // }
    }

    // Getters
    bool isOpen() const { return is_open; }
    int getSelectedIndex() const { return selected_index; }

    // Setters
    void setSelectedIndex(int index) {
        if (index >= 0 && index < static_cast<int>(INVENTORY_SIZE)) {
            selected_index = index;
        }
    }
};
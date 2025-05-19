#pragma once


#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class inventory {
private:
    static constexpr size_t INVENTORY_SIZE = 10;

    struct Item {
        std::string name;
        SDL_Texture* icon;
        int quantity;

        Item() : name(""), icon(nullptr), quantity(0) {}
    };

    std::array<Item, INVENTORY_SIZE> items;
    int selected_index;
    bool is_open;

public:
    inventory();
    bool addItem(const std::string& name, SDL_Texture* icon, int quantity);
    void render(SDL_Renderer* renderer, TTF_Font* font) const;
    void handleInput(const SDL_Event& event);
    bool isOpen() const { return is_open; }
    int getSelectedIndex() const { return selected_index; }
    void setSelectedIndex(int index);
};

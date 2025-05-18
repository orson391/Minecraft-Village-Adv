#include "Map.h"
#include <SDL2/SDL_image.h>
#include <iostream>

const float RENDER_DISTANCE = 1000.0f;

Map::Map() {
    // Initialize world to grass (0)
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            world[y][x] = 0; // Grass
        }
    }

    // Load Krita map PNG
    SDL_Surface* surface = IMG_Load("assets/level1.png");
    if (!surface) {
        std::cerr << "Failed to load level1.png: " << SDL_GetError() << std::endl;
        return;
    }

    // Lock surface for pixel access
    if (SDL_LockSurface(surface) < 0) {
        std::cerr << "Failed to lock surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Read pixels (assuming 32x32 tiles, RGB colors)
    Uint32* pixels = (Uint32*)surface->pixels;
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            // Sample pixel at tile center (x*32 + 16, y*32 + 16)
            int pixelX = x * TILE_SIZE + 16;
            int pixelY = y * TILE_SIZE + 16;
            if (pixelX < surface->w && pixelY < surface->h) {
                Uint32 pixel = pixels[pixelY * surface->w + pixelX];
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

                // Assign tile based on color (adjust RGB values based on your tiles)
                if (g > r && g > b) { // Green-ish for grass
                    world[y][x] = 0; // Grass
                }
                else if (b > r && b > g) { // Blue-ish for water
                    world[y][x] = 1; // Water
                }
            }
        }
    }

    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);
}

Map::~Map() {
    SDL_DestroyTexture(grassTop);
    SDL_DestroyTexture(waterTop);
}

void Map::loadTextures(SDL_Renderer* renderer) {
    grassTop = IMG_LoadTexture(renderer, "assets/tempgrass.png");
    waterTop = IMG_LoadTexture(renderer, "assets/tempwater.png");

    if (!grassTop || !waterTop) {
        std::cerr << "Failed to load textures: " << SDL_GetError() << std::endl;
    }
}

void Map::drawWorld(SDL_Renderer* renderer, float cam_x, float cam_y) {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            float dx = (x * TILE_SIZE + TILE_SIZE / 2) - cam_x;
            float dy = (y * TILE_SIZE + TILE_SIZE / 2) - cam_y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= RENDER_DISTANCE) {
                int type = world[y][x];
                SDL_Rect tile = {
                    static_cast<int>(x * TILE_SIZE - cam_x),
                    static_cast<int>(y * TILE_SIZE - cam_y),
                    TILE_SIZE,
                    TILE_SIZE
                };

                SDL_Texture* topTexture = nullptr;
                switch (type) {
                case 0: topTexture = grassTop; break; // Grass
                case 1: topTexture = waterTop; break; // Water
                }

                if (topTexture) {
                    SDL_RenderCopy(renderer, topTexture, nullptr, &tile);
                }
            }
        }
    }
}

bool Map::isSolid(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    if (tileX < 0 || tileX >= WORLD_WIDTH || tileY < 0 || tileY >= WORLD_HEIGHT) {
        return true; // Out of bounds is solid
    }
    return false; // No tiles are solid (adjust if water is solid)
}

bool Map::isJumpable(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    if (tileX < 0 || tileX >= WORLD_WIDTH || tileY < 0 || tileY >= WORLD_HEIGHT) {
        return false;
    }
    return world[tileY][tileX] == 1; // Water is jumpable
}
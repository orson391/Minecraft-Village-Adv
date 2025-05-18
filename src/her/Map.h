#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>

const int WORLD_WIDTH = 60;  // Matches Krita map (1920 / 32)
const int WORLD_HEIGHT = 32; // Matches Krita map (1024 / 32)
const int TILE_SIZE = 32;

class Map {
public:
    Map();
    ~Map();
    void loadTextures(SDL_Renderer* renderer);
    void drawWorld(SDL_Renderer* renderer, float cam_x, float cam_y);
    bool isSolid(int x, int y);    // For collision
    bool isJumpable(int x, int y); // For jumpable tiles

private:
    SDL_Texture* grassTop;
    SDL_Texture* waterTop;
    int world[WORLD_HEIGHT][WORLD_WIDTH];
};

#endif
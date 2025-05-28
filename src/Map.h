#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>

const int WORLD_WIDTH = 60;  // Matches original dimensions (1920 / 32)
const int WORLD_HEIGHT = 32; // Matches original dimensions (1024 / 32)
const int TILE_SIZE = 32;

// Base tile types (for terrain generation)
const int BASE_GRASS = 0;
const int BASE_WATER = 1;
const int BASE_CLIFF = 2;

// Tile variants (for rendering)
enum TileVariant {
    CENTER,          // Center tile (e.g., grass center)
    EDGE_TOP,        // Edge with neighbor above
    EDGE_BOTTOM,     // Edge with neighbor below
    EDGE_LEFT,       // Edge with neighbor to the left
    EDGE_RIGHT,      // Edge with neighbor to the right
    EDGE_TOP_LEFT,   // Corner: top-left
    EDGE_TOP_RIGHT,  // Corner: top-right
    EDGE_BOTTOM_LEFT,// Corner: bottom-left
    EDGE_BOTTOM_RIGHT// Corner: bottom-right
};

class Map {
public:
    Map();
    ~Map();
    void loadTextures(SDL_Renderer* renderer);
    void drawWorld(SDL_Renderer* renderer, float cam_x, float cam_y);
    bool isSolid(int x, int y);    // For collision
    bool isJumpable(int x, int y); // For jumpable tiles

private:
    SDL_Texture* tilesetTexture; // Single texture for the tileset
    // Source rectangles for each tile type and variant
    SDL_Rect tileSources[3][9];  // 3 base types (GRASS, WATER, CLIFF) x 9 variants (CENTER + 8 edges)
    int baseWorld[WORLD_HEIGHT][WORLD_WIDTH]; // Base terrain (GRASS, WATER, CLIFF)
    struct Tile {
        int baseType;
        TileVariant variant;
    };
    Tile world[WORLD_HEIGHT][WORLD_WIDTH]; // Final tiles with variants

    // Perlin noise helper functions
    float perlinNoise(float x, float y, float scale);
    float lerp(float a, float b, float t);
    float dotGridGradient(int ix, int iy, float x, float y);
};

#endif
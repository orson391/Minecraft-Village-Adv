#include "Map.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

const float RENDER_DISTANCE = 1500.0f;

// Permutation table for Perlin noise (simplified)
static int permutation[] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
    35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
    55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
    18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
    250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
    189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
    172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
    107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

Map::Map()
{
    // Step 1: Generate base terrain using Perlin noise
    float noiseScale = 0.1f; // Adjust scale for larger/smaller features (smaller value = larger features)
    float noise[WORLD_HEIGHT][WORLD_WIDTH];
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            // Generate noise value for each tile
            noise[y][x] = perlinNoise(x, y, noiseScale);
            // Map noise value to tile type (adjusted for mostly water)
            if (noise[y][x] < -0.1f)
            { // Low values: water (most of the map)
                baseWorld[y][x] = BASE_WATER;
            }
            else if (noise[y][x] < 0.2f)
            { // Medium values: grass (islands)
                baseWorld[y][x] = BASE_GRASS;
            }
            else
            { // High values: cliff (peaks on islands)
                baseWorld[y][x] = BASE_CLIFF;
            }
        }
    }

    // Step 2: Light smoothing pass to refine islands
    for (int iteration = 0; iteration < 2; iteration++)
    { // Fewer iterations for Perlin noise
        int newBaseWorld[WORLD_HEIGHT][WORLD_WIDTH];
        for (int y = 0; y < WORLD_HEIGHT; y++)
        {
            for (int x = 0; x < WORLD_WIDTH; x++)
            {
                // Count neighboring tiles of each type
                int grassCount = 0, waterCount = 0, cliffCount = 0;
                for (int dy = -1; dy <= 1; dy++)
                {
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        if (dx == 0 && dy == 0)
                            continue; // Skip the tile itself
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < WORLD_WIDTH && ny >= 0 && ny < WORLD_HEIGHT)
                        {
                            switch (baseWorld[ny][nx])
                            {
                            case BASE_GRASS:
                                grassCount++;
                                break;
                            case BASE_WATER:
                                waterCount++;
                                break;
                            case BASE_CLIFF:
                                cliffCount++;
                                break;
                            }
                        }
                    }
                }

                // Smoothing rules (light touch to preserve Perlin noise structure)
                int currentType = baseWorld[y][x];
                if (currentType == BASE_WATER)
                {
                    if (grassCount >= 6 && waterCount < 2)
                    {
                        newBaseWorld[y][x] = BASE_GRASS; // Water heavily surrounded by grass becomes grass
                    }
                    else if (cliffCount >= 6 && waterCount < 2)
                    {
                        newBaseWorld[y][x] = BASE_CLIFF; // Water heavily surrounded by cliffs becomes cliff
                    }
                    else
                    {
                        newBaseWorld[y][x] = BASE_WATER;
                    }
                }
                else if (currentType == BASE_GRASS)
                {
                    if (waterCount >= 5)
                    {
                        newBaseWorld[y][x] = BASE_WATER; // Grass surrounded by water becomes water
                    }
                    else if (cliffCount >= 4 && grassCount < 3)
                    {
                        newBaseWorld[y][x] = BASE_CLIFF; // Grass near cliffs becomes cliff
                    }
                    else
                    {
                        newBaseWorld[y][x] = BASE_GRASS;
                    }
                }
                else if (currentType == BASE_CLIFF)
                {
                    if (waterCount >= 5)
                    {
                        newBaseWorld[y][x] = BASE_WATER; // Cliff surrounded by water becomes water
                    }
                    else if (grassCount >= 4 && cliffCount < 3)
                    {
                        newBaseWorld[y][x] = BASE_GRASS; // Cliff near grass becomes grass
                    }
                    else
                    {
                        newBaseWorld[y][x] = BASE_CLIFF;
                    }
                }
            }
        }
        // Copy the new base world back
        for (int y = 0; y < WORLD_HEIGHT; y++)
        {
            for (int x = 0; x < WORLD_WIDTH; x++)
            {
                baseWorld[y][x] = newBaseWorld[y][x];
            }
        }
    }

    // Step 3: Assign tile variants based on neighbors
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            world[y][x].baseType = baseWorld[y][x];

            // Check neighboring tiles to determine the variant
            bool top = y > 0 && baseWorld[y - 1][x] != baseWorld[y][x];
            bool bottom = y < WORLD_HEIGHT - 1 && baseWorld[y + 1][x] != baseWorld[y][x];
            bool left = x > 0 && baseWorld[y][x - 1] != baseWorld[y][x];
            bool right = x < WORLD_WIDTH - 1 && baseWorld[y][x + 1] != baseWorld[y][x];

            // Determine the variant based on neighboring tiles
            if (top && left)
            {
                world[y][x].variant = EDGE_TOP_LEFT;
            }
            else if (top && right)
            {
                world[y][x].variant = EDGE_TOP_RIGHT;
            }
            else if (bottom && left)
            {
                world[y][x].variant = EDGE_BOTTOM_LEFT;
            }
            else if (bottom && right)
            {
                world[y][x].variant = EDGE_BOTTOM_RIGHT;
            }
            else if (top)
            {
                world[y][x].variant = EDGE_TOP;
            }
            else if (bottom)
            {
                world[y][x].variant = EDGE_BOTTOM;
            }
            else if (left)
            {
                world[y][x].variant = EDGE_LEFT;
            }
            else if (right)
            {
                world[y][x].variant = EDGE_RIGHT;
            }
            else
            {
                world[y][x].variant = CENTER;
            }
        }
    }
}

// Perlin noise implementation
float Map::lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float Map::dotGridGradient(int ix, int iy, float x, float y)
{
    // Precomputed gradient vectors (simplified to 4 directions)
    int hash = permutation[(permutation[ix & 255] + iy) & 255] % 4;
    float gx, gy;
    switch (hash)
    {
    case 0:
        gx = 1;
        gy = 0;
        break; // Right
    case 1:
        gx = -1;
        gy = 0;
        break; // Left
    case 2:
        gx = 0;
        gy = 1;
        break; // Up
    case 3:
        gx = 0;
        gy = -1;
        break; // Down
    default:
        gx = 0;
        gy = 0;
        break;
    }

    // Distance vector
    float dx = x - ix;
    float dy = y - iy;

    // Dot product
    return (dx * gx + dy * gy);
}

float Map::perlinNoise(float x, float y, float scale)
{
    // Scale the coordinates
    x *= scale;
    y *= scale;

    // Grid cell coordinates
    int x0 = static_cast<int>(x);
    int x1 = x0 + 1;
    int y0 = static_cast<int>(y);
    int y1 = y0 + 1;

    // Fractional part
    float sx = x - x0;
    float sy = y - y0;

    // Interpolate
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);

    value = lerp(ix0, ix1, sy);

    // Normalize to [-1, 1]
    return value;
}

Map::~Map()
{
    SDL_DestroyTexture(tilesetTexture);
}

void Map::loadTextures(SDL_Renderer *renderer)
{
    // Load the tileset
    tilesetTexture = IMG_LoadTexture(renderer, "assets/tiles.png");
    if (!tilesetTexture)
    {
        std::cerr << "Failed to load tileset.png: " << SDL_GetError() << std::endl;
        return;
    }

    // Map tileset regions to tile types and variants
    // GRASS
    // Constants

    // GRASS
    tileSources[BASE_GRASS][CENTER] = {32, 64, TILE_SIZE, TILE_SIZE};               // Grass center
    tileSources[BASE_GRASS][EDGE_TOP] = {0, 32, TILE_SIZE, TILE_SIZE};            // Grass top edge
    tileSources[BASE_GRASS][EDGE_BOTTOM] = {0, 64, TILE_SIZE, TILE_SIZE};         // Grass bottom edge
    tileSources[BASE_GRASS][EDGE_LEFT] = {0, 96, TILE_SIZE, TILE_SIZE};           // Grass left edge
    tileSources[BASE_GRASS][EDGE_RIGHT] = {0, 128, TILE_SIZE, TILE_SIZE};         // Grass right edge
    tileSources[BASE_GRASS][EDGE_TOP_LEFT] = {0, 0, TILE_SIZE, TILE_SIZE};      // Grass top-left corner
    tileSources[BASE_GRASS][EDGE_TOP_RIGHT] = {64, 96, TILE_SIZE, TILE_SIZE};     // Grass top-right corner
    tileSources[BASE_GRASS][EDGE_BOTTOM_LEFT] = {32, 128, TILE_SIZE, TILE_SIZE};  // Grass bottom-left corner
    tileSources[BASE_GRASS][EDGE_BOTTOM_RIGHT] = {64, 128, TILE_SIZE, TILE_SIZE}; // Grass bottom-right corner

    // WATER
    tileSources[BASE_WATER][CENTER] = {64, 0, TILE_SIZE, TILE_SIZE};               // Water center
    tileSources[BASE_WATER][EDGE_TOP] = {64, 32, TILE_SIZE, TILE_SIZE};            // Water top edge
    tileSources[BASE_WATER][EDGE_BOTTOM] = {64, 64, TILE_SIZE, TILE_SIZE};         // Water bottom edge
    tileSources[BASE_WATER][EDGE_LEFT] = {64, 96, TILE_SIZE, TILE_SIZE};           // Water left edge
    tileSources[BASE_WATER][EDGE_RIGHT] = {64, 128, TILE_SIZE, TILE_SIZE};         // Water right edge
    tileSources[BASE_WATER][EDGE_TOP_LEFT] = {96, 96, TILE_SIZE, TILE_SIZE};       // Water top-left corner
    tileSources[BASE_WATER][EDGE_TOP_RIGHT] = {128, 96, TILE_SIZE, TILE_SIZE};     // Water top-right corner
    tileSources[BASE_WATER][EDGE_BOTTOM_LEFT] = {96, 128, TILE_SIZE, TILE_SIZE};   // Water bottom-left corner
    tileSources[BASE_WATER][EDGE_BOTTOM_RIGHT] = {128, 128, TILE_SIZE, TILE_SIZE}; // Water bottom-right corner

    // CLIFF
    tileSources[BASE_CLIFF][CENTER] = {32, 32, TILE_SIZE, TILE_SIZE};           // Cliff center
    tileSources[BASE_CLIFF][EDGE_TOP] = {32, 64, TILE_SIZE, TILE_SIZE};         // Cliff top edge
    tileSources[BASE_CLIFF][EDGE_BOTTOM] = {32, 96, TILE_SIZE, TILE_SIZE};      // Cliff bottom edge
    tileSources[BASE_CLIFF][EDGE_LEFT] = {96, 0, TILE_SIZE, TILE_SIZE};         // Cliff left edge (hole tile used)
    tileSources[BASE_CLIFF][EDGE_RIGHT] = {96, 0, TILE_SIZE, TILE_SIZE};        // Cliff right edge
    tileSources[BASE_CLIFF][EDGE_TOP_LEFT] = {96, 0, TILE_SIZE, TILE_SIZE};     // Cliff top-left corner
    tileSources[BASE_CLIFF][EDGE_TOP_RIGHT] = {96, 0, TILE_SIZE, TILE_SIZE};    // Cliff top-right corner
    tileSources[BASE_CLIFF][EDGE_BOTTOM_LEFT] = {96, 0, TILE_SIZE, TILE_SIZE};  // Cliff bottom-left corner
    tileSources[BASE_CLIFF][EDGE_BOTTOM_RIGHT] = {96, 0, TILE_SIZE, TILE_SIZE}; // Cliff bottom-right corner
}

void Map::drawWorld(SDL_Renderer *renderer, float cam_x, float cam_y)
{
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            float dx = (x * TILE_SIZE + TILE_SIZE / 2) - cam_x;
            float dy = (y * TILE_SIZE + TILE_SIZE / 2) - cam_y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= RENDER_DISTANCE)
            {
                Tile tile = world[y][x];
                SDL_Rect dest = {
                    static_cast<int>(x * TILE_SIZE - cam_x),
                    static_cast<int>(y * TILE_SIZE - cam_y),
                    TILE_SIZE,
                    TILE_SIZE};

                // Render the appropriate tile from the tileset
                SDL_RenderCopy(renderer, tilesetTexture, &tileSources[tile.baseType][tile.variant], &dest);
            }
        }
    }
}

bool Map::isSolid(int x, int y)
{
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    if (tileX < 0 || tileX >= WORLD_WIDTH || tileY < 0 || tileY >= WORLD_HEIGHT)
    {
        return true; // Out of bounds is solid
    }
    return world[tileY][tileX].baseType == BASE_CLIFF; // Cliffs are solid
}

bool Map::isJumpable(int x, int y)
{
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    if (tileX < 0 || tileX >= WORLD_WIDTH || tileY < 0 || tileY >= WORLD_HEIGHT)
    {
        return false;
    }
    return world[tileY][tileX].baseType == BASE_WATER; // Only water is jumpable
}
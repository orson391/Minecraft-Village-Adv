#include "rain.h"
#include <cstdlib> // for rand

void rain::Rain_Init(Rain* rain, int screenWidth, int screenHeight) {
    rain->screenWidth = screenWidth;
    rain->screenHeight = screenHeight;

    for (int i = 0; i < MAX_RAIN; i++) {
        rain->drops[i].x = rand() % screenWidth;
        rain->drops[i].y = rand() % screenHeight;
        rain->drops[i].speed = 4 + rand() % 4;
        rain->drops[i].length = 10 + rand() % 5;
    }
}

void rain::Rain_Update(Rain* rain) {
    for (int i = 0; i < MAX_RAIN; i++) {
        rain->drops[i].y += rain->drops[i].speed;

        if (rain->drops[i].y > rain->screenHeight) {
            rain->drops[i].y = -rain->drops[i].length;
            rain->drops[i].x = rand() % rain->screenWidth;
        }
    }
}

void rain::Rain_Render(Rain* rain, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 200); // light blue rain color

    for (int i = 0; i < MAX_RAIN; i++) {
        SDL_RenderDrawLine(renderer,
            static_cast<int>(rain->drops[i].x),
            static_cast<int>(rain->drops[i].y),
            static_cast<int>(rain->drops[i].x),
            static_cast<int>(rain->drops[i].y + rain->drops[i].length));
    }
}

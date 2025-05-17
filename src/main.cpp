#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Minecraft-Like", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue
        SDL_RenderClear(renderer);

        // Draw dirt block (brown)
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_Rect dirt_block = {100, 100, 32, 32}; // x=100, y=100, 32x32 pixels
        SDL_RenderFillRect(renderer, &dirt_block);

        // Draw player (green)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect player = {400, 300, 32, 32}; // Center of 800x600 window
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

#include "Coregame.h"
#include "mouse.h"

SDL_Window* Coregame::window = nullptr;
SDL_Renderer* Coregame::renderer = nullptr;
TTF_Font* Coregame::font = nullptr;
bool Coregame::running = true;
SDL_Event Coregame::event;
float Coregame::deltaTime = 0.0f;

bool Coregame::initGame(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;
    if (TTF_Init() == -1) {
        printf("Failed to initialize TTF: %s\n", TTF_GetError());
        return false;
    }
    font = TTF_OpenFont("assets/arial.ttf", 24); // path and font size
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return false;
    }


    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window)
        return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        return false;

    return true;
}

void Coregame::handleEvents()
{
    // Reset click states once per frame
    mouse::LeftClick = false;
    mouse::RightClick = false;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                mouse::LeftClick = true;
                mouse::mouseX = mouseX;
                mouse::mouseY = mouseY;
                // Handle one-time click here
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                //printf("Right mouse CLICK\n");
                mouse::RightClick = true;
                // Handle right click here
            }
        }
    }
}

void Coregame::renderText(const char* text, int x, int y, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = { x, y, surface->w, surface->h };

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void Coregame::clear()
{
    SDL_RenderClear(renderer);
}

void Coregame::render()
{
    SDL_RenderPresent(renderer);
}

void Coregame::cleanGame()
{
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

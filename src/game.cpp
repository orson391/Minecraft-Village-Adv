#include "game.h"
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0, 0, 800, 640 };
bool Game::isRunning = false;
int Game::fps = 60;

Game::Game() {
}

Game::~Game() {
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int fp) {
    fps = fp;
    camera.w = width;
    camera.h = height;
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cout << "Subsystems Initialized" << std::endl;
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cout << "Window created" << std::endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::cout << "Renderer created" << std::endl;
    isRunning = true;
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) isRunning = false;
    }
}

void Game::update() {
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    if (fps > 0) SDL_Delay(1000 / fps);
}

void Game::clean() {
    if (window) SDL_DestroyWindow(window);
    if (renderer) SDL_DestroyRenderer(renderer);
    window = nullptr;
    renderer = nullptr;
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
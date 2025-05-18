#pragma once
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
class Coregame
{
public:
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static bool running;
    static SDL_Event event;
    static float deltaTime;
    static TTF_Font* font;
    static void renderText(const char* text, int x, int y, SDL_Color color);
    static bool initGame(const char* title, int width, int height);
    static void cleanGame();
    static void handleEvents();
    static void clear();
    static void render();
};

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <iostream>

class Game {
public:
    Game();                // Constructor
    ~Game();               // Destructor
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int fp);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() const { return isRunning; }

    static SDL_Renderer* renderer;
    static SDL_Event event;
    static SDL_Rect camera;

    static bool isRunning;
    static int fps;
private:
    SDL_Window* window;
};

#endif
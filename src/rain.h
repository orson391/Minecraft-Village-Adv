#pragma once
#define MAX_RAIN 200
#include <SDL2/SDL.h>

class rain
{
public:
    typedef struct {
        float x, y;
        float speed;
        int length;
    } RainDrop;

    typedef struct {
        RainDrop drops[MAX_RAIN];
        int screenWidth;
        int screenHeight;
    } Rain;

    void Rain_Init(Rain* rain, int screenWidth, int screenHeight);
    void Rain_Update(Rain* rain);
    void Rain_Render(Rain* rain, SDL_Renderer* renderer);
};

#include "tree.h"
#include "AddTexture.h"
#include "Coregame.h"


void tree::init()
{
    myobject.Texture = AddTexture::addTexture("assets/uptree.png");

    // Use the tree's actual position (x, y)s
    myobject.ObsticleRect = { x, y, 100, 128 };
    myobject.ObsticleHitbox = { myobject.ObsticleRect.x + 40, myobject.ObsticleRect.y + 90, 20, 20 }; // center trunk

    myobject.Shadow = {
        x + 30, // center minus half shadow width
        y + 85,     // near bottom of the image
        40,               // shadow width
        40                 // shadow height
    };
}

void tree::render(float cam_x, float cam_y) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    if ((x + 2 * tILE_SIZE < cam_x - tILE_SIZE) || (x > cam_x + screenWidth) ||
        (y + 2 * tILE_SIZE < cam_y - tILE_SIZE) || (y > cam_y + screenHeight)) {
        return;
    }

    SDL_Rect drawRect = { x - static_cast<int>(cam_x), y - static_cast<int>(cam_y),
                          myobject.ObsticleRect.w, myobject.ObsticleRect.h };
    SDL_Rect shadowRect = { myobject.Shadow.x - static_cast<int>(cam_x),
                           myobject.Shadow.y - static_cast<int>(cam_y),
                           myobject.Shadow.w, myobject.Shadow.h };
    SDL_Rect hitboxRect = { myobject.ObsticleHitbox.x - static_cast<int>(cam_x),
                           myobject.ObsticleHitbox.y - static_cast<int>(cam_y),
                           myobject.ObsticleHitbox.w, myobject.ObsticleHitbox.h };

    // Render shadow
    SDL_SetRenderDrawBlendMode(Coregame::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Coregame::renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(Coregame::renderer, &shadowRect);

    // Render tree
    AddTexture::drawTexture(myobject.Texture, &drawRect);

    // Render hitbox (debug)
    SDL_SetRenderDrawColor(Coregame::renderer, 0, 255, 0, 255); // Green
    SDL_RenderDrawRect(Coregame::renderer, &hitboxRect);
}



void tree::spawnNear(float player_x, float player_y) {
    int offsetX = (rand() % 201) - 100;
    int offsetY = (rand() % 201) - 100;

    x = static_cast<int>(player_x) + offsetX;
    y = static_cast<int>(player_y) + offsetY;

    init(); // set rects based on new position
}

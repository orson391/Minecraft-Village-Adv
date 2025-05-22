#include "tree.h"
#include "AddTexture.h"
#include "Coregame.h"
#include <vector>
#include "ItemDrop.h"

extern std::vector<ItemDrop>& getItemDrops();

void tree::init()
{
    myobject.health = myobject.maxHealth; // Initialize health to max
    myobject.isAlive = true;
    myobject.showHealthBar = false;
    myobject.healthBarTimer = 0.0f;
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

void tree::takeDamage(int damage) {
    if (!myobject.isAlive) return;

    myobject.health -= damage;
    myobject.showHealthBar = true; // Show health bar on hit
    myobject.healthBarTimer = 3.0f; // Display for 3 seconds
    if (myobject.health <= 0) {
        myobject.health = 0;
        myobject.isAlive = false;
        printf("rock broke\n");
         // Drop item
        SDL_Texture* itemTexture = AddTexture::addTexture("assets/Raw_Mutton_JE3_BE2.png");
        //ItemDrop drop(hitbox.x, hitbox.y, "Meat", itemTexture, 1);
        //printf("Item Droped at %d %d\n", hitbox.x, hitbox.y);
        ItemDrop drop(myobject.ObsticleHitbox.x,myobject.ObsticleHitbox.y, "Wood", itemTexture, 6);
        //printf("Item Droped at %f %f\n",x,y);
        
        getItemDrops().push_back(drop);
    }
}

void tree::render(float cam_x, float cam_y) {

    if (!myobject.isAlive) return;
    // Update health bar timer
    if (myobject.showHealthBar) {
        myobject.healthBarTimer -= Coregame::deltaTime;
        if (myobject.healthBarTimer <= 0.0f) {
            myobject.showHealthBar = false;
        }
    }
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

    if (myobject.showHealthBar) {
        // Health bar dimensions
        const int barWidth = 30; // Same width as animal
        const int barHeight = 4; // Thin bar
        const int barOffsetY = -70; // 10 pixels above animal

        // Background (red, full width)
        SDL_Rect bgRect = {
            hitboxRect.x,
            hitboxRect.y + barOffsetY,
            barWidth,
            barHeight
        };
        SDL_SetRenderDrawColor(Coregame::renderer, 255,100, 50, 255);
        SDL_RenderFillRect(Coregame::renderer, &bgRect);

        // Foreground (green, proportional to health)
        int fillWidth = static_cast<int>((static_cast<float>(myobject.health) / myobject.maxHealth) * barWidth);
        SDL_Rect fillRect = {
            hitboxRect.x,
            hitboxRect.y + barOffsetY,
            fillWidth,
            barHeight
        };
        SDL_SetRenderDrawColor(Coregame::renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(Coregame::renderer, &fillRect);
    }


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

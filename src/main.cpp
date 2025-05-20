#define SDL_MAIN_HANDLED

#include "Coregame.h"
#include "Player.h"
#include "AddTexture.h"
#include "Map.h"
#include "camara.h"
#include "Keyevent.h"
#include "animal.h"
#include "collision.h"
#include "tree.h"
#include "rain.h"
#include "rock.h"
#include "inventory.h"
#include "ItemDrop.h"

#include <vector>
#include <iostream>

// Global item drops
std::vector<ItemDrop> itemDrops;

std::vector<ItemDrop> &getItemDrops()
{
    return itemDrops;
}

int main(int argc, char *argv[])
{
    if (!Coregame::initGame("My Game", WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        return -1;
    }

    // test

    // Initialize Inventory
    inventory inv;

    // Initialize Player
    Player player(WORLD_WIDTH * TILE_SIZE / 2.0f, WORLD_HEIGHT * TILE_SIZE / 2.0f, 32, 32);

    // Initialize Map
    Map map;
    map.loadTextures(Coregame::renderer);

    // Initialize Environment (Tree, Rock)
    tree mytree;
    mytree.spawnNear(player.player_x, player.player_y);

    rock myrock;
    myrock.spawnNear(player.player_x, player.player_y);

    // Initialize Rain System
    rain rainSystem;
    rain::Rain rainData;
    rainSystem.Rain_Init(&rainData, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize Camera
    camara cam;
    cam.camara_x = player.player_x - WINDOW_WIDTH / 2.0f;
    cam.camara_y = player.player_y - WINDOW_HEIGHT / 2.0f;

    // Initialize Animal
    animal myanimal(player.player_x + 50, player.player_y + 50); // Spawn near the player

    // Timing
    Uint32 lastTime = SDL_GetTicks();
    const float targetFrameTime = 1.0f / 60.0f;

    // Main Game Loop
    while (Coregame::running)
    {
        Uint32 startTime = SDL_GetTicks();
        float deltaTime = (startTime - lastTime) / 1000.0f;
        Coregame::deltaTime = deltaTime;
        lastTime = startTime;

        // --- INPUT HANDLING ---
        Coregame::handleEvents();
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        static bool zoomInPressed = false;
        static bool zoomOutPressed = false;

        inv.handleInput(Coregame::event);

        if ((state[SDL_SCANCODE_EQUALS] || state[SDL_SCANCODE_KP_PLUS]) && !zoomInPressed)
        {
            cam.setZoom(cam.zoom + 0.02f);
            zoomInPressed = true;
        }
        else if (!(state[SDL_SCANCODE_EQUALS] || state[SDL_SCANCODE_KP_PLUS]))
        {
            zoomInPressed = false;
        }

        if ((state[SDL_SCANCODE_MINUS] || state[SDL_SCANCODE_KP_MINUS]) && !zoomOutPressed)
        {
            cam.setZoom(cam.zoom - 0.02f);
            zoomOutPressed = true;
        }
        else if (!(state[SDL_SCANCODE_MINUS] || state[SDL_SCANCODE_KP_MINUS]))
        {
            zoomOutPressed = false;
        }

        // --- UPDATE LOGIC ---
        cam.update(player.player_x, player.player_y, deltaTime);
        SDL_RenderSetScale(Coregame::renderer, cam.zoom, cam.zoom);

        myanimal.update(deltaTime, player.player_x, player.player_y, cam.camara_x, cam.camara_y);
        player.update(cam.camara_x, cam.camara_y, mytree, myrock, myanimal);

        for (auto &drop : itemDrops)
        {
            drop.update(deltaTime);
        }

        for (auto &drop : itemDrops)
        {
            float dx = player.player_x - drop.x;
            float dy = player.player_y - drop.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 32)
            {
                //std::cout << "Item is nearby!\n";
                if (drop.pickUp(inv))
                {
                    printf("Picked up %s\n", drop.getName().c_str());
                }
            }
            // if (collision::checkCollision(&player.rect, &drop.getHitbox()))
            //{

            //}
        }

        rainSystem.Rain_Update(&rainData);

        // --- RENDERING ---
        Coregame::clear();

        map.drawWorld(Coregame::renderer, cam.camara_x, cam.camara_y);
        mytree.render(cam.camara_x, cam.camara_y);
        myrock.render(cam.camara_x, cam.camara_y);
        myanimal.draw(Coregame::renderer, cam.camara_x, cam.camara_y);
        player.renderCharacter(Coregame::renderer, cam.camara_x, cam.camara_y);
        rainSystem.Rain_Render(&rainData, Coregame::renderer);

        for (auto &drop : itemDrops)
        {
            drop.draw(Coregame::renderer, cam.camara_x, cam.camara_y);
        }
        // test

        // std::cout << "Player: (" << player.rect.x << ", " << player.rect.y << ")\n";
        // std::cout << "Item: (" << my.hitbox.x << ", " << my.hitbox.y << ")\n";

        // if (collision::checkCollision(&player.rect, &my.hitbox))
        //{
        //     inv.addItem(my.name, my.texture, 1);

        //}

        inv.render(Coregame::renderer, Coregame::font);

        SDL_RenderSetScale(Coregame::renderer, 1.0f, 1.0f); // Reset scaling
        Coregame::render();

        // --- FRAME DELAY FOR STABLE FPS ---
        Uint32 frameTime = SDL_GetTicks() - startTime;
        float delay = targetFrameTime - (frameTime / 1000.0f);
        if (delay > 0)
        {
            SDL_Delay(static_cast<Uint32>(delay * 1000.0f));
        }
    }

    Coregame::cleanGame();
    return 0;
}

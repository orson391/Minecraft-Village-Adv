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



int main(int argc, char* argv[])
{
    if (!Coregame::initGame("My Game", WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return -1;
    }

    inventory myinvent;
    

    // Initialize Player
    Player player(WORLD_WIDTH * TILE_SIZE / 2.0f, WORLD_HEIGHT * TILE_SIZE / 2.0f, 32, 32);

    // Initialize Map
    Map map;
    map.loadTextures(Coregame::renderer);

    //tree
    tree mytree;
    //mytree.init();
    mytree.spawnNear(player.player_x, player.player_y);

    rock myrock;
    myrock.spawnNear(player.player_x, player.player_y);

    //rain
    rain rainSystem;
    rain::Rain rainData;

    rainSystem.Rain_Init(&rainData, WINDOW_WIDTH, WINDOW_HEIGHT);
    

    
    
    // Initialize Camera
    camara cam;
    cam.camara_x = player.player_x - WINDOW_WIDTH / 2.0f;
    cam.camara_y = player.player_y - WINDOW_HEIGHT / 2.0f;

    //
    

    // Initialize Animal
    animal myanimal(player.player_x + 50, player.player_y + 50); // Near the player
    myanimal.skin = AddTexture::addTexture("assets/sheep2.png");




    myinvent.addItem("Health Potion", myanimal.skin, 5);




    // Game loop timing
    Uint32 lastTime = SDL_GetTicks();

    const float targetFrameTime = 1.0f / 60.0f;

    while (Coregame::running) {
        Uint32 startTime = SDL_GetTicks();
        float deltaTime = (startTime - lastTime) / 1000.0f;
        //if (deltaTime < 0.0001f) deltaTime = 1.0f / 60.0f; // Fallback to ~60 FPS
        Coregame::deltaTime = deltaTime; // Sync with Coregame::deltaTime
        lastTime = startTime;

        Coregame::handleEvents(); // 1. Handle input
        // Handle Zoom Input
        const Uint8* state = SDL_GetKeyboardState(NULL);
        static bool zoomInPressed = false;
        static bool zoomOutPressed = false;
        
        myinvent.handleInput(Coregame::event);

        if ((state[SDL_SCANCODE_EQUALS] || state[SDL_SCANCODE_KP_PLUS])) {
            if (!zoomInPressed) {
                cam.setZoom(cam.zoom + 0.02f);
                zoomInPressed = true;
            }
        }
        else {
            zoomInPressed = false;
        }

        if ((state[SDL_SCANCODE_MINUS] || state[SDL_SCANCODE_KP_MINUS])) {
            if (!zoomOutPressed) {
                cam.setZoom(cam.zoom - 0.02f);
                zoomOutPressed = true;
            }
        }
        else {
            zoomOutPressed = false;
        }


        Coregame::clear();        // 2. Clear frame



        
        

        cam.update(player.player_x, player.player_y,deltaTime); // 3. Update camera
        SDL_RenderSetScale(Coregame::renderer, cam.zoom, cam.zoom);

        map.drawWorld(Coregame::renderer, cam.camara_x, cam.camara_y); // 4. Draw map

        mytree.render(cam.camara_x,cam.camara_y);
        myrock.render(cam.camara_x, cam.camara_y);

        myanimal.update(deltaTime, player.player_x, player.player_y, cam.camara_x, cam.camara_y); // 5. Update animal

        player.update(cam.camara_x, cam.camara_y, mytree,myrock);

        // Render player using custom renderCharacter method
        player.renderCharacter(Coregame::renderer,cam.camara_x,cam.camara_y); // 7. Render player


        myinvent.render(Coregame::renderer, Coregame::font);


        // Render animal
        AddTexture::drawTexture(myanimal.skin, &myanimal.rect); // 8. Draw animal

       
        SDL_Color white = { 255, 255, 255, 255 };
        Coregame::renderText("Inventory", cam.camara_x, cam.camara_y, white);

        
        

        //  rainfull:
        rainSystem.Rain_Update(&rainData);
        rainSystem.Rain_Render(&rainData, Coregame::renderer);

        

        


        //dont touch
        SDL_RenderSetScale(Coregame::renderer, 1.0f, 1.0f);

        Coregame::render(); // 9. Render frame


        Uint32 frameTime = SDL_GetTicks() - startTime;
        float delay = targetFrameTime - (frameTime / 1000.0f);
        if (delay > 0) {
            SDL_Delay(static_cast<Uint32>(delay * 1000.0f));
        }
    }

    Coregame::cleanGame();
    return 0;
}
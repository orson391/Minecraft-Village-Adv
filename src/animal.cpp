#include "animal.h"

animal::animal(float startX, float startY) {
	x = startX;
	y = startY;
	ve_x = ve_y = 0;
	state = animalState::Idle;
	stateTimer = 2.0f;
}
void animal::update(float deltaTime, float playerX, float playerY, float cam_x, float cam_y)
{
    float dx = x - playerX;
    float dy = y - playerY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 10.f && state != animalState::Flee)
    {
        state = animalState::Flee;
        float angle = std::atan2(dy, dx);
        ve_x = std::cos(angle) * 150.0f;
        ve_y = std::sin(angle) * 150.0f;
        stateTimer = 1.5f;
    }

    stateTimer -= deltaTime;

    switch (state) {
    case animalState::Idle:
        ve_x = ve_y = 0;
        if (stateTimer <= 0) {
            state = animalState::Wander;
            float angle = static_cast<float>(rand() % 360) * M_PI / 180.0f;
            ve_x = std::cos(angle) * 50.0f;
            ve_y = std::sin(angle) * 50.0f;
            stateTimer = 3.0f;
        }
        break;

    case animalState::Wander:
        if (stateTimer <= 0) {
            state = animalState::Idle;
            ve_x = ve_y = 0;
            stateTimer = 2.0f;
        }
        break;

    case animalState::Flee:
        if (stateTimer <= 0) {
            state = animalState::Idle;
            ve_x = ve_y = 0;
            stateTimer = 2.0f;
        }
        break;
    }

    x += ve_x * deltaTime;
    y += ve_y * deltaTime;

    // Update the rect for rendering, considering the camera offset
    rect = { static_cast<int>(x - cam_x), static_cast<int>(y - cam_y), 32, 32 };
}

//void animal::update(float deltaTime, float playerX, float playerY, float cam_x, float cam_y)
//{
//    float dx = x - playerX;
//    float dy = y - playerY;
//    float distance = std::sqrt(dx * dx + dy * dy);
//
//    if (distance < 10.f && state != animalState::Flee)
//    {
//        state = animalState::Flee;
//        float angle = std::atan2(dy, dx);
//        ve_x = std::cos(angle) * 150.0f;
//        ve_y = std::sin(angle) * 150.0f;
//        stateTimer = 1.5f;
//    }
//
//    stateTimer -= deltaTime;
//
//    switch (state) {
//    case animalState::Idle:
//        ve_x = ve_y = 0;
//        if (stateTimer <= 0) {
//            state = animalState::Wander;
//            float angle = static_cast<float>(rand() % 360) * M_PI / 180.0f;
//            ve_x = std::cos(angle) * 50.0f;
//            ve_y = std::sin(angle) * 50.0f;
//            stateTimer = 3.0f;
//        }
//        break;
//
//    case animalState::Wander:
//        if (stateTimer <= 0) {
//            state = animalState::Idle;
//            ve_x = ve_y = 0;
//            stateTimer = 2.0f;
//        }
//        break;
//
//    case animalState::Flee:
//        if (stateTimer <= 0) {
//            state = animalState::Idle;
//            ve_x = ve_y = 0;
//            stateTimer = 2.0f;
//        }
//        break;
//    }
//
//    x += ve_x * deltaTime;
//    y += ve_y * deltaTime;
//
//    // This is the important part:
//    rect = { static_cast<int>(x - cam_x), static_cast<int>(y - cam_y), 32, 32 };
//}


void animal::draw(SDL_Renderer* renderer)
{
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), 20, 20 };
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // brown
    SDL_RenderFillRect(renderer, &rect);
}

void animal::drawimage()
{
    
}
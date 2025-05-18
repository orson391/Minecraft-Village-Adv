#include "camara.h"

camara::camara(float initialZoom) {
    setZoom(initialZoom);
}

void camara::update(float target_x, float target_y, float deltaTime) {
    // Calculate view dimensions based on zoom
    float viewWidth = WINDOW_WIDTH / zoom;
    float viewHeight = WINDOW_HEIGHT / zoom;

    // Compute target camera position (centered on target)
    float targetCamX = target_x - viewWidth / 2.0f;
    float targetCamY = target_y - viewHeight / 2.0f;

    // Smoothly interpolate current position toward target
    camara_x += (targetCamX - camara_x) * smoothingSpeed * deltaTime;
    camara_y += (targetCamY - camara_y) * smoothingSpeed * deltaTime;

    // Cache world boundaries
    static constexpr float worldWidth = WORLD_WIDTH * TILE_SIZE;
    static constexpr float worldHeight = WORLD_HEIGHT * TILE_SIZE;

    // Clamp camera position to world bounds
    camara_x = std::clamp(camara_x, 0.0f, worldWidth - viewWidth);
    camara_y = std::clamp(camara_y, 0.0f, worldHeight - viewHeight);
}

void camara::setZoom(float newZoom) {
    // Clamp zoom to prevent extreme values
    zoom = std::clamp(newZoom, minZoom, maxZoom);
}
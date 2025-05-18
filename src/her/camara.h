#pragma once
#include "Map.h"
#include "Coregame.h"
#include <algorithm> // For std::clamp

class camara {
public:
    camara(float initialZoom = 1.0f);
    void update(float target_x, float target_y, float deltaTime);
    void setZoom(float newZoom);

    float camara_x = 0.0f;
    float camara_y = 0.0f;
    float zoom = 1.0f;

private:
    static constexpr float smoothingSpeed = 5.0f; // Adjust for desired smoothness
    static constexpr float minZoom = 0.1f;
    static constexpr float maxZoom = 5.0f;
};
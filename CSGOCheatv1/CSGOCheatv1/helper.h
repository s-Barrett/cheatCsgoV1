#include <iostream>
#include <Windows.h>
#include "datatypes.h"

// Constants for screen dimensions
const float SCREEN_WIDTH = 1920.0f;
const float SCREEN_HEIGHT = 1080.0f;

// Converts screen coordinates to normalized device coordinates (-1 to 1 range)
void ConvertToRange(Vec2& point) {
    // Normalize X coordinate
    point.x /= SCREEN_WIDTH;
    point.x *= 2.0f;
    point.x -= 1.0f;

    // Normalize Y coordinate 
    point.y /= SCREEN_HEIGHT;
    point.y *= 2.0f;
    point.y -= 1.0f;
}

// Projects 3D world coordinates to 2D screen coordinates using view matrix
bool WorldToScreen(const Vec3& vecOrigin, Vec2& vecScreen, float* matrix) {
    // Calculate screen coordinates using matrix multiplication
    vecScreen.x = vecOrigin.x * matrix[0] + vecOrigin.y * matrix[1] + vecOrigin.z * matrix[2] + matrix[3];
    vecScreen.y = vecOrigin.x * matrix[4] + vecOrigin.y * matrix[5] + vecOrigin.z * matrix[6] + matrix[7];
    
    // Calculate W component for perspective division
    float w = vecOrigin.x * matrix[12] + vecOrigin.y * matrix[13] + vecOrigin.z * matrix[14] + matrix[15];

    // Check if point is behind camera
    if (w < 0.0f)
        return false;

    // Perform perspective divide to get normalized device coordinates
    Vec2 ndc;
    ndc.x = vecScreen.x / w;
    ndc.y = vecScreen.y / w;

    // Convert to screen coordinates
    vecScreen.x = (SCREEN_WIDTH / 2 * ndc.x) + (ndc.x + SCREEN_WIDTH / 2);
    vecScreen.y = (SCREEN_HEIGHT / 2 * ndc.y) + (ndc.y + SCREEN_HEIGHT / 2);

    // Convert to normalized range
    ConvertToRange(vecScreen);
    return true;
}

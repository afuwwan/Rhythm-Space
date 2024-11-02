#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Setting.h"

class Camera {
public:
    glm::vec2 position;  // Camera's position in 2D
    float zoom;          // Camera zoom level

    Camera();                       // Constructor
    glm::mat4 GetViewMatrix(float screenWidth, float screenHeight) const;  // Method to get the view matrix
    void Follow(const glm::vec2& target);  // Method to set camera position
    void SetZoom(float newZoom);  // Method to set the zoom level
private:


};

#endif

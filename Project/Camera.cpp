#include "Camera.h"

Camera::Camera() : position(0.0f, 0.0f), zoom(1.0f) 
{


}

// Returns the view matrix based on the camera position and zoom level
glm::mat4 Camera::GetViewMatrix(float screenWidth, float screenHeight) const 
{
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
    //return glm::scale(view, glm::vec3(zoom, zoom, 1.0f));

    // Center the zoom around the position by scaling around it
    //glm::vec3 scale(zoom, zoom, 1.0f);
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
    //return glm::scale(view, scale);

    //glm::vec2 adjustedPosition = position - (0.5f * (1.0f - 1.0f / zoom) * glm::vec2(800, 600));
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-adjustedPosition, 0.0f));
    //return glm::scale(view, glm::vec3(zoom, zoom, 1.0f));

    // World coordinates at the center of the screen
    //glm::vec2 center(screenWidth * 0.5f, screenHeight / 0.5f);
    //glm::vec2 adjustedPosition = position - (center - position) * (1.0f - 1.0f / zoom);

    // Translate and scale the view matrix
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-adjustedPosition, 0.0f));
    //return glm::scale(view, glm::vec3(zoom, zoom, 1.0f));

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));

    // Translate to center, scale, then translate back
    view = glm::translate(view, glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.0f));
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    view = glm::translate(view, glm::vec3(-screenWidth / 2.0f, -screenHeight / 2.0f, 0.0f));

    return view;
}

// Sets the camera to follow the target position
void Camera::Follow(const glm::vec2& target) 
{
    position = target;
}

// Sets a new zoom level
void Camera::SetZoom(float newZoom) 
{
    zoom = newZoom;

}

void Camera::SmoothFollow(const glm::vec2& targetPos, float speed, float deltaTime) {
    position += (targetPos - position) * speed * deltaTime;
}

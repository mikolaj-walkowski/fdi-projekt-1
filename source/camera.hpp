#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
    public:
    glm::vec2 position = glm::vec2(0,0);
    float scale = 1;

    void update(float moveAmount, float scaleAmount);
    void reset();
    glm::vec2 transform(glm::vec2 x, glm::vec2 fboSize);
};

#endif
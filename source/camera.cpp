#include "camera.hpp"
#include <imgui.h>

bool isKeyDown(ImGuiKey key)
{
    return ImGui::IsKeyDown(ImGui::GetKeyIndex(key));
}

void Camera::update(float moveAmount, float scaleAmount)
{
    if(isKeyDown(ImGuiKey_DownArrow)) position.y += moveAmount/scale;
    if(isKeyDown(ImGuiKey_UpArrow)) position.y -= moveAmount/scale;
    if(isKeyDown(ImGuiKey_LeftArrow)) position.x -= moveAmount/scale;
    if(isKeyDown(ImGuiKey_RightArrow)) position.x += moveAmount/scale;
    if(isKeyDown(ImGuiKey_Z)) scale *= scaleAmount;
    if(isKeyDown(ImGuiKey_C)) scale /= scaleAmount;
    if(isKeyDown(ImGuiKey_Home)) reset();
}

void Camera::reset()
{
    position = glm::vec2(0,0);
    scale = 1;
}

glm::vec2 Camera::transform(glm::vec2 x, glm::vec2 fboSize)
{
    return (x - 0.5f*fboSize - position) * scale + 0.5f*fboSize;
}
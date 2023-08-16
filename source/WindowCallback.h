#pragma once
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class WindowCallback {
public:
    WindowCallback() {
        windowCallbackInstance = this;
    }
    // Static wrappers for callbacks
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        windowCallbackInstance->framebufferSizeCallback(window, width, height);
    }
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        windowCallbackInstance->mouseCallback(window, xpos, ypos);
    }
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        windowCallbackInstance->scrollCallback(window, xoffset, yoffset);
    }
    static void SetGLFWCallbacks(GLFWwindow* window){
        glfwSetFramebufferSizeCallback(window, WindowCallback::FramebufferSizeCallback);
        glfwSetCursorPosCallback(window, WindowCallback::MouseCallback);
        glfwSetScrollCallback(window, WindowCallback::ScrollCallback);
    }
    // Getters
    glm::mat4 GetMouseRotation() {
        return mouseRotation;
    }
    glm::mat4 GetScrollModelScale() {
        return scrollModelScale;
    }
private:
    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
    void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        //return if using IMGUI
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        //static variables persists across all function calls and doesn't go out of range.
        static double last_xpos = xpos;
        static double last_ypos = ypos;
        double dx = xpos - last_xpos;
        double dy = ypos - last_ypos;
        last_xpos = xpos;
        last_ypos = ypos;

        glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS)
        {
            xVelocity = 0.0025f * static_cast<float>(dx);
            yVelocity = 0.0025f * static_cast<float>(dy);
            //rotation
            /*
            glm::quat rotY = glm::angleAxis(0.0025f * static_cast<float>(dy), cameraRight);
            glm::quat rotX = glm::angleAxis(0.0025f * static_cast<float>(dx), cameraUp);
            glm::quat rotation = rotX * rotY;

            //!!! mouseRotation *= glm::mat4_cast(rotation) ; -> mouseRotation = mouseRotation*glm::mat4_cast(rotation); which will cause rotations to apply in model space!!!
            mouseRotation = glm::mat4_cast(rotation) * mouseRotation;
            */
        }
        xVelocity *= friction;
        yVelocity *= friction;

        if (std::abs(xVelocity) > 0.001f || std::abs(yVelocity) > 0.001f) {
            //glm::quat rotY = glm::angleAxis(xVelocity, cameraRight);
            glm::quat rotY = glm::angleAxis(yVelocity, cameraRight);
            //glm::quat rotX = glm::angleAxis(yVelocity, cameraUp);
            glm::quat rotX = glm::angleAxis(xVelocity, cameraUp);
            glm::quat rotation = rotX * rotY;

            mouseRotation = glm::mat4_cast(rotation) * mouseRotation;
        }

    }
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        float scale_factor = 1.0f + yoffset * 0.1f;
        // scale matrix by x y z factors
        scrollModelScale = glm::scale(scrollModelScale, glm::vec3(scale_factor));

    };


    
    static WindowCallback* windowCallbackInstance;
    float xVelocity = 0.0f;
    float yVelocity = 0.0f;
    float friction = 0.96f;
    glm::mat4 mouseRotation{1.0f};
    glm::mat4 scrollModelScale{1.0f};

};
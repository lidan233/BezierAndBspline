//
// Created by 李源 on 2020-06-17.
//

#ifndef LEARNOPENGL_CALLBACK_H
#define LEARNOPENGL_CALLBACK_H


#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height) ;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos ;
        lastY = ypos ;
        firstMouse = false ;

    }

    float xoffset = xpos - lastX ;
    float yoffset = lastY - ypos  ;

    lastX = xpos ;
    lastY = ypos ;

    camera.ProcessMouseMovement(xoffset,yoffset) ;

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset) ;
}
void processInput(GLFWwindow *window)
{

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


    if(glfwGetKey(window,GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window,true) ;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


#endif //LEARNOPENGL_CALLBACK_H

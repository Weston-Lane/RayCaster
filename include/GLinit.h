#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int glInit(GLFWwindow** window);
#include <iostream>
#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logger.h"

int main() {
    // Initialize GLFW

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello GLFW", NULL, NULL);
    if (!window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR( "Failed to initialize Glad");
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back buffers
        glfwSwapInterval(2);//need double buffer or screen will flicker
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }


    // Clean up
    glfwTerminate();
    return 0;
}
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello GLFW", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize Glad" << std::endl;
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
      
  std::wcout << "1) ✊\n";
  std::wcout << "2) ✋\n";
  std::wcout << "3) ✌️\n";

    // Clean up
    glfwTerminate();
    return 0;
}
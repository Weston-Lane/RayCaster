#pragma once
#include <iostream>
#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "logger.h"
#include "defines.h"
#include "mathUtils.h"

const char* vertexSourcePlayer = R"(
    #version 330 core
    layout (location=0) in vec3 aPos;
    uniform vec3 translate;
    out vec4 vertColor;
    void main()
    {
        vec3 new=aPos+translate;
        gl_Position=vec4(new,1);
        vertColor=vec4(1,.5,0,1);
    }
)";
const char* fragmentSourcePlayer = R"(
    #version 330 core
    in vec4 vertColor;
    out vec4 fragColor;
    void main()
    {
        fragColor=vertColor;
    }
)";
const char* vertexSourceMap = R"(
    #version 330 core
    layout (location=1) in vec3 aPos;
    layout (location=2) in vec3 aColor;
    out vec3 vertColor;
    void main()
    {
        gl_Position=vec4(aPos,1);
        vertColor=aColor;
    }
)";
const char* fragmentSourceMap = R"(
    #version 330 core
    in vec3 vertColor;
    out vec4 fragColor;
    void main()
    {
        fragColor.a=vertColor.x;
        fragColor=vec4(vertColor,vertColor.x);
    }
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
int glInit(GLFWwindow** window)
{

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", nullptr, nullptr);
    if (!*window) {
        LOG_ERROR("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(*window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD\n");
        return -1;
    }
     glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

    // Initial viewport setup

    // Set viewport and register resize callback
    glViewport(0,0,WIDTH,HEIGHT);
    glClearColor(.5,.5,.5,1);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const char* version=(const char*)glGetString(GL_VERSION);
    LOG_INFO("OpenGL Version: "+std::string(version)+" Initialized");
    return 1;
}
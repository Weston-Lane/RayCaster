#include <iostream>
#include <Windows.h>
#include <vector>

#include "logger.h"
#include "defines.h"
#include "renderer.h"
#include "GLinit.h"
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    const char* typeStr;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               typeStr = "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "UNDEFINED BEHAVIOR"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "PORTABILITY WARNING"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "PERFORMANCE ISSUE"; break;
        case GL_DEBUG_TYPE_MARKER:              typeStr = "MARKER"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "PUSH GROUP"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "POP GROUP"; break;
        case GL_DEBUG_TYPE_OTHER:               typeStr = "OTHER"; break;
        default:                                typeStr = "UNKNOWN"; break;
    }

    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        {
            if(source==33350) break;
            LOG_FATAL("GL CALLBACK=> Source:",source," IDs:",id,"|| type:",typeStr," ",severity," ", message);
        }break;        
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            LOG_ERROR("GL CALLBACK=> Source:",source," IDs:",id,"|| type:",typeStr," ",severity," ", message);
        }break;      
        case GL_DEBUG_SEVERITY_LOW:
        {
            LOG_WARN("GL CALLBACK=> Source:",source," IDs:",id,"|| type:",typeStr," ",severity," ", message);
        }break;         
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            LOG_INFO("GL CALLBACK=> Source:",source," IDs:",id,"|| type:",typeStr," ",severity," ", message);
        }break;
        default:
        {
            LOG_ERROR("GL CALLBACK: UNKNOWN");
        }break;
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
int glInit(GLFWwindow** window)
{

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    #if GL_LOG_ENABLED==1
    glDebugMessageCallback(MessageCallback, 0);
    #endif
    const char* version=(const char*)glGetString(GL_VERSION);
    LOG_INFO("OpenGL Version: "+std::string(version)+" Initialized");
    return 1;
}
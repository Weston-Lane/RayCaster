#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "defines.h"
class VertexBuffer
{
    public:
        VertexBuffer();
        VertexBuffer(const void* data, ui64 size,GLuint mode);
        ~VertexBuffer();
        
        void BufferData(const void* data, ui64 size,GLuint mode);
        void Bind() const;
        void UnBind() const;
    private:
        unsigned int m_rendererID;
};
#pragma once
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "defines.h"
#include "VertexBuffer.h"
#include "mathUtils.h"
#include "logger.h"
struct VertexBufferElement
{
    ui32 count;
    ui32 type;
    unsigned int normalized;
    ui32 offset;
};
class VertexBufferLayout
{
    public:
        VertexBufferLayout();

        template<typename T>
        void Push(ui32 count, ui32 normalized, ui32 offset)
        {
            LOG_FATAL("not a valid type for Buffer Layout");
        }
        
    private:
        std::vector<VertexBufferElement> m_Elements;
};
class VertexArray
{
    public:
        VertexArray();
        ~VertexArray();
        
        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    private:

};

template<> inline
void VertexBufferLayout::Push<float>(ui32 count, ui32 normalized, ui32 offset)
{
    m_Elements.push_back({count,GL_FLOAT, GL_FALSE, offset});
}
template<> inline
void VertexBufferLayout::Push<unsigned int>(ui32 count, ui32 normalized, ui32 offset)
{
    m_Elements.push_back({count,GL_UNSIGNED_INT, GL_FALSE, offset});
}
template<> inline
void VertexBufferLayout::Push<int>(ui32 count, ui32 normalized, ui32 offset)
{
    m_Elements.push_back({count,GL_UNSIGNED_INT, GL_FALSE, offset});
}
template<> inline
void VertexBufferLayout::Push<unsigned char>(ui32 count, ui32 normalized, ui32 offset)
{
    m_Elements.push_back({count,GL_UNSIGNED_INT, GL_TRUE, offset});
}

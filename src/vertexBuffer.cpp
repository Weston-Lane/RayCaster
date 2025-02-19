#include "vertexBuffer.h"

#include "defines.h"
VertexBuffer::VertexBuffer(const void* data, ui64 size,GLuint mode)
{
    glGenBuffers(1,&m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER,m_rendererID);
    glBufferData(GL_ARRAY_BUFFER,size,data,mode);
}
VertexBuffer::VertexBuffer()
{
    glGenBuffers(1,&m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER,m_rendererID);
}
void VertexBuffer::BufferData(const void* data, ui64 size, GLuint mode)
{
    glBufferData(GL_ARRAY_BUFFER,size,data,mode);
}
VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1,&m_rendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER,m_rendererID);
}
void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
}
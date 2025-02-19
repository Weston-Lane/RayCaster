#include "indexBuffer.h"
#include "logger.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1,&m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_rendererID);    
}
IndexBuffer::IndexBuffer(const void* data, unsigned int amountOfInd,GLuint mode)
:m_amount(amountOfInd)
{
    if(sizeof(unsigned int)!=sizeof(GLuint)){LOG_ERROR("GLuint is not the size of unsigned int");}
    glGenBuffers(1,&m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,amountOfInd*sizeof(unsigned int),data,GL_STATIC_DRAW);
}
void IndexBuffer::BufferData(const void* data, unsigned int amountOfInd,GLuint mode)
{
    m_amount=amountOfInd;
    if(sizeof(unsigned int)!=sizeof(GLuint)){LOG_ERROR("GLuint is not the size of unsigned int");}
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,amountOfInd*sizeof(unsigned int),data,GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
   glDeleteBuffers(1,&m_rendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_rendererID);
}
void IndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
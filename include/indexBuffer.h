#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class IndexBuffer
{
    public:
        IndexBuffer();
        IndexBuffer(const void* data, unsigned int amountOfInd,GLuint mode);
        ~IndexBuffer();

        void BufferData(const void* data, unsigned int amountOfInd,GLuint mode);//if obj drops out of scope the destructor will delete the buffer so this is needed for buffer obj created outside of function
        void Bind() const;
        void UnBind() const;
        inline unsigned int getCount() const { return m_amount;}
    private:
        unsigned int m_rendererID;
        unsigned int m_amount;
};
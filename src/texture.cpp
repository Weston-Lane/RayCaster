#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "logger.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION

Texture2D::Texture2D(const std::string& path)
    :m_rendererID(0), m_filePath(path), m_localBuffer(0), m_width(0), m_height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    m_localBuffer=stbi_load(path.c_str(),&m_width,&m_height,&m_BPP,4);
    if (!m_localBuffer) 
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
    } 
    else 
    {
        std::cout << "Loaded texture: " << path << " (" << m_width << "x" << m_height << "), Channels: " << 4 << std::endl;
    }

    glGenTextures(1,&m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//may need to change to gl_nearest
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//mag filter for larger area than text
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);//if texture foes outside of area
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_localBuffer);
    glBindTexture(GL_TEXTURE_2D,0);

    if(m_localBuffer){stbi_image_free(m_localBuffer);}
}
Texture2D::~Texture2D()
{
    glDeleteTextures(1,&m_rendererID);
    
}

void Texture2D::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    
}
void Texture2D::UnBind()
{

}


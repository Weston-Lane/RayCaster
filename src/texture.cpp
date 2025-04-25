#include "texture.h"
#include "shaderUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "logger.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION

Texture2D::Texture2D(const std::string& path)
    :m_rendererID(0), m_filePath(path), m_localBuffer(0), m_width(0), m_height(0), m_BPP(0)
{
    //stbi_set_flip_vertically_on_load(1);
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
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);//may need to change to gl_nearest
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//mag filter for larger area than text
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);//if texture foes outside of area
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_localBuffer);
    glBindTexture(GL_TEXTURE_2D,0);
    
    //do not want to free texture to load into texture array
    //if(m_localBuffer){stbi_image_free(m_localBuffer);}
    
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
/*-------------------------------TEXTURE ARRAY--------------------------------------------------------------------------*/
TextureArray2D::TextureArray2D(Texture2D* textures, int width, int height, int numLayers)
    :m_rendererID(0), m_layerCount(numLayers), m_width(width), m_height(height)
{
    glGenTextures(1,&m_rendererID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_rendererID);

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 
                    1,           // Number of mipmap levels
                    GL_RGBA8,            // Internal format
                    width, height,       // Dimensions of each texture
                    m_layerCount);  
    
    for(ui8 layer=0;layer<m_layerCount;layer++)
    {
    
        //!LAYER# IS THE # THAT CORRESPENDS TO THE TILE OF THE MAP TO BE RENDERED EX: LAYER=0 (STONE), LAYER=1 (REDBRICK)
        // Upload the texture data to the specific layer
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                        0,                    // Mipmap level
                        0, 0, layer,          // xoffset, yoffset, layer 
                        width, height, 1,     // width, height, depth (always 1 for one layer)
                        GL_RGBA,              // Format
                        GL_UNSIGNED_BYTE,     // Type
                        textures[layer].data());
        
        stbi_image_free(textures[layer].data()); //frees here, might change
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D_ARRAY,0);
    LOG_INFO("Texture Array ID: ", m_rendererID, " loaded\nSize: (",m_width,"x",m_height,") TexCount: ", m_layerCount);

}
TextureArray2D::~TextureArray2D()
{
    glDeleteTextures(1,&m_rendererID);
}
void TextureArray2D::Bind(unsigned int slot, GLuint shaderProgram, std::string uniformName) const
{
    const char* nameC=uniformName.c_str();
    GLint uniformLocation = glGetUniformLocation(shaderProgram, nameC);

    glActiveTexture(GL_TEXTURE0+slot);

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_rendererID);

    glUniform1i(uniformLocation, slot);
}
void TextureArray2D::UnBind() const
{

}

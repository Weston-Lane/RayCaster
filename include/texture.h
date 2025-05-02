#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "defines.h"
#include <string>
class Texture2D
{
private:
    unsigned int m_rendererID;
    std::string m_filePath;
    unsigned char* m_localBuffer;//local storage for texture
    int m_width, m_height, m_BPP;//bits per pixel
public:
    Texture2D(const std::string& path);
    Texture2D();
    ~Texture2D();

    void Bind(unsigned int slot) const;
    void UnBind();

    inline int GetWidth() const {return m_width;}
    inline int GetHeight() const {return m_height;}
    inline std::string GetFilePath() const {return m_filePath;}
    inline int GetRenderID() const{return m_rendererID;}
    inline unsigned char* data() const {return m_localBuffer;}


};

class TextureArray2D
{
    private:
        int m_width, m_height; //dimensions of each texture
        int m_layerCount; //number of textures
        unsigned int m_rendererID;
    public:
        TextureArray2D(Texture2D* textures, int width, int  height, int numLayers);
        ~TextureArray2D();

        void Bind(unsigned int slot, GLuint shaderProgram, std::string uniformName) const;
        void UnBind() const;

        inline int GetWidth() const {return m_width;}
        inline int GetHeight() const {return m_height;}
        inline int GetRenderID() const{return m_rendererID;}

        
};
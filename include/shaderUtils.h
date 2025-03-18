#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "mathUtils.h"

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};
class Shader
{
    public:
        Shader(const std::string& filePath);
        ~Shader();
        void Bind() const;
        void UnBind() const;

        //set uniform
        void SetUniform1i(const std::string& name, int data);
        void SetUniform2f(const std::string& name, vec2f& data);
        void SetUniform3f(const std::string& name, vec3f& data);
        void SetUniform4f(const std::string& name, vec4f& data);

        inline unsigned int getID() {return m_rendererID;}
        inline std::string& GetFilePath(){return m_filePath;}

    private:

        ShaderSource loadShader(const std::string& path);
        unsigned int CompileShader(unsigned int type, std::string& source);
        unsigned int CreateShader(std::string& vertex, std::string& fragment);
        std::string m_filePath;
        unsigned int m_rendererID;
        int GetUniformLocation(const std::string& name);
};




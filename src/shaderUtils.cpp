#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "shaderUtils.h"


Shader::Shader(const std::string& filePath)
:m_filePath(filePath), m_rendererID(0)
{
    ShaderSource source=loadShader(filePath);
    m_rendererID=CreateShader(source.vertex,source.fragment);
}
Shader::~Shader()
{
    glDeleteProgram(m_rendererID);
}
ShaderSource Shader::loadShader(const std::string& path)
{
    std::ifstream src(path);
    enum Type
    {
        NONE=-1, VERTEX=0, FRAGMENT=1
    };
    
    std::string line;
    std::stringstream srcs[2];
    Type type=NONE;
    while(getline(src,line))
    {
        if(line.find("#shader")!=std::string::npos)
        {
            if(line.find("vertex")!=std::string::npos) {type=Type::VERTEX;}
            else if(line.find("fragment")!=std::string::npos){type=Type::FRAGMENT;}
        }
        else
        {
            if(type!=NONE) {srcs[(int)type]<<line<<'\n';}
        }
    }
    return {srcs[0].str(),srcs[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, std::string& source)
{
    
    unsigned int id=glCreateShader(type);
    const char* sourceC=source.c_str();
    
    glShaderSource(id,1,&sourceC,NULL);
    glCompileShader(id);

    int success=0;
    glGetShaderiv(id,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        
        glGetShaderInfoLog(id,512,NULL,infoLog);
        LOG_ERROR("SHADER TYPE [",type,"] COMPILATION FAILED [GL infoLog]:",infoLog);
        return NULL;
    }
    return id;
}
unsigned int Shader::CreateShader(std::string& vertex, std::string& fragment)
{
    unsigned int program=glCreateProgram();
    unsigned int vs=CompileShader(GL_VERTEX_SHADER,vertex);
    unsigned int fs=CompileShader(GL_FRAGMENT_SHADER,fragment);
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);

    int success=0;
    glGetProgramiv(program,GL_LINK_STATUS,&success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(program,512,NULL,infoLog);
        
        LOG_ERROR("SHADER COULD NOT LINK [GL infoLog]:",infoLog);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
void Shader::Bind() const
{
    glUseProgram(m_rendererID);   
}
void Shader::UnBind() const
{
    glUseProgram(0);
}

//set uniform
void Shader::SetUniform2f(const std::string& name, vec2f& data)
{
    const char* nameC=name.c_str();
    unsigned int loc=glGetUniformLocation(m_rendererID,nameC);
    glUniform2f(loc,data.x,data.y);
}
void Shader::SetUniform3f(const std::string& name, vec3f& data)
{
    const char* nameC=name.c_str();
    unsigned int loc=glGetUniformLocation(m_rendererID,nameC);
    glUniform3f(loc,data.x,data.y,data.z);
}
void Shader::SetUniform4f(const std::string& name, vec4f& data)
{
    const char* nameC=name.c_str();
    unsigned int loc=glGetUniformLocation(m_rendererID,nameC);
    glUniform4f(loc,data.x,data.y,data.z,data.w);
}





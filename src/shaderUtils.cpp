#include "shaderUtils.h"
ShaderSource loadShader(const std::string& path)
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
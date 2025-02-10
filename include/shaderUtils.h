#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "defines.h"

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};
ShaderSource loadShader(const std::string& path);



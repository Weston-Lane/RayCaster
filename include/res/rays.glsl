#shader vertex
#version 460 core
layout (location=3) in vec2 aPos;
out vec4 vertColor;
void main()
{
    gl_Position=vec4(aPos,0,1.f);//do not put a 0 in the w pos or undefined behavior
    vertColor=vec4(0,1,0,0);
}

#shader fragment
#version 460 core
in vec4 vertColor;
out vec4 fragColor;
void main()
{
    fragColor=vertColor;
}
#shader vertex
#version 330 core
layout (location=0) in vec3 aPos;
uniform vec3 translate;
out vec4 vertColor;
void main()
{
    vec3 new=aPos+translate;
    gl_Position=vec4(new,1);
    vertColor=vec4(1,.5,0,1);
}

#shader fragment
#version 330 core
in vec4 vertColor;
out vec4 fragColor;
void main()
{
    fragColor=vertColor;
}
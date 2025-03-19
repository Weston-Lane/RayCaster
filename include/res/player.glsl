#shader vertex
#version 460 core
layout (location=0) in vec3 aPos;
//uniform vec3 translate;
out vec4 vertColor;
void main()
{
    vec3 new=aPos;//+translate;
    // new.x+=new.x*.45;
    // new.y+=new.y*.25;
    gl_Position=vec4(new.x,new.y,new.z,1);
    vertColor=vec4(1,.5,0,1);
}

#shader fragment
#version 460 core
in vec4 vertColor;
out vec4 fragColor;
void main()
{
    fragColor=vertColor;
}
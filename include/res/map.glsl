#shader vertex
#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
out vec3 vertColor;
void main()
{
    gl_Position=vec4(aPos,1);
    vertColor=aColor;
}

#shader fragment
#version 460 core
in vec3 vertColor;
//uniform sampler2D uTexture;
out vec4 fragColor;
void main()
{
    //fragColor.a=vertColor.x;

    fragColor=vec4(vertColor,vertColor.x);
    //fragColor=texture(uTexture,vertColor.xy);
}

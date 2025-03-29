#shader vertex
#version 460 core
layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTex;
//uniform vec4 wallColor;
//out vec4 vertColor;
out vec2 texCoord;
void main()
{
    vec2 pos=aPos;
    gl_Position=vec4(pos,0,1.f);//do not put a 0 in the w pos or undefined behavior//gl_Position=vec4(aPos,0,1.f);//do not put a 0 in the w pos or undefined behavior
    //vertColor=wallColor;
    texCoord=aTex;
    //texCoord=vec2(.5,.9);
}

#shader fragment
#version 460 core
//in vec4 vertColor;
in vec2 texCoord;
uniform float uFog;
uniform sampler2D uTexture;
out vec4 fragColor;
void main()
{
    vec4 texColor=texture(uTexture,texCoord);
    texColor.w=uFog;
    fragColor=texColor;
}
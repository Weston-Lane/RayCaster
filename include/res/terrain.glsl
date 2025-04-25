#shader vertex
#version 460 core

layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTex;
layout (location=2) in vec2 aTexSlot;
//layout (location=3) in vec2 aFog;

out vec2 texCoord;
out vec2 texSlot;
out float fog;
void main()
{
    vec2 pos=aPos;
    gl_Position=vec4(pos,0,1.f);//do not put a 0 in the w pos or undefined behavior

    texCoord=aTex;
    texSlot=aTexSlot;
    fog=aTexSlot.y;
}

#shader fragment
#version 460 core

in vec2 texCoord;
in vec2 texSlot;
in float fog;

uniform float uFog;
uniform sampler2D uTexture;
uniform sampler2DArray uTexArray;

out vec4 fragColor;

void main()
{
    
    //vec4 texColor=texture(uTexture,texCoord);
    int slot=int(texSlot.x);
    vec4 texColor=texture(uTexArray,vec3(texCoord,slot));
    texColor.w=1;//fog;
    fragColor=texColor;
}
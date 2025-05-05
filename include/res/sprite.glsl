#shader vertex
#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aTexCoord;
uniform float uFog;
out vec3 texCoord;
out float fog;
void main()
{
    gl_Position=vec4(aPos,1);
    texCoord=aTexCoord;
    fog=uFog;
}

#shader fragment
#version 460 core
in vec4 vertColor;
in vec3 texCoord;
in float fog;
uniform sampler2D uTexture;

out vec4 fragColor;
void main()
{
    vec4 texColor=texture(uTexture,texCoord.xy);
    texColor.a*=fog;
    fragColor=texColor;
}
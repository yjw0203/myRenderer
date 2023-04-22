#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 outColor;
layout(set = 0,binding = 0) uniform sampler2D myTexture;
void main() {
    //outColor = vec4(1,0,0,1);
    outColor = texture(myTexture,uv);
    //outColor = vec4(pos.xyz,1);
    //outColor = vec4(uv,0,1);
}
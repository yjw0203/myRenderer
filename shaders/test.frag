#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 outColor;
layout(set = 1,binding = 0) uniform sampler2D myTexture;

void main() {
    outColor = texture(myTexture,uv);
}
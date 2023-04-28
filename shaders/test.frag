#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;

layout(location = 0) out vec4 outColor;
layout(set = 0,binding = 0) uniform sampler2D myTexture;
void main() {
    outColor = vec4(color,1.0);
}
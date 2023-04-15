#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 outColor;
layout(set = 0,binding = 0) uniform sampler2D myTexture;
void main() {
    float x=pos.x * 2.0;
    float y=-pos.y * 2.0;
    float f=pow(x*x+y*y-1,3)-5*x*x*y*y*y;
    if(f<0)
    {
        outColor = color;
    }
    else
    {
        outColor = texture(myTexture,vec2(0.5,0.5));
    }
}
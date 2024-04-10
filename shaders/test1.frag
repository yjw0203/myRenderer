#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 outColor;

layout(set = 1,binding = 0) uniform sampler2D sampler_tex;

void main() {
    float x=pos.x * 4.0;
    float y=-pos.y * 4.0;
    float f=pow(x*x+y*y-1,3)-5*x*x*y*y*y;
    if(f<0)
    {
        outColor = texture(sampler_tex,vec2(x,y));
    }
    else
    {
        outColor = vec4(0.0,0.0,0.0,0.0);
    }
}
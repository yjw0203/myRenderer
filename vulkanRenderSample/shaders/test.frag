#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 outColor;

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
        outColor = vec4(0.0,0.0,0.0,1.0);
    }
}
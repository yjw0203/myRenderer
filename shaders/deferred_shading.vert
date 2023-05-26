#version 450

layout(location = 0) in vec2 in_pos;

layout(location = 0) out vec2 pos;


void main() {
    gl_Position = vec4(in_pos,1,1);
    pos = in_pos;
 }

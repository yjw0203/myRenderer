#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

layout(location = 0) out vec4 pos;
layout(location = 1) out vec3 color;

 void main() {
 pos = vec4(in_pos, 1.0);
 
 gl_Position = pos;
 color = in_color;

 }

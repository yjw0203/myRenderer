#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec4 pos;
layout(location = 1) out vec2 uv;

layout(set = 0,binding = 0) uniform Camera
{
    mat4 view;
    mat4 project;
}camera;

 void main() {
 pos = camera.project * camera.view * vec4(in_pos, 1.0);
 
 gl_Position = pos;
 uv = in_uv;

 }

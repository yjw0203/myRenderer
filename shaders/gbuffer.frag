#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;

layout(set = 1,binding = 0) uniform sampler2D albedoTex;
layout(set = 0,binding = 1) uniform Light
{
    vec3 pos;
    vec3 color;
}light;

layout(set = 0,binding = 2) uniform cameraPos
{
    vec3 pos;
}camerapos;

void main() {
    albedo = texture(albedoTex,uv);
    normal = vec4(in_normal,1);
}
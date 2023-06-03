#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 diffuse;
layout(location = 3) out vec4 specular;
layout(location = 4) out vec4 ambient;

layout(set = 1,binding = 0) uniform sampler2D albedoTex;
layout(set = 0,binding = 1) uniform Light
{
    vec3 pos;
    vec3 color;
}light;

layout(set = 0,binding = 2) uniform cameraPos
{
    mat4 view;
    mat4 project;
    vec3 pos;
}camerapos;

layout(set = 0,binding = 3) uniform Material
{
    vec4 diffuse;
    vec4 specular;
    vec3 ambient;
}material;

void main() {
    albedo = texture(albedoTex,uv);
    normal = vec4(in_normal,1);
    diffuse = material.diffuse;
    specular = material.specular;
    ambient = vec4(material.ambient,1);
}
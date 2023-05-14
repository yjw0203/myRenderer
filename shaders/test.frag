#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outColor;
layout(set = 1,binding = 0) uniform sampler2D myTexture;
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

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.pos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.4;
    vec3 viewDir = normalize(camerapos.pos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;


    vec4 objectColor = texture(myTexture,uv);
    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
    result = (ambient + diffuse + specular) * objectColor.rgb;
    outColor = vec4(result, 1.0);
}
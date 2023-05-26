#version 450

layout(location = 0) in vec2 pos;

layout(location = 0) out vec4 outColor;
layout(set = 1,binding = 0) uniform sampler2D albedo_map;
layout(set = 1,binding = 1) uniform sampler2D normal_map;
layout(set = 1,binding = 2) uniform sampler2D depth_map;

layout(set = 0,binding = 0) uniform Camera
{
    mat4 view;
    mat4 project;
}camera;

layout(set = 0,binding = 1) uniform Light
{
    vec3 pos;
    vec3 color;
}light;

layout(set = 0,binding = 2) uniform cameraPos
{
    vec3 pos;
}camerapos;


layout(set = 0,binding = 3) uniform Screen
{
    vec2 size;
}screen;

void main() {

    vec2 uv = gl_FragCoord.xy/screen.size;

    mat4 screenToWorld = inverse(camera.project * camera.view);
    vec4 camera_pos = vec4(camerapos.pos,1);

    vec4 pos = screenToWorld * vec4(uv * 2.0-1.0,texture(depth_map,uv).r,1);
    pos = vec4(pos.xyz/pos.w,1);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color;

    vec3 norm = normalize(texture(normal_map,uv).xyz);
    vec3 lightDir = normalize(light.pos - pos.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.4;
    vec3 viewDir = normalize(camera_pos.xyz - pos.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;


    vec4 objectColor = texture(albedo_map,uv);
    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
    outColor = vec4(result, 1.0);
}
#version 450

layout(location = 0) in vec2 pos;

layout(location = 0) out vec4 outColor;
layout(set = 1,binding = 0) uniform sampler2D albedo_map;
layout(set = 1,binding = 1) uniform sampler2D normal_map;
layout(set = 1,binding = 2) uniform sampler2D diffuse_map;
layout(set = 1,binding = 3) uniform sampler2D specular_map;
layout(set = 1,binding = 4) uniform sampler2D ambient_map;
layout(set = 1,binding = 5) uniform sampler2D depth_map;

layout(set = 0,binding = 0) uniform Camera
{
    mat4 view;
    mat4 project;
    vec3 pos;
}camera;

layout(set = 0,binding = 1) uniform Light
{
    vec3 pos;
    vec3 color;
}light;

layout(set = 0,binding = 2) uniform Option
{
    vec2 screenSize;
}option;

void main() {

    vec2 uv = SV_Position.xy/option.screenSize;

    mat4 screenToWorld = inverse(camera.project * camera.view);
    vec4 camera_pos = vec4(camera.pos,1);

    vec4 pos = screenToWorld * vec4(uv * 2.0-1.0,texture(depth_map,uv).r,1);
    pos = vec4(pos.xyz/pos.w,1);

    vec3 ambientStrength = texture(ambient_map,uv).xyz;
    vec3 ambient = ambientStrength * light.color.xyz;

    vec3 norm = normalize(texture(normal_map,uv).xyz);
    vec3 lightDir = normalize(light.pos - pos.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * texture(diffuse_map,uv).xyz;

    vec4 specularStrength = texture(specular_map,uv);
    vec3 viewDir = normalize(camera_pos.xyz - pos.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularStrength.w);
    vec3 specular = specularStrength.xyz * spec * light.color;


    vec4 objectColor = texture(albedo_map,uv);
    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
    outColor = vec4(result, 1.0);
}
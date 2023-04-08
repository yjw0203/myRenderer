#version 450

layout(location = 0) out vec4 pos;
layout(location = 1) out vec4 color;

vec2 positions[3] = vec2[](
 vec2(-1.0, -3.0),
 vec2(3.0, 1.0),
 vec2(-1.0, 1.0)
 );

vec3 colors[3] = vec3[](
 vec3(0.0, 1.0, 0.0),
 vec3(0.0, 1.0, 0.0),
 vec3(0.0, 1.0, 0.0)
 );

 void main() {
 gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
 pos = gl_Position;
 color = vec4(colors[gl_VertexIndex],1.0);
 }

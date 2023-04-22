#version 450

layout(location = 0) out vec4 pos;
layout(location = 1) out vec2 uv;

mat4 setRotation( float x, float y, float z )
{
    float a = sin(x); float b = cos(x); 
    float c = sin(y); float d = cos(y); 
    float e = sin(z); float f = cos(z); 

    float ac = a*c;
    float bc = b*c;

    return mat4( d*f,      d*e,       -c, 0.0,
                 ac*f-b*e, ac*e+b*f, a*d, 0.0,
                 bc*f+a*e, bc*e-a*f, b*d, 0.0,
                 0.0,      0.0,      0.0, 1.0 );
}

mat4 setTranslation( float x, float y, float z )
{
    return mat4( 1.0, 0.0, 0.0, 0.0,
				 0.0, 1.0, 0.0, 0.0,
				 0.0, 0.0, 1.0, 0.0,
				 x,     y,   z, 1.0 );
}

vec3 positions[8] = vec3[](
 vec3(-1.0, -1.0, -1.0),
 vec3(-1.0, 1.0, -1.0),
 vec3(1.0, 1.0, -1.0),
 vec3(1.0, -1.0, -1.0),
 vec3(-1.0, -1.0, 1.0),
 vec3(-1.0, 1.0, 1.0),
 vec3(1.0, 1.0, 1.0),
 vec3(1.0, -1.0, 1.0)
 );

 vec2 uvs[4] = vec2[](
    vec2(0,0),
    vec2(0,1),
    vec2(1,0),
    vec2(1,1)
 );

int uv_indices[36] = int[](
 0,3,1,
 0,2,3,
 0,3,1,
 0,2,3,
 1,3,0,
 0,3,2,
 1,3,2,
 1,2,0,
 0,1,2,
 1,2,3,
 2,0,1,
 2,1,3
);

int pos_indices[36] = int[](
 0,2,1,
 0,3,2,
 3,6,2,
 3,7,6,
 4,5,7,
 7,5,6,
 0,1,5,
 0,5,4,
 1,2,5,
 2,6,5,
 0,4,7,
 0,7,3
);


 void main() {
 pos = vec4(positions[pos_indices[gl_VertexIndex]]+vec3(0,0,1.5), 1.0);
 
 gl_Position = pos;

 uv = uvs[uv_indices[gl_VertexIndex]];

 }

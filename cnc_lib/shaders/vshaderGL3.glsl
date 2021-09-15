#version 300 es
layout(location = 0) in highp vec4 vertex;
layout(location = 1) in highp vec3 normal;

out highp vec3 vert;
out highp vec3 vertNormal;
out highp vec3 color;

uniform highp mat4 mvp_matrix;

void main()
{
    gl_Position = mvp_matrix * vertex;
    vertNormal = normal;
    vert = vertex.xyz;
}

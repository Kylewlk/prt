#version 430
layout(location= 1) uniform mat4 mvp;

layout(location= 0) in vec4 pos;
layout(location= 1) in vec3 color;

layout(location= 0) out vec3 vColor;

void main()
{
    vColor = color;
    gl_Position = mvp*pos;
}
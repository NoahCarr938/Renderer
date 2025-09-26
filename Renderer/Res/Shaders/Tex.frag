#version 430 core

in vec2 vertUV;

out vec4 vertColor;

layout (location = 3) uniform float time;

void main()
{
// red and green to illustrate that uvs are working
   vertColor = vec4(vertUV.x, vertUV.y, 0, 1);
}
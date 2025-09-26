#version 430 core

// Vertex attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;


// Uniforms
layout (location = 0) uniform mat4 proj;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 model;

layout (location = 3) uniform float time;
layout (location = 4) uniform float displacement = 5;

out vec2 vertUV;

void main()
{  
// exports to fragment shader
   vertUV = uv;
   vertUV.x += sin(time);

   vec4 myPosition = position;
   //myPosition.y += 5;
// transform object from
// object to world (model)
// World to camera (view)
// camera to clip (proj)
   gl_Position = proj * view * model * myPosition;
}
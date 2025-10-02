#version 430 core

layout (location = 3) unifrom sampler2D albedo;
layout (location = 4) uniform vec3 ambientLightColor;
layout (location = 5) uniform vec3 lightDir;

in vec2 vUV;
in vec3 vNormal;

out vec4 fragColor;

void main()
{
   float d = max(0, dot(vNormal, -lightDir));
   vec3 diffuse = d * lightColor;
   vec4 baseColor = texture(albedo, vUV);

   fragColor.xyz = baseColor.xyz * (ambientLightColor + diffuse);
   fragColor.a = baseColor.a;
}
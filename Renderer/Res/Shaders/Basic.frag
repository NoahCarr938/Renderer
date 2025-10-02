#version 430 core

in vec2 vUV;

layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 ambientLight;

out vec4 fragColor;

void main()
{
    vec4 baseColor = texture(albedo, vUV);  // get color from texture at UV location

    fragColor.rgb = baseColor.rgb * ambientLight; // combine RGB with tint via multiply
    fragColor.a = baseColor.a; // use alpha from texture
}

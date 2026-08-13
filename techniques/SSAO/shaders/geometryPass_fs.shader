#version 440

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
};

in VS_OUT
{
	vec2 textureCoordinates;
	vec3 fragPosition;
	vec3 normal;
} fs_in;

layout (location = 0) out vec4 fragPosition;
layout (location = 1) out vec4 fragNormal;
layout (location = 2) out vec4 fragColor;

uniform Material material;

void main()
{
	fragPosition = vec4(fs_in.fragPosition, 1.);
	fragNormal = vec4(normalize(fs_in.normal), 1.);
	fragColor = vec4(texture(material.texture_diffuse1, fs_in.textureCoordinates).rgb, texture(material.texture_specular1, fs_in.textureCoordinates).r);
}
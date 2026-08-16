#version 440

in VS_OUT
{
	vec2 textureCoordinates;
	vec3 fragPosition;
	vec3 normal;
} fs_in;

out float fragDepth;

uniform vec3 lightPosition;

void main()
{
	fragDepth = length(lightPosition - fs_in.fragPosition);
}
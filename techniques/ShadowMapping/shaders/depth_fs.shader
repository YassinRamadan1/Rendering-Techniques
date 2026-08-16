#version 440

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

out float fragDepth;

uniform vec3 lightPosition;

void main()
{
	fragDepth = length(lightPosition - fragPosition);
}
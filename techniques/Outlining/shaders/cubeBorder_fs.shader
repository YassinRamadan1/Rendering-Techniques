#version 440

out vec4 fragColor;

uniform vec3 borderColor;

void main()
{
    fragColor = vec4(borderColor, 1.);
}
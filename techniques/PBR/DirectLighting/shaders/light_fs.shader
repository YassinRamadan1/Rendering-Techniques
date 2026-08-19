#version 440

out vec4 fragColor;

uniform vec3 lightColor;

void main()
{
    vec3 color = lightColor / (lightColor + vec3(1.0));
    fragColor = vec4(color, 1.);
}
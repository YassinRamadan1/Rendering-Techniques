#version 440

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform vec3 lightColor;

void main()
{
    fragColor = vec4(lightColor, 1.);

    brightColor = vec4(0, 0, 0, 1);
	if(dot(lightColor, vec3(0.2126, 0.7152, 0.0722)) > 1.)
	{
		brightColor = vec4(lightColor, 1.);
	}
}
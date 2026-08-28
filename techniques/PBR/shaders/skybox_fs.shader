#version 440

in vec3 fragPosition;

out vec4 fragColor;

uniform samplerCube tex_skybox;

void main()
{
	vec3 color = texture(tex_skybox, normalize(fragPosition)).rgb;
	
	color = color / (color + vec3(1.0));

	color = pow(color, vec3(1 / 2.2));

	fragColor = vec4(color, 1.0);
}
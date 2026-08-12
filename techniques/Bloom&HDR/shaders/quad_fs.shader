#version 440

out vec4 fragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec2 resolution;
uniform float exposure;

void main()
{	
	vec2 textureCoordinates = gl_FragCoord.xy / resolution.xy;
	vec3 color = texture(tex1, textureCoordinates).rgb + texture(tex2, textureCoordinates).rgb;

	color = vec3(1.) - exp(-color * exposure);
	color = pow(color, vec3(1 / 2.2));

	fragColor = vec4(color, 1.);
}
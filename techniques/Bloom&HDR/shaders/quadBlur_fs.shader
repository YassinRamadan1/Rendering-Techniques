#version 440

out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 resolution;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform bool horizontal;

void main()
{	
	vec2 textureCoordinates = gl_FragCoord.xy / resolution.xy;
	vec3 color = weight[0] * texture(tex, textureCoordinates).rgb;
	vec2 offset = 1. / textureSize(tex, 0);
	if (horizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			color += weight[i] * (texture(tex, textureCoordinates - vec2(i * offset.x, 0)).rgb + texture(tex, textureCoordinates + vec2(i * offset.x, 0)).rgb);
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			color += weight[i] * (texture(tex, textureCoordinates - vec2(0, i * offset.y)).rgb + texture(tex, textureCoordinates + vec2(0, i * offset.y)).rgb);
		}
	}

	fragColor = vec4(color, 1.);
}
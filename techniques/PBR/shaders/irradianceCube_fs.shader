#version 440

const float PI = 3.14159265359;

in vec3 fragPosition;

out vec3 fragColor;

uniform samplerCube tex_environment;

void main()
{
	vec3 normal = normalize(fragPosition);
	vec3 tangent = normalize(cross(vec3(0, 1, 0), normal));
	vec3 biTangent = cross(normal, tangent);
	vec3 irradiance = vec3(0);

	float sampleDelta = 0.025; 
	float sampleCount = 0.0;
	for (float phi = 0.0; phi < 2.0f * PI; phi += sampleDelta)
		for (float theta = 0.0; theta < PI / 2.0f; theta += sampleDelta)
		{
			vec3 sampleInTangent = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			vec3 wi = sampleInTangent.x * tangent + sampleInTangent.y * biTangent + sampleInTangent.z * normal;
			vec3 radiance = texture(tex_environment, wi).rgb;
			irradiance += radiance * cos(theta) * sin(theta);
			++sampleCount;
		}

	fragColor = PI * irradiance / sampleCount;
}
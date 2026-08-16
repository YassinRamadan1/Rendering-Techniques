#version 440

struct DirLight
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
	float constant;
	float linear;
	float quadratic;

	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

out vec4 fragColor;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_depth[2];
uniform samplerCube texture_pointLightDepth;

uniform PointLight light[4];
uniform DirLight dirLight[2];
uniform vec3 cameraPosition;
uniform float shininessCoeffecient;
uniform mat4 dirLightTransform[2];

float sampleCount = 20;
vec3 sampleOffsetDirections[20] = vec3[]
(
vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 calcDirLight(DirLight l, vec3 normal, vec3 viewDir, float shadow);
vec3 calcPointLight(PointLight l, vec3 normal, vec3 viewDir, float shadow);

void main()
{	
	vec3 viewDir = normalize(cameraPosition - fragPosition);
	vec3 color = vec3(0);
	float shadow;
	for (int i = 0; i < 2; ++i)
	{
		shadow = 0.0;
		vec2 offset = 1.f / textureSize(texture_depth[i], 0);
		vec4 fragPositionLightView = dirLightTransform[i] * vec4(fragPosition, 1.);
		fragPositionLightView /= fragPositionLightView.w;
		fragPositionLightView = fragPositionLightView * 0.5 + 0.5;
	
		if (fragPositionLightView.z <= 1.)
		{
			float bias = max(0.005, 0.05 * (1.f - dot(normalize(normal), -dirLight[i].direction)));
			for (int x = -1; x <= 1; ++x)
				for (int y = -1; y <= 1; ++y)
					shadow += texture(texture_depth[i], fragPositionLightView.xy + vec2(x, y) * offset).r < fragPositionLightView.z - bias ? 1.0 : 0.0;
		}

		color += calcDirLight(dirLight[i], normalize(normal), viewDir, shadow / 9.f);
	}

	for (int i = 0; i < 1; ++i)
	{
		shadow = 0.0f;
		vec3 lightToFrag = fragPosition - light[i].position;
		
		float bias = max(0.005, 0.05 * (1.f - dot(normal, -normalize(lightToFrag))));

		for (int j = 0; j < sampleCount; ++j)
		{
			shadow += texture(texture_pointLightDepth, normalize(lightToFrag + 0.05 * sampleOffsetDirections[j])).r < length(lightToFrag) - bias ? 1.0 : 0.0;
		}

		color += calcPointLight(light[i], normalize(normal), viewDir, shadow / sampleCount);
	}
	fragColor = vec4(color, 1.);
}

vec3 calcDirLight(DirLight l, vec3 normal, vec3 viewDir, float shadow)
{
	vec3 lightDir = normalize(-l.direction);
	
	vec3 diffuse = texture(texture_diffuse, textureCoordinates).rgb;

    vec3 ambient = l.ambient * diffuse;
	
	diffuse = max(dot(normal, lightDir), 0.) * l.diffuse * diffuse;
	
	vec3 specular = pow(max(dot(normal, normalize(viewDir + lightDir)), 0.), shininessCoeffecient) * l.specular;

	return ambient + (1. - shadow) * (diffuse + specular);
}

vec3 calcPointLight(PointLight l, vec3 normal, vec3 viewDir, float shadow)
{
	vec3 lightDir = normalize(l.position - fragPosition);
	
	vec3 diffuse = texture(texture_diffuse, textureCoordinates).rgb;
    vec3 ambient = l.ambient * diffuse;
	diffuse = max(dot(normal, lightDir), 0.) * l.diffuse * diffuse;
	
	vec3 specular = pow(max(dot(normal, normalize(viewDir + lightDir)), 0.), shininessCoeffecient) * l.specular;
	float r = length(l.position - fragPosition);
	float attenuation = 1. / (l.constant + l.linear * r + l.quadratic * r * r);

	return ambient + attenuation * (1. - shadow) * (diffuse + specular);
}
#version 440

in VS_OUT
{
	vec2 textureCoordinates;
	vec3 lightPosition;
	vec3 cameraPosition;
	vec3 fragPosition;
} fs_in;

out vec4 fragColor;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_normal1;
	sampler2D texture_height1;
	sampler2D texture_ao1;
};

uniform vec3 lightColor;
uniform float shininessCoeffecient;
uniform float heightScale;
uniform Material material;

vec2 parallaxMapping(vec2 textureCoords, vec3 viewDir)
{
	const int minNumLevels = 8;
	const int maxNumLevels = 32;
	int numberOfLevels = int(mix(maxNumLevels, minNumLevels, max(dot(viewDir, vec3(0., 0., 1.)), 0.)));
	
	vec2 curTexCoords = textureCoords;
	vec2 deltaTexCoords = viewDir.xy * heightScale / numberOfLevels;
	float deltaDepth = 1. / numberOfLevels;
	float curDepth = 0.;
	float curDepthMapValue = 1. - texture(material.texture_height1, curTexCoords).r;
	float lastDepthMapValue = curDepthMapValue;
	while(curDepth < curDepthMapValue)
	{
		curDepth += deltaDepth;
		curTexCoords -= deltaTexCoords;
		lastDepthMapValue = curDepthMapValue;
		curDepthMapValue = 1. - texture(material.texture_height1, curTexCoords).r;
	}
	
	vec2 lastTexCoords = curTexCoords + deltaTexCoords;
	float afterDepth = curDepthMapValue - curDepth;
	float beforeDepth = lastDepthMapValue - curDepth + deltaDepth;
	return mix(curTexCoords, lastTexCoords, afterDepth / (afterDepth - beforeDepth));
}

void main()
{
	vec3 viewDirection = normalize(fs_in.cameraPosition - fs_in.fragPosition);

	vec2 newTexCoordinates = parallaxMapping(fs_in.textureCoordinates, viewDirection);
	
	if (newTexCoordinates.x > 1. || newTexCoordinates.x < 0. || newTexCoordinates.y > 1. || newTexCoordinates.y < 0.)
	{
		discard;
	}

	vec3 normal = texture(material.texture_normal1, newTexCoordinates).rgb;
	normal = normalize(normal * 2. - 1.);

	vec3 lightDirection = normalize(fs_in.lightPosition - fs_in.fragPosition);
	vec3 c = texture(material.texture_diffuse1, newTexCoordinates).rgb * lightColor;
	vec3 diffuse = c * max(dot(normal, lightDirection), 0.);
	vec3 ambient = 0.1 * texture(material.texture_ao1, newTexCoordinates).r * c;

	vec3 halfway = normalize(viewDirection + lightDirection);
	vec3 specular = 0.3 * pow(max(dot(normal, halfway), 0.), shininessCoeffecient) * lightColor;

	fragColor = vec4(diffuse + ambient + specular, 1.);
}
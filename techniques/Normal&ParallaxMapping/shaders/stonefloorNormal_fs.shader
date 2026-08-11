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
};

uniform vec3 lightColor;
uniform float shininessCoeffecient;
uniform Material material;

void main()
{
	vec3 normal = texture(material.texture_normal1, fs_in.textureCoordinates).rgb;
	normal = normalize(normal * 2. - 1.);
	
	vec3 viewDirection = normalize(fs_in.cameraPosition - fs_in.fragPosition);

	vec3 lightDirection = normalize(fs_in.lightPosition - fs_in.fragPosition);
	vec3 c = texture(material.texture_diffuse1, fs_in.textureCoordinates).rgb * lightColor;
	vec3 diffuse = c * max(dot(normal, lightDirection), 0.);
	vec3 ambient = 0.1 * c;

	vec3 halfway = normalize(viewDirection + lightDirection);
	vec3 specular = 0.3 * pow(max(dot(normal, halfway), 0.), shininessCoeffecient) * lightColor;

	fragColor = vec4(diffuse + ambient + specular, 1.);
}
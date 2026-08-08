#version 440

in VS_OUT
{
	vec2 textureCoordinates;
	vec3 fragPosition;
	vec3 normal;
} fs_in;

out vec4 fragColor;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_normal1;
};

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform Material material;

void main()
{
	vec3 normal = normalize(fs_in.normal);
	
	vec3 lightDirection = normalize(lightPosition - fs_in.fragPosition);
	vec3 c = texture(material.texture_diffuse1, fs_in.textureCoordinates).rgb * lightColor;
	vec3 diffuse = c * clamp(dot(normal, lightDirection), 0.,1.);
	vec3 ambient = 0.1 * c;
	fragColor = vec4(diffuse + ambient, 1.);
}
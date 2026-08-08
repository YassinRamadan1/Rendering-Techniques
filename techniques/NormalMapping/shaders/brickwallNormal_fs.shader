#version 440

in VS_OUT
{
	vec2 textureCoordinates;
	vec3 lightPosition;
	vec3 cameraPosition;
	vec3 fragPosition;
	vec3 no;
} fs_in;

out vec4 fragColor;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_normal1;
};

uniform vec3 lightColor;
uniform Material material;

void main()
{
	vec3 normal = texture(material.texture_normal1, fs_in.textureCoordinates).rgb;
	normal = normalize(normal * 2. - 1.);
	
	vec3 lightDirection = normalize(fs_in.lightPosition - fs_in.fragPosition);
	vec3 c = texture(material.texture_diffuse1, fs_in.textureCoordinates).rgb * lightColor;
	vec3 diffuse = c * clamp(dot(normal, lightDirection), 0.,1.);
	vec3 ambient = 0.1 * c;
	fragColor = vec4(diffuse + ambient, 1.);
}
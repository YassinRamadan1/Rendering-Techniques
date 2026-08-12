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
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragColor;

uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_color;
uniform vec2 resolution;

uniform vec3 cameraPosition;
uniform DirLight dirLight;
uniform PointLight light[4];
uniform float shininessCoeffecient;

vec3 calcPointLight(PointLight l, vec3 normal);

void main()
{
	vec2 textureCoordinates = gl_FragCoord.xy / resolution;
	vec3 normal = texture(texture_normal, textureCoordinates).rgb;

	vec3 color = vec3(0);
	for(int i = 0; i < 4; ++i)
	{
		color += calcPointLight(light[i], normal);
	}

	fragColor = vec4(color, 1.);
}

vec3 calcPointLight(PointLight l, vec3 normal)
{
	vec2 textureCoordinates = gl_FragCoord.xy / resolution;
	vec3 fragPosition = texture(texture_position, textureCoordinates).rgb;
	vec4 color = texture(texture_color, textureCoordinates);

	vec3 viewDir = normalize(cameraPosition - fragPosition);

	vec3 lightDir = normalize(l.position - fragPosition);
	
	vec3 diffuse = color.rgb;
    vec3 ambient = l.ambient * diffuse;
	diffuse = max(dot(normal, lightDir), 0.) * l.diffuse * diffuse;
	
	vec3 specular = pow(max(dot(normal, normalize(viewDir + lightDir)), 0.), shininessCoeffecient) * color.a * l.specular;
	
	float r = length(l.position - fragPosition);
	float attenuation = 1. / (l.constant + l.linear * r + l.quadratic * r * r);

	return ambient + attenuation * (diffuse + specular);
}
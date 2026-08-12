#version 440

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
in vec3 fragPos;
in vec3 normal;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform sampler2D tex;
uniform sampler2D spec;
uniform PointLight light[4];
uniform vec3 cameraPos;

vec3 calcPointLight(PointLight l, vec3 normal, vec3 viewDir);

void main()
{	
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 color = vec3(0);
	for(int i = 0; i < 4; ++i)
	{
		color += calcPointLight(light[i], normalize(normal), viewDir);
	}

	fragColor = vec4(color, 1.);

	brightColor = vec4(0, 0, 0, 1);
	if(dot(color, vec3(0.2126, 0.7152, 0.0722)) > 1.)
	{
		brightColor = vec4(color, 1.);
	}
}

vec3 calcPointLight(PointLight l, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(l.position - fragPos);
	
	vec3 diffuse = texture(tex, textureCoordinates).rgb;
    vec3 ambient = l.ambient * diffuse;
	diffuse = max(dot(normal, lightDir), 0.) * l.diffuse * diffuse;
	
	vec3 specular = pow(max(dot(normal, normalize(viewDir + lightDir)), 0.), 32.0) * l.specular * texture(spec, textureCoordinates).r;
	
	float r = length(l.position - fragPos);
	float attenuation = 1. / (l.constant + l.linear * r + l.quadratic * r * r);

	return  ambient + attenuation * (diffuse + specular);
}
#version 440

struct DirLight
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragColor;

uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_color;
uniform sampler2D texture_ao;

uniform vec2 resolution;
uniform float shininessCoeffecient;
uniform vec3 cameraPosition;
uniform mat4 view;
uniform bool withAO;

uniform DirLight light[2];

vec3 calcDirLight(DirLight l, vec3 normal);

void main()
{
	vec2 textureCoordinates = gl_FragCoord.xy / resolution;
	vec3 normal = texture(texture_normal, textureCoordinates).rgb;

	vec3 color = vec3(0);
	for(int i = 0; i < 2; ++i)
	{
		color += calcDirLight(light[i], normal);
	}

	fragColor = vec4(color, 1.);
}

vec3 calcDirLight(DirLight l, vec3 normal)
{
	vec2 textureCoordinates = gl_FragCoord.xy / resolution;
	vec3 fragPosition = texture(texture_position, textureCoordinates).rgb;
	vec4 color = texture(texture_color, textureCoordinates);

	vec3 viewDir = normalize(cameraPosition - fragPosition);

	vec3 lightDir = -normalize(mat3(view) * l.direction);
	
	vec3 diffuse = color.rgb;
    
	vec3 ambient = l.ambient * diffuse;
	ambient *= withAO == true ? texture(texture_ao, textureCoordinates).r : 1. ;
	
	diffuse = max(dot(normal, lightDir), 0.) * l.diffuse * diffuse;
	
	vec3 specular = pow(max(dot(normal, normalize(viewDir + lightDir)), 0.), shininessCoeffecient) * color.a * l.specular;

	return ambient + diffuse + specular;
}
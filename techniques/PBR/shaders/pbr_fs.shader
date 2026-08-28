#version 440
const float PI = 3.14159265359;

in vec3 fragPosition;
in vec2 textureCoordinates;
in mat3 TBN;

out vec4 fragColor;

uniform vec3 lightColor[4];
uniform vec3 lightPosition[4];
uniform vec3 cameraPosition;

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_metalness;
uniform sampler2D tex_roughness;
uniform sampler2D tex_ao;
uniform samplerCube tex_irradiance;
uniform samplerCube tex_specular;
uniform sampler2D tex_brdf;

vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float distributionGGX(float roughness, vec3 N, vec3 H);
float geometryShlickGGX(float NdotV, float k);
float geometrySmith(float roughness, vec3 N, vec3 V, vec3 L);
float calculateAttenuation(vec3 fragPosition, vec3 lightPosition);

void main()
{
	vec3 n = normalize(TBN * (texture(tex_normal, textureCoordinates).rgb * 2.0 - 1.0));
	vec3 v = normalize(cameraPosition - fragPosition);
	float NdotV = max(dot(n, v), 0.0);
	
	vec3 albedo = pow(texture(tex_albedo, textureCoordinates).rgb, vec3(2.2));
	float metalness = texture(tex_metalness, textureCoordinates).r;
	float roughness = texture(tex_roughness, textureCoordinates).r;
	float ao = texture(tex_ao, textureCoordinates).r;
	
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metalness);
	
	vec3 Lo = vec3(0.0f);
	for (int i = 0; i < 4; ++i)
	{
		vec3 l = normalize(lightPosition[i] - fragPosition);
		float NdotL = max(dot(n, l), 0.0);
		vec3 h = normalize(l + v);

		float attenuation = calculateAttenuation(fragPosition, lightPosition[i]);
		vec3 radiance = attenuation * lightColor[i];

		vec3 F = fresnelSchlick(max(dot(h, v), 0.0), f0);
		float NDF = distributionGGX(roughness, n, h);
		float G = geometrySmith(roughness, n, v, l);

		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= (1.0 - metalness);
		
		vec3 numerator = kS * G * NDF;
		float denom = 4.0 * NdotV * NdotL;
		vec3 specular = numerator / max(denom, 0.001);
		vec3 diffuse = kD * albedo / PI;
		vec3 BRDF = diffuse + specular; 

		Lo += BRDF * radiance * NdotL;
	}

	vec3 irradiance = texture(tex_irradiance, n).rgb;
	vec3 kS = fresnelSchlickRoughness(NdotV, f0, roughness);
	vec3 kD = vec3(1.0) - kS;
	kD *= (1.0 - metalness);
	
	vec3 inDirectDiffuse = kD * albedo * irradiance;
	
	const float maxReflectionLod = 4.0;
	vec3 prefilteredColor = textureLod(tex_specular, reflect(-v, n), roughness * maxReflectionLod).rgb;
	vec2 environemntBRDF = texture(tex_brdf, vec2(NdotV, roughness)).rg;
	
	vec3 inDirectSpecular = prefilteredColor * (kS * environemntBRDF.x + environemntBRDF.y);

	vec3 ambient = ao * (inDirectDiffuse + inDirectSpecular);
	
	vec3 color = Lo + ambient;

	color = color / (color + vec3(1.0));

	color = pow(color, vec3(1 / 2.2));
	
	fragColor = vec4(color, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(float roughness, vec3 N, vec3 H)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float denom = (NdotH * NdotH * (a2 - 1) + 1);
	return  a2 / (PI *  denom * denom);
}

float geometryShlickGGX(float NdotV, float k)
{
	return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(float roughness, vec3 N, vec3 V, vec3 L)
{
	float k = (roughness + 1);
	k =  k * k / 8.0;

	float NdotL = max(dot(N, L), 0.0);
	float NdotV = max(dot(N, V), 0.0);
	float shadowing = geometryShlickGGX(NdotL, k);
	float masking = geometryShlickGGX(NdotV, k);
	return shadowing * masking;
}

float calculateAttenuation(vec3 fragPosition, vec3 lightPosition)
{
	float r = length(fragPosition - lightPosition);
	return 1.0 / (1.0 + 0.1 * r + 0.01 * r * r);
}
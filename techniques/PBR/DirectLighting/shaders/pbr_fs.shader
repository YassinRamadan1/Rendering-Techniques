#version 440

in vec3 tangent_fragPosition;
in vec2 textureCoordinates;
in vec3 tangent_lightPosition[4];
in vec3 tangent_cameraPosition;

out vec4 fragColor;

uniform vec3 lightColor[4];

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_metalness;
uniform sampler2D tex_roughness;
uniform sampler2D tex_ao;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0);
float distributionGGX(float roughness, vec3 N, vec3 H);
float geometryShlickGGX(float NdotV, float k);
float geometrySmith(float roughness, vec3 N, vec3 V, vec3 L);
float calculateAttenuation(vec3 fragPosition, vec3 lightPosition);

void main()
{
	vec3 n = normalize(texture(tex_normal, textureCoordinates).rgb * 2.0 - 1.0);
	vec3 v = normalize(tangent_cameraPosition - tangent_fragPosition);
	
	vec3 albedo = pow(texture(tex_albedo, textureCoordinates).rgb, vec3(2.2));
	float metalness = texture(tex_metalness, textureCoordinates).r;
	float roughness = texture(tex_roughness, textureCoordinates).r;
	float ao = texture(tex_ao, textureCoordinates).r;
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metalness);
	
	vec3 Lo = vec3(0.0f);
	for (int i = 0; i < 4; ++i)
	{
		vec3 l = normalize(tangent_lightPosition[i] - tangent_fragPosition);
		float NdotL = max(dot(n, l), 0.0);
		vec3 h = normalize(l + v);

		float attenuation = calculateAttenuation(tangent_fragPosition, tangent_lightPosition[i]);
		attenuation = 1.0;
		vec3 radiance = attenuation * lightColor[i];

		vec3 F = fresnelSchlick(max(dot(h, v), 0.0), f0);
		float NDF = distributionGGX(roughness, n, h);
		float G = geometrySmith(roughness, n, v, l);

		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= (1.0 - metalness);
		
		vec3 numerator = kS * G * NDF;
		float denom = 4.0 * max(dot(n, v), 0.0) * NdotL;
		vec3 specular = numerator / max(denom, 0.001);
		vec3 diffuse = kD * albedo / PI;
		vec3 BRDF = diffuse + specular; 

		Lo += BRDF * radiance * NdotL;
	}
	vec3 ambient = vec3(0.03) * albedo * ao;
	Lo += ambient;

	Lo = Lo / (Lo + vec3(1.0));

	Lo = pow(Lo, vec3(1 / 2.2));
	
	fragColor = vec4(Lo, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1 - F0) * pow(1. - cosTheta, 5.);
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
	return 1.f / (1 + r * r);
}
# version 440

const float PI = 3.14159265359;

out vec2 fragColor;

uniform vec2 resolution;

vec2 Hammersley(uint i, uint N);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec2 BRDFIntegration(float NdotV, float roughness);

void main()
{
	vec2 textureCoordinates = gl_FragCoord.xy / resolution.xy;
	fragColor = BRDFIntegration(textureCoordinates.x, textureCoordinates.y);
}

float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness * roughness;
	float phi = 2 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / ((a * a - 1) * Xi.y + 1.0));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	vec3 H = vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

	vec3 up = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 tangent = normalize(cross(up, N));
	vec3 biTangent = cross(N, tangent);

	return tangent * H.x + biTangent * H.y + N * H.z;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float a = roughness;
	float k = (a * a) / 2.0;
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}

vec2 BRDFIntegration(float NdotV, float roughness)
{
	vec3 V = vec3(sqrt(1.0f - NdotV * NdotV), 0, NdotV);
	float A = 0.0f;
	float B = 0.0f;

	vec3 N = vec3(0., 0, 1);
	const uint sampleCount = 1024u;
	for (uint i = 0; i < sampleCount; ++i)
	{
		vec2 Xi = Hammersley(i, sampleCount);
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L = reflect(-V, H);

		float NdotL = max(L.z, 0.0);
		float NdotH = max(H.z, 0.0);
		float VdotH = max(dot(V, H), 0.0);
		
		if (NdotL > 0.0)
		{
			float G = GeometrySmith(N, V, L, roughness);
			float G_Vis = G * VdotH / (NdotV * NdotH);
			float F = pow(1.0 - VdotH, 5.0);
			A += (1 - F) * G_Vis;
			B += F * G_Vis;
		}
	}
	
	return vec2(A / sampleCount, B / sampleCount);
}
# version 440

const float PI = 3.14159265359;

in vec3 fragPosition;

out vec3 fragColor;

uniform samplerCube tex_environment;
uniform float roughness;

vec2 Hammersley(uint i, uint N);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
float DistributionGGX(vec3 N, vec3 H, float roughness);

void main()
{
	vec3 N = normalize(fragPosition);
	vec3 V = N;
	vec3 R = V;

	const uint sampleCount = 1024u;
	vec3 prefilteredColor = vec3(0);
	float totalWeight = 0.0;

	for (uint i = 0; i < sampleCount; ++i)
	{
		vec2 Xi = Hammersley(i, sampleCount);
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L = reflect(-V, H);

		float NdotL = max(dot(N, L), 0.0);

		if (NdotL > 0)
		{
			float D   = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

            float resolution = 512.0;
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(sampleCount) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
			
			prefilteredColor += textureLod(tex_environment, L, mipLevel).xyz * NdotL;
			totalWeight += NdotL;
		}
	}

	fragColor = prefilteredColor / totalWeight;
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
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness * roughness;
	float phi = 2 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / ((a * a - 1) * Xi.y + 1.0));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);

	vec3 H = vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

	vec3 up = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 tangent = normalize(cross(up, N));
	vec3 biTangent = cross(N, tangent);

	return tangent * H.x + biTangent * H.y + N * H.z;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
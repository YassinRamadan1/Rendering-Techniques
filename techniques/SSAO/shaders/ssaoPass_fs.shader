#version 440

out float fragColor;

uniform sampler2D texture_position;
uniform sampler2D texture_normal;

uniform sampler2D texture_noise;
uniform vec3 samples[64];
uniform mat4 projection;
uniform vec2 resolution;
uniform float radius;
uniform float bias;

void main()
{
	vec2 textureCoordinates = gl_FragCoord.xy / resolution;

	vec3 normal = texture(texture_normal, textureCoordinates).xyz;

	vec3 randomVector = texture(texture_noise, textureCoordinates * resolution / 4.).xyz;
	vec3 tangent = normalize(randomVector - dot(randomVector, normal) * normal);
	vec3 biTangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, biTangent, normal);
	
	vec3 fragPosition = texture(texture_position, textureCoordinates).xyz;
	float occlusionFactor = 0.;
	for(int i = 0; i < 64; ++i)
	{
		vec3 samp = tbn * samples[i];

		samp = fragPosition + samp * radius;

		vec4 screenSpacePoint = projection * vec4(samp, 1.);
		
		screenSpacePoint /= screenSpacePoint.w;
		
		screenSpacePoint = screenSpacePoint * 0.5 + 0.5;
		
		float samplePositionDepth = texture(texture_position, screenSpacePoint.xy).z;
		
		float rangeCheck = smoothstep(0., 1., radius / abs(fragPosition.z - samplePositionDepth));
		occlusionFactor += (samplePositionDepth >= samp.z + bias ? 1. : 0.) * rangeCheck;
	}

	fragColor = 1.0 - (occlusionFactor / 64.0);
}
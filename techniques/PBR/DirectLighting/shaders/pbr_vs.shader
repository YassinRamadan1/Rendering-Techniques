#version 440

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_biTangent;
layout (location = 4) in vec2 in_textureCoordinates;

out vec3 tangent_fragPosition;
out vec2 textureCoordinates;
out vec3 tangent_lightPosition[4];
out vec3 tangent_cameraPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition[4];
uniform vec3 cameraPosition;

void main()
{
	textureCoordinates = in_textureCoordinates;

	mat3 mode = mat3(model);
	vec3 N = normalize(transpose(inverse(mode)) * in_normal);
	vec3 T = mode * in_tangent;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	
	mat3 TBN = transpose(mat3(T, B, N));

	for (int i = 0; i < 4; ++i)
	{
		tangent_lightPosition[i] = TBN * lightPosition[i];
	}

	tangent_cameraPosition = TBN * cameraPosition;

	vec4 fragPos = model * vec4(in_position, 1.);
	tangent_fragPosition = TBN * fragPos.xyz;

	gl_Position = projection * view * fragPos;
}
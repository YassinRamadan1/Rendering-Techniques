#version 440

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_biTangent;
layout (location = 4) in vec2 in_textureCoordinates;

out vec3 fragPosition;
out vec2 textureCoordinates;
out mat3 TBN;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	textureCoordinates = in_textureCoordinates;

	mat3 mode = mat3(model);
	vec3 N = normalize(transpose(inverse(mode)) * in_normal);
	vec3 T = mode * in_tangent;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);

	fragPosition = (model * vec4(in_position, 1.)).xyz;

	gl_Position = projection * view * vec4(fragPosition, 1.0);
}
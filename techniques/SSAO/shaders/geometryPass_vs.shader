#version 440
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_biTangent;
layout (location = 4) in vec2 in_textureCoordinates;

out VS_OUT
{
	vec2 textureCoordinates;
	vec3 fragPosition;
	vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.normal = normalize(transpose(inverse(mat3(view * model))) * in_normal);
	vs_out.textureCoordinates = in_textureCoordinates;
	vec4 fragPos = view * model * vec4(in_position, 1.);
	vs_out.fragPosition = fragPos.xyz;
	gl_Position = projection * fragPos;
}
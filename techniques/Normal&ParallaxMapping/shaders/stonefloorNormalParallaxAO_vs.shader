#version 440
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_biTangent;
layout (location = 4) in vec2 in_textureCoordinates;

out VS_OUT
{
	vec2 textureCoordinates;
	vec3 lightPosition;
	vec3 cameraPosition;
	vec3 fragPosition;
} vs_out;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat3 mode = mat3(model);
	vec3 normal = normalize(transpose(inverse(mode)) * in_normal);
	vec3 tangent = mode * in_tangent;
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 biTangent = cross(normal, tangent);

	mat3 tbn = transpose(mat3(tangent, biTangent, normal));
	vs_out.textureCoordinates = in_textureCoordinates;
	vs_out.cameraPosition = tbn * cameraPosition;
	vs_out.lightPosition = tbn * lightPosition;

	vec4 fragPos = model * vec4(in_position, 1.);
	vs_out.fragPosition = tbn * fragPos.xyz;
	gl_Position = projection * view * fragPos;
}
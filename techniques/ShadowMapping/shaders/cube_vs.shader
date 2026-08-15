#version 440
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TextureCoordinates;

out vec2 textureCoordinates;
out vec3 fragPosition;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	normal = transpose(inverse(mat3(model))) * in_Normal;
	textureCoordinates = in_TextureCoordinates;
	fragPosition = (model * vec4(in_Position, 1.0f)).xyz;
	gl_Position = projection * view * vec4(fragPosition, 1.);
}
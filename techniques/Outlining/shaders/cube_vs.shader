#version 440

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	textureCoordinates = in_TextureCoordinates;
	gl_Position = projection * view * model * vec4(in_Position, 1.0f);
}

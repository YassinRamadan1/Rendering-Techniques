#version 440

layout (location = 0) in vec2 in_Position;

void main()
{
	gl_Position = vec4(in_Position, 0.0f, 1.0f);
}

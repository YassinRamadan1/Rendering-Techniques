#version 440

layout (location = 0) in vec3 in_position;

out vec3 fragPosition;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragPosition = in_position;

	gl_Position = projection * mat4(mat3(view)) * vec4(in_position, 1.);
	gl_Position = gl_Position.xyww;
}
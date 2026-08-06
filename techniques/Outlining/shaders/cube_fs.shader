#version 440

in vec2 textureCoordinates;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex, textureCoordinates);
}
# version 440

in vec3 fragPosition;

out vec3 fragColor;

uniform sampler2D tex_equiRectangular;

void main()
{
	vec3 direction = normalize(fragPosition);

	vec2 textureCoordinates = vec2(atan(direction.z, direction.x), asin(direction.y));
	textureCoordinates *= vec2(0.1591, 0.3183);

	fragColor = texture(tex_equiRectangular, textureCoordinates + vec2(0.5)).rgb;
}
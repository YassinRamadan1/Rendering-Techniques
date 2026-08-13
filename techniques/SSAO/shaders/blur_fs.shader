#version 440

out float fragColor;

uniform sampler2D texture_ao;
uniform vec2 resolution;

void main()
{
    vec2 textureCoordinates = gl_FragCoord.xy / resolution;

    vec2 offset = 1.0f / textureSize(texture_ao, 0);
    
    float color = 0.f;
    for (int i = -2; i < 2; ++i)
    {
        for (int j = -2; j < 2; ++j)
        {
            color += texture(texture_ao, textureCoordinates + vec2(i, j) * offset).r;
        }
    }

    fragColor = color / 16.0;
}
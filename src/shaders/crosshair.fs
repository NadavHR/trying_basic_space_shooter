#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec2 position;
uniform sampler2D screenTexture;

void main()
{
    vec2 positionRelative = TexCoords - position;
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++)
    // {
    //     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    // }
    vec3 col =  vec3(texture(screenTexture, TexCoords.st));
    // for(int i = 0; i < 9; i++) {
    //     col += sampleTex[i] * kernel[i];
    // }
    float dis = length(positionRelative);
    if (dis < 0.05 ) {
        FragColor = vec4(clamp(col.r + dis/0.05, 0, 1), col.g, col.b, 1.0);
    } else {
        FragColor = vec4(col, 1.0);
    }
} 
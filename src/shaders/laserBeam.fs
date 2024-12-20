#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 texCoord;
in vec3 FragPos;
in mat3 TBN;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
void main()
{    
    FragColor = 5 * texture(texture_diffuse1, texCoord);
    BrightColor = 5 * texture(texture_diffuse1, texCoord);
}
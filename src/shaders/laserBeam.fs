#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 FragPos;
in mat3 TBN;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
void main()
{    
    FragColor = texture(texture_diffuse1, texCoord);
}
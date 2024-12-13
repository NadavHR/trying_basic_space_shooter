#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
uniform sampler2D gPosition;
out vec2 TexCoords;
out vec3 FragPos;
void main()
{
    TexCoords = aTexCoords;
    FragPos = texture(gPosition, TexCoords).rgb;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  
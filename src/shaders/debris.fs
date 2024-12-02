#version 330 core
out vec4 FragColor;

uniform float time, duration;
in vec3 fColor;

void main()
{
    FragColor = vec4(fColor * (duration - time), 1.0);
}
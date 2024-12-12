#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;


out vec3 fColor;
uniform float time;
uniform vec3 position;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 offsets[100];
uniform vec3 speeds[100];

void main()
{
    vec3 offset = offsets[gl_InstanceID] + position + (speeds[gl_InstanceID] * time);
    gl_Position = projection * view * vec4(aPos + offset, 1.0);
    fColor = aColor;
} 

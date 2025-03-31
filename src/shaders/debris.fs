#version 330 core
out vec4 FragColor;

uniform float time, duration;
in vec3 fColor;
uniform sampler2D frame;
uniform vec2 screenSize;

void main()
{
    vec3 backgroundColor = texture(frame, gl_FragCoord.xy / screenSize).rgb;
    float effectScalar = (duration - time) / duration;
    FragColor = vec4((fColor * effectScalar) + (backgroundColor * (1-effectScalar)), 1.0);
}
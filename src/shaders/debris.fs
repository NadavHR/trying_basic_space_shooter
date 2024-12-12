#version 330 core
out vec4 FragColor;

uniform float time, duration;
in vec3 fColor;
uniform sampler2D frame;
uniform vec2 screenSize;

void main()
{
    float effectScalar = (duration - time) / duration;
    // FragColor = vec4(gl_FragCoord.xy / screenSize, 0.0, 1.0);
    FragColor = vec4((fColor * effectScalar) + (texture(frame, gl_FragCoord.xy / screenSize).rgb * (1-effectScalar)), 1.0);
    // FragColor = vec4(1 - (texture(frame, framePos).rgb), 1.0);
}
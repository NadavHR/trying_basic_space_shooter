#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform float exposure;
uniform float gamma;
uniform sampler2D hdrBuffer;
uniform sampler2D bloomBuffer;
void main()
{             
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBuffer, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
  
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
    // FragColor = vec4(hdrColor, 1.0);
}  
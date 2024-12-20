
#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D hdrBuffer;
uniform sampler2D brightBuffer;

uniform vec3 Position;
uniform vec3 Color;
uniform vec3 viewPos;
uniform float quadratic, linear, lightThreshold;
void main()
{             
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 color = texture(hdrBuffer, TexCoords).rgb;
    vec3 bright =  texture(brightBuffer, TexCoords).rgb;

    float distance = length(Position - FragPos);
    vec3 viewDir  = normalize(viewPos - FragPos);
    
    // diffuse
    vec3 lightDir = normalize(Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * Color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
    vec3 specular = Color * spec * Specular;
    // attenuation
    float attenuation = 1.0 / (1.0 + linear * distance + quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    vec3 lighting = diffuse + specular;

    FragColor = vec4(color + lighting, 1.0);
    // FragColor = vec4(color, 1.0);

    if (length(FragColor.rgb) > lightThreshold) {
        BrightColor = vec4(bright + FragColor.rgb, 1.0);
    }
    else {
        BrightColor = vec4(0, 0, 0, 1);
    }
}  
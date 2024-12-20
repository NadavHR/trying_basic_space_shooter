#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 FragPos;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D gAlbedoSpec;

uniform vec3 Ambient;
uniform float AmbientStrength;

void main()
{             
    // retrieve data from G-buffer
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 Position = texture(gPosition, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    vec3 lighting = Ambient * AmbientStrength * Albedo; 
    
    FragColor = vec4(lighting, 1.0);
    // FragColor = vec4(Albedo, 1.0);
    // FragColor = vec4(Normal, 1.0);
    // Position.z *= -(1.0/150.0);
    // FragColor = vec4(Position, 1.0);
}  
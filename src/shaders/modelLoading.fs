#version 330 core
layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec3 gPosition;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

void main()
{    
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // gAlbedoSpec.rgb = texture(texture_normal1, TexCoords).rgb;
    // gAlbedoSpec.rgb = texture(texture_specular1, TexCoords).rgb;
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    vec3 norm = texture(texture_normal1, TexCoords).xyz * 2.0 - 1.0;
    norm = normalize(TBN * norm);
    gNormal = norm;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}  
#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;
in mat3 TBN;
uniform vec3 viewPos;
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;  

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform mat4 model;
void main()
{    
    // FragColor = texture(texture_diffuse1, texCoord);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, texCoord));
  	
    // diffuse 
    // vec3 norm = normalize(mat3(transpose(inverse(model))) * (normal + texture(texture_normal1, texCoord).xyz));
    vec3 norm = normalize(texture(texture_normal1, texCoord).xyz * 2.0 - 1.0);
    norm = normalize(TBN * norm);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, texCoord).rgb;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(texture_specular1, texCoord).rgb;  
    // vec3 result =  (ambient + diffuse) + specular;
    float distance = length(light.position - FragPos);

    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));    
    
    vec3 result = diffuse + ambient + specular;
    result *= attenuation;

    FragColor = vec4(result, 1.0);
}
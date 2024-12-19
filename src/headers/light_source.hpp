#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP
#include "headers/shader.hpp"

enum LightType {
    DIRECTIONAL_LIGHT,
    POINT_LIGHT
};

class LightSource
{
private:
    LightType mlightType;
    glm::vec3 mlightVector;
    glm::vec3 mlightColor;
    float mlightIntensity;
    Shader* mshader;
    static Shader* directionalLightShader;
    static Shader* pointLightShader;
    
public:
    static void setViewPos(glm::vec3 viewPos);
    void use();
    LightSource(LightType lightType);
    ~LightSource();
};

#endif /* LIGHT_SOURCE_HPP */

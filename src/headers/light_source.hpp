#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP
#include "headers/shader.hpp"

class LightSource
{
private:
    static Shader* pointLightShader;
    static glm::vec3 mviewPos;
    
public:
    Shader* mshader;
    glm::vec3 mlightPosition;
    glm::vec3 mlightColor;
    float mlightLinearIntensity;
    float mlightQuadraticIntensity;
    float mlightThreshold;
    static void setViewPos(glm::vec3 viewPos);
    void use();
    LightSource();
    ~LightSource();
};

#endif /* LIGHT_SOURCE_HPP */

#include "../headers/light_source.hpp"

Shader * LightSource::pointLightShader = NULL;
glm::vec3 LightSource::mviewPos = glm::vec3(0.0);

LightSource::LightSource()
{
    if (pointLightShader == NULL) { // static member values are initialized to NULL by default 
        pointLightShader = new Shader("shaders/pointLight.vs", "shaders/pointLight.fs"); // this value never needs to be cleared until the end of the program
    }
    mshader = pointLightShader;

}

LightSource::~LightSource()
{
}

void LightSource::use() {
    mshader->use();
    mshader->setVec3("viewPos", mviewPos);
    glm::vec3 position = mlightPosition;
    mshader->setVec3("Position", position);
    mshader->setVec3("Color", mlightColor);
    mshader->setFloat("quadratic", mlightQuadraticIntensity);
    mshader->setFloat("linear", mlightLinearIntensity);

}

void LightSource::setViewPos(glm::vec3 viewPos) {
    mviewPos = viewPos;
}
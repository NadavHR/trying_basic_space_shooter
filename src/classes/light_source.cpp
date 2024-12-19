#include "../headers/light_source.hpp"

LightSource::LightSource(LightType lightType)
{
    switch (lightType)
    {
    case DIRECTIONAL_LIGHT:
        if (directionalLightShader == NULL) { // static member values are initialized to NULL by default 
            directionalLightShader = new Shader("shaders/directionalLight.vs", "shaders/directionalLight.fs"); // this value never needs to be cleared until the end of the program
        }
        mshader = directionalLightShader;
        break;
    
    case POINT_LIGHT:
        if (pointLightShader == NULL) { // static member values are initialized to NULL by default 
            pointLightShader = new Shader("shaders/pointLight.vs", "shaders/pointLight.fs"); // this value never needs to be cleared until the end of the program
        }
        break;
    }
}

LightSource::~LightSource()
{
}

void LightSource::use() {
    mshader->use();
}
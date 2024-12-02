#include "../headers/static_utils.hpp"

float timing::deltaTime = 0;
float timing::lastFrame = 0;
Renderer* rendering::renderer = NULL;
glm::mat4 rendering::projection = NULL;
glm::mat4 rendering::view = NULL;
glm::vec3 rendering::viewPos = glm::vec3(0,0,0);

Shader * shaders::modelLoadingShader = NULL;
Shader * shaders::safeGetModelLoadingShader() {
    if (modelLoadingShader == NULL) {
        modelLoadingShader = new Shader("shaders/modelLoading.vs", "shaders/modelLoading.fs");
    }
    return modelLoadingShader;
}
Shader * shaders::debrisParticleEffectsShader = NULL;
Shader * shaders::safeGetDebrisShader() {
    if (debrisParticleEffectsShader == NULL) {
        debrisParticleEffectsShader = new Shader("shaders/debris.vs", "shaders/debris.fs");
    }
    debrisParticleEffectsShader->use();
    for(unsigned int i = 0; i < effectOptions::PARTICLE_COUNT; i++)
    {
        debrisParticleEffectsShader->setVec3(("offsets[" + std::to_string(i) + "]"), glm::normalize(glm::vec3(RAND_RANGE_F(-1, 1), RAND_RANGE_F(-1, 1), RAND_RANGE_F(-1, 1))));
        debrisParticleEffectsShader->setVec3(("speeds[" + std::to_string(i) + "]"), glm::normalize(glm::vec3(RAND_RANGE_F(-1, 1), RAND_RANGE_F(-1, 1), RAND_RANGE_F(-1, 1))));
    }  
    debrisParticleEffectsShader->setVec3("viewPos", rendering::viewPos);
    debrisParticleEffectsShader->setTransform("view", rendering::view);
    debrisParticleEffectsShader->setProjection(rendering::projection);
    return debrisParticleEffectsShader;
}
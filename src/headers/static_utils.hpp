#ifndef STATIC_UTILS_HPP
#define STATIC_UTILS_HPP
#include "shader.hpp"
#include "renderer.hpp"
#define RAND_RANGE_F(low, high)  (low + (float)(rand()) / ( (float)(RAND_MAX/(high-(low)))))

namespace timing {
    // timing
    extern float deltaTime;
    extern float lastFrame;
}

namespace screen {
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;  
}

namespace rendering {
    extern Renderer* renderer;
    extern glm::mat4 projection;
    extern glm::mat4 view;
    extern glm::vec3 viewPos;
}

namespace shaders {
    extern Shader * modelLoadingShader;
    extern Shader * safeGetModelLoadingShader();
    extern Shader * debrisParticleEffectsShader;
    extern Shader * safeGetDebrisShader();
}

namespace effectOptions {
    const unsigned int PARTICLE_COUNT = 100; 
}

#endif /* STATIC_UTILS_HPP */
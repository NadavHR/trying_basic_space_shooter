#ifndef STATIC_UTILS_HPP
#define STATIC_UTILS_HPP
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
    #include "renderer.hpp"
    extern Renderer * renderer;
}

#endif /* STATIC_UTILS_HPP */

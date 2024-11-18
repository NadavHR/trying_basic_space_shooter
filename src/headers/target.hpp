#ifndef  TARGET_HPP
#define  TARGET_HPP
#include "model_render_object.hpp"
#include "crosshair.hpp"
#include <vector>

class Target {
    private:
        static std::vector<Target *> targets;
        ModelRenderObject mmodel;
        Shader mshader;
        glm::vec3 mvelocity = glm::vec3(0.0, 0.0, 0.0);
        float mhitRadius;
    
    public:
        Target();
        ~Target();  // add score tracking here
        bool isHit(const Crosshair& crosshair );
};

#endif /*  TARGET_HPP */

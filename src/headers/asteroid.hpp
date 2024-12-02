#ifndef  ASTEROID_HPP
#define  ASTEROID_HPP
#include "model_render_object.hpp"
#include "crosshair.hpp"
#include "static_utils.hpp"
#include "particle_effect.hpp"
#include <vector>
#include <random>

const float INITIAL_SPEED = 30, SPEED_MULTIPLIER = 1.05,
     DEFAULT_HIT_RADIUS = 2.7, DEFAULT_DISTANCE = 150.0, SPAWN_TIME_SEC = 1.5,
     X_RANGE = 36, Y_RANGE = 25.5;
class Asteroid {
    private:
        static std::vector<Asteroid *> targets;
        static ModelRenderObject * basicModel;
        size_t mindex;
        Shader& mshader;
        glm::vec3 mvelocity = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 mposition;
        const float mhitRadius;
        void periodic(float deltaTimeSec);
    
    public:
        Asteroid(const glm::vec3 spaceshipPosition);
        ~Asteroid();  // add score tracking here
        bool isHit(const Crosshair& crosshair );
       static void allPeriodic(const Crosshair& crosshair, const glm::vec3 spaceshipPosition, const bool shooting);
};

#endif /*  ASTEROID_HPP */

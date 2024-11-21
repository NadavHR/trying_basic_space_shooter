#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP
#include "../headers/model_render_object.hpp"
#include "../headers/shader.hpp"
#include <glm/glm.hpp>
#include <filesystem>
#include "../headers/crosshair.hpp"
#include "renderer.hpp"
#include "static_utils.hpp"

const float X_BOUND = 1.8, Y_BOUND = 1.3, 
            MAX_VEL = 3.0, AIM_EFFECT_STRENGTH = 10000.0,
            SHOOT_COOLDOWN_SEC = 0.18, SHOOT_EFFECT_TIME_SEC = 0.18, // SHOOT_EFFECT_TIME_SEC needs to be smaller or equal to SHOOT_COOLDOWN_SEC for an effect to occure
            LASER_SPEED = 80.0, DEFAULT_LIGHT_DISTANCE = 4.0;
class Spaceship {
    private:
        ModelRenderObject mmodel;
        ModelRenderObject mlaserModel;
        Shader mlaserShader;
        Shader& mshader;
        glm::vec3 mvelocity = glm::vec3(0.0, 0.0, 0.0);
        Crosshair &mcrosshair;
        float mdecel = 1.7  ; // the direction is always opposite to the speed 
        float maccel = 1.2; // the direction is always the direction of the input
        float mxInput = 0.0, myInput = 0.0; // the current input
        bool misShooting = false;
        float mlastShotSec;

        

    public:
        Spaceship(Crosshair & crosshair);
        ModelRenderObject& getRenderObject();
        void periodic(float deltaTimeSec);
        void inputX(float x);
        void inputY(float y);
        void shoot(bool isShooting);
};

#endif /* SPACESHIP_HPP */

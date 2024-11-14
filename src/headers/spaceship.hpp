#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP
#include "../headers/model_render_object.hpp"
#include "../headers/shader.hpp"
#include <glm/glm.hpp>
#include <filesystem>

const float X_BOUND = 1.8, Y_BOUND = 1.3, MAX_VEL = 3.0;
class Spaceship {
    private:
        ModelRenderObject mmodel;
        Shader mshader;
        glm::vec3 mvelocity = glm::vec3(0.0, 0.0, 0.0);
        float mdecel = 1.7  ; // the direction is always opposite to the speed 
        float maccel = 1.2; // the direction is always the direction of the input
        float mxInput = 0.0, myInput = 0.0; // the current input
        

    public:
        Spaceship(glm::mat4 projection);
        ModelRenderObject& getRenderObject();
        void periodic(float deltaTimeSec, glm::mat4 viewMatrix, glm::vec3 camPosition);
        void inputX(float x);
        void inputY(float y);
};

#endif /* SPACESHIP_HPP */

#ifndef CROSSHAIR_HPP
#define CROSSHAIR_HPP
#include "camera.hpp"
#include <glm/glm.hpp>
class Crosshair {
    private:
        unsigned int mscreenWidth;
        unsigned int mscreenHeight;
        float screenRatio;
        Camera& mcam;
        glm::uvec2 mscreenPos;


    public:
        Crosshair(unsigned int screenWidth, unsigned int screenHeight, Camera& cam);
        void setScreenPos(unsigned int x, unsigned int y);
        glm::uvec2 getScreenPos();
        glm::vec2 getNormalizedScreenCoords();
        glm::vec3 getNormalizedDirection();
        glm::vec3 getPlanarDirectionVector();
        unsigned int getScreenHeight();
        unsigned int getScreenWidth();
        Camera& getCam();

};

#endif /* CROSSHAIR_HPP */

#ifndef CROSSHAIR_HPP
#define CROSSHAIR_HPP
#include "camera.hpp"
#include "shader.hpp"
#include "screen_renderer.hpp"
#include <glm/glm.hpp>
#include "static_utils.hpp"
class Crosshair {
    private:
        unsigned int mscreenWidth;
        unsigned int mscreenHeight;
        float screenRatio;
        Camera& mcam;
        glm::uvec2 mscreenPos;
        Shader mcrosshairShader;
        ScreenRenderer mscreenRenderer;



    public:
        Crosshair(unsigned int screenWidth, unsigned int screenHeight, Camera& cam);
        void setScreenPos(unsigned int x, unsigned int y);
        glm::uvec2 getScreenPos();
        glm::vec2 getNormalizedScreenCoords();
        glm::vec3 getNormalizedDirection() const;
        glm::vec3 getPlanarDirectionVector() const;
        unsigned int getScreenHeight();
        unsigned int getScreenWidth();
        Camera& getCam();
        void periodic();

};

#endif /* CROSSHAIR_HPP */

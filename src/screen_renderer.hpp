#ifndef SCREEN_RENDERER_HPP
#define SCREEN_RENDERER_HPP
#include "renderer.hpp"
class ScreenRenderer {
    public:
        ScreenRenderer(Renderer * pRenderer, Shader * pShader);
        void render();
        ~ScreenRenderer();

        Shader * getPShader() const;
        void setPShader(Shader * mpShader_);

        Renderer * getPRenderer() const;
        void setPRenderer(Renderer * mpRenderer_);

        unsigned int getQuadVAO() const;
        unsigned int getQuadVBO() const;
    
    private:
        Shader * mpShader;
        Renderer * mpRenderer;
        unsigned int mQuadVAO;
        unsigned int mQuadVBO;


};

#endif // SCREEN_RENDERER_HPP

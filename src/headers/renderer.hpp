#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "irender_object.hpp"
#include "shader.hpp"
#include <vector>
#include <glad/glad.h>

class Renderer {
    public:
        Renderer(unsigned int screenWidth, unsigned int screenHeight);
        void render(); 
        void renderTarget(IRenderObject &target);
        void renderTargetForward(IRenderObject &target);
        void clear();
        ~Renderer();
        void addRenderObject(IRenderObject * object);
        unsigned int getTexture() const;
        unsigned int getFrameBuffer() const;
        unsigned int getRenderBuffer() const;
        glm::vec3 mambientColor;
        float mambientStrength;
    
    private:
        unsigned int mFrameWidth, mFrameHeight;
        std::vector<IRenderObject*> mRenderObjects;
        Shader mambientLightShader;
        unsigned int mTexture;  // the texture being written to
        unsigned int mforwardFBO;  // Frame Buffer Object
        unsigned int mforwardRBO;  // Render Buffer Object
        unsigned int mgBuffer; // the g buffer
        unsigned int mgPosition, mgNormal, mgAlbedoSpec; // deferred rendering buffers 
        unsigned int mQuadVAO, mQuadVBO;  // VAO and VBO for rendering final effects to screen
        unsigned int mdefferedRBO;  // the RBO of the deferred rendering

        
};

#endif // RENDERER_HPP

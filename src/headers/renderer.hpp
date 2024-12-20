#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "irender_object.hpp"
#include "shader.hpp"
#include "light_source.hpp"
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
        void addForwardRenderObject(IRenderObject * object);
        void addLightSource(LightSource * source);
        unsigned int getTexture() const;
        unsigned int getFrameBuffer() const;
        unsigned int getRenderBuffer() const;
        glm::vec3 mambientColor;
        float mambientStrength;
        float mgamma;
        float mexposure;
    
    private:
        unsigned int mFrameWidth, mFrameHeight;
        std::vector<IRenderObject*> mforwardRenderObjects;
        std::vector<IRenderObject*> mRenderObjects;
        std::vector<LightSource*> mlightSources;
        Shader mambientLightShader;
        Shader mtoneMappingShader;
        Shader mblurShader;
        unsigned int mbloomAmmount;

        unsigned int mTexture;  // the final texture being written to
        unsigned int mforwardFBO;  // Frame Buffer Object
        unsigned int mforwardRBO;  // Render Buffer Object

        unsigned int mpingpongFBO[2];  // FBO for ping pong buffers
        unsigned int mpingpongBuffer[2]; // buffers of ping pong buffers

        unsigned int mhdrTexture;  // the hdr texture being written to
        unsigned int mbrightTexture; // the texture with all really bright pixels
        unsigned int mhdrFBO;  // Frame Buffer Object for hdr
        unsigned int mhdrRBO;  // Render Buffer Object for hdr

        unsigned int mgBuffer; // the g buffer
        unsigned int mgPosition, mgNormal, mgAlbedoSpec; // deferred rendering buffers 
        unsigned int mdefferedRBO;  // the RBO of the deferred rendering

        unsigned int mQuadVAO, mQuadVBO;  // VAO and VBO for rendering final effects to screen 
        
};

#endif // RENDERER_HPP

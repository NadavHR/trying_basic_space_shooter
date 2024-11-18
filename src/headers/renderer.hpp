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
        void clear();
        ~Renderer();
        void addRenderObject(IRenderObject * object);
        unsigned int getTexture() const;
        unsigned int getFrameBuffer() const;
        unsigned int getRenderBuffer() const;
    
    private:
        unsigned int mFrameWidth, mFrameHeight;
        std::vector<IRenderObject*> mRenderObjects;
        unsigned int mTexture;  // the texture being written to
        unsigned int mFBO;  // Frame Buffer Object
        unsigned int mRBO;  // Rendr Buffer Object
        
};

#endif // RENDERER_HPP

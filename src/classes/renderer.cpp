#include "../headers/renderer.hpp"

Renderer::Renderer(unsigned int frameWidth, unsigned int frameHeight) 
{
    mFrameWidth = frameWidth;
    mFrameHeight = frameHeight;

    

    // generate frame buffer object
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO); 

    // generate texture to attach to frame buffer object
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    // attach texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

    // generate render buffer object
    glGenRenderbuffers(1, &mRBO);

    glBindRenderbuffer(GL_RENDERBUFFER, mRBO);  

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    // attach render buffer to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);

    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    

}

void Renderer::clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

void Renderer::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    // render
    // ------
    for (IRenderObject * object : mRenderObjects) {
        object->render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Renderer::renderTarget(IRenderObject &target) {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    // render
    // -----
    target.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int Renderer::getTexture() const {
    return mTexture;
}
unsigned int Renderer::getFrameBuffer() const {
    return mFBO;
}
unsigned int Renderer::getRenderBuffer() const {
    return mRBO;
}
Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &mFBO);  
    glDeleteRenderbuffers(1, &mRBO);

}

void Renderer::addRenderObject(IRenderObject *object)
{
    mRenderObjects.push_back(object);
}


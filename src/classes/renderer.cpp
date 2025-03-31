#include "../headers/renderer.hpp"

#define SET_TEXTURE(name, i, textureID) \
glActiveTexture(GL_TEXTURE##i); \
glUniform1i(glGetUniformLocation(shaderID, name), i); \
glBindTexture(GL_TEXTURE_2D, textureID);

#define BIND_GBUFFER_TEXTURES() \
SET_TEXTURE("gNormal", 0, mgNormal) \
SET_TEXTURE("gPosition", 1, mgPosition) \
SET_TEXTURE("gAlbedoSpec", 2, mgAlbedoSpec)

static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};


Renderer::Renderer(unsigned int frameWidth, unsigned int frameHeight) : mambientLightShader("shaders/ambientLightDeferred.vs",
                                                                                            "shaders/ambientLightDeferred.fs"),
                                                                        mtoneMappingShader("shaders/toneMapper.vs", 
                                                                                           "shaders/toneMapper.fs"),
                                                                        mblurShader("shaders/gaussBlur.vs", "shaders/gaussBlur.fs")
{
    mFrameWidth = frameWidth;
    mFrameHeight = frameHeight;
    mambientColor = glm::vec3(1,1,1);
    mambientStrength = 0.05;
    mbloomAmmount = 5;

    mgamma = 2.2;
    mexposure = 1.0;

    #define GEN_HDR_TEXTURE(texture, attachment) \
    glGenTextures(1, &texture); \
    glBindTexture(GL_TEXTURE_2D, texture); \
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, NULL); \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); \
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT##attachment, GL_TEXTURE_2D, texture, 0);


    // generate g buffer frame buffer
    glGenFramebuffers(1, &mgBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);

    // - normal color buffer
    GEN_HDR_TEXTURE(mgNormal, 0)

    // - position color buffer
    GEN_HDR_TEXTURE(mgPosition, 1)
    
    // - color + specular color buffer
    glGenTextures(1, &mgAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, mgAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mgAlbedoSpec, 0);
    
    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachmentsG[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachmentsG);

    // - depth buffer 
    glGenRenderbuffers(1, &mdefferedRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mdefferedRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mdefferedRBO);

    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    // generate forward frame buffer object
    glGenFramebuffers(1, &mforwardFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mforwardFBO); 

    // generate texture to attach to frame buffer object
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    // attach texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

    // generate render buffer object
    glGenRenderbuffers(1, &mforwardRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mforwardRBO);  
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    // attach render buffer to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mforwardRBO);

    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    // generate general use frame buffer object
    glGenFramebuffers(1, &mgeneralUseFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mgeneralUseFBO); 

    // generate texture to attach to frame buffer object
    glGenTextures(1, &mgeneralUseTexture);
    glBindTexture(GL_TEXTURE_2D, mgeneralUseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    // attach texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mgeneralUseTexture, 0);

    // generate render buffer object
    glGenRenderbuffers(1, &mgeneralUseRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mgeneralUseRBO);  
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    // attach render buffer to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mgeneralUseRBO);
    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    // generate HDR frame buffer object
    glGenFramebuffers(1, &mhdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mhdrFBO); 

    // generate texture to attach to frame buffer object
    GEN_HDR_TEXTURE(mhdrTexture, 0)
    GEN_HDR_TEXTURE(mbrightTexture, 1)

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachmentsHDR[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachmentsHDR);

    // generate render buffer object
    glGenRenderbuffers(1, &mhdrRBO);

    glBindRenderbuffer(GL_RENDERBUFFER, mhdrRBO);  

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    // attach render buffer to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mhdrRBO);

    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    glGenFramebuffers(2, mpingpongFBO);
    glGenTextures(2, mpingpongBuffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mpingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, mpingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mpingpongBuffer[i], 0
        );
    }

    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    glGenVertexArrays(1, &mQuadVAO);
    glBindVertexArray(mQuadVAO);
    glGenBuffers(1, &mQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

}

void Renderer::clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, mforwardFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, mhdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::drawDeffered() {
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mdefferedRBO);
    // render
    // ------
    glEnable(GL_DEPTH_TEST); 
    for (IRenderObject * object : mRenderObjects) {
        object->render();
    }
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, mgBuffer);

}

void Renderer::applyLight() {
    glDisable(GL_DEPTH_TEST); 
    glBindFramebuffer(GL_FRAMEBUFFER, mhdrFBO);
    mambientLightShader.use();
    unsigned int shaderID = mambientLightShader.getProgramID();

    BIND_GBUFFER_TEXTURES()
    // also send light relevant uniforms
    mambientLightShader.setVec3("Ambient", mambientColor);
    mambientLightShader.setFloat("AmbientStrength", mambientStrength);
    glBindVertexArray(mQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    for (LightSource* light : mlightSources) {
        light->use();
        shaderID = light->mshader->getProgramID();
        BIND_GBUFFER_TEXTURES()
        SET_TEXTURE("hdrBuffer", 3, mhdrTexture);
        SET_TEXTURE("brightBuffer", 4, mbrightTexture)
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

}

void Renderer::DrawForward() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mgBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mhdrFBO);
    glBlitFramebuffer(0, 0, mFrameWidth, mFrameHeight, 0, 0, mFrameWidth, mFrameHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mforwardFBO);
    glBlitFramebuffer(0, 0, mFrameWidth, mFrameHeight, 0, 0, mFrameWidth, mFrameHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer(GL_FRAMEBUFFER, mhdrFBO);
    glEnable(GL_DEPTH_TEST); 
    for (IRenderObject * object : mforwardRenderObjects) {
        object->render();
    }
    glDisable(GL_DEPTH_TEST); 
}

void Renderer::render()
{
    drawDeffered();

    applyLight();

    DrawForward();


    // apply bloom
    glBindVertexArray(mQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    bool horizontal = true, first_iteration = true;
    mblurShader.use();
    for (unsigned int i = 0; i < 2 * mbloomAmmount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mpingpongFBO[horizontal]); 
        mblurShader.setInt("horizontal", horizontal);
        glBindTexture(
            GL_TEXTURE_2D, first_iteration ? mbrightTexture : mpingpongBuffer[!horizontal]
        ); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mforwardFBO);

    mtoneMappingShader.use();
    unsigned int shaderID = mtoneMappingShader.getProgramID();
    SET_TEXTURE("hdrBuffer", 0, mhdrTexture);
    SET_TEXTURE("bloomBuffer", 1, mpingpongBuffer[1]);
    mtoneMappingShader.setFloat("exposure", mexposure);
    mtoneMappingShader.setFloat("gamma", mgamma);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
}
void Renderer::renderTarget(IRenderObject &target) {
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);
    // render
    // -----
    target.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderTargetForward(IRenderObject &target) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mforwardFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mgeneralUseFBO);
    glBlitFramebuffer(0, 0, mFrameWidth, mFrameHeight, 0, 0, mFrameWidth, mFrameHeight, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, mgeneralUseFBO);
    // render
    // -----
    target.render();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mforwardFBO);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mgeneralUseFBO);
    glBlitFramebuffer(0, 0, mFrameWidth, mFrameHeight, 0, 0, mFrameWidth, mFrameHeight, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);
    GLenum err = glGetError();
    if(err == GL_INVALID_OPERATION ) {
        std::cout << "ERROR: invalid operation" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int Renderer::getTexture() const {
    return mTexture;
}
unsigned int Renderer::getFrameBuffer() const {
    return mforwardFBO;
}
unsigned int Renderer::getRenderBuffer() const {
    return mforwardRBO;
}
Renderer::~Renderer()
{
    glDeleteBuffers(1, &mQuadVBO);
    glDeleteVertexArrays(1, &mQuadVAO);
    unsigned int textures[] = {mgAlbedoSpec, mgPosition, mgNormal};
    glDeleteTextures(3, textures);
    glDeleteRenderbuffers(1, &mdefferedRBO);
    glDeleteFramebuffers(1, &mgBuffer);

    glDeleteFramebuffers(1, &mforwardFBO);  
    glDeleteRenderbuffers(1, &mforwardRBO);
    glDeleteTextures(1, &mTexture);

    glDeleteFramebuffers(1, &mgeneralUseFBO);  
    glDeleteRenderbuffers(1, &mgeneralUseRBO);
    glDeleteTextures(1, &mgeneralUseTexture);

    glDeleteFramebuffers(1, &mhdrFBO);
    glDeleteRenderbuffers(1, &mhdrRBO);
    glDeleteTextures(1, &mhdrTexture);

    glDeleteFramebuffers(2, mpingpongFBO);
    glDeleteBuffers(2, mpingpongBuffer);

}

void Renderer::addRenderObject(IRenderObject *object)
{
    mRenderObjects.push_back(object);
}

void Renderer::addForwardRenderObject(IRenderObject * object) 
{
    mforwardRenderObjects.push_back(object);
}

void Renderer::addLightSource(LightSource * lightSource) {
    mlightSources.push_back(lightSource);
}
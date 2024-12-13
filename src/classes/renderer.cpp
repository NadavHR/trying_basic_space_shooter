#include "../headers/renderer.hpp"

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
                                                                                            "shaders/ambientLightDeferred.fs")
{
    mFrameWidth = frameWidth;
    mFrameHeight = frameHeight;
    mambientColor = glm::vec3(1,1,1);
    mambientStrength = 1;

    // generate g buffer frame buffer
    glGenFramebuffers(1, &mgBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);

    // - normal color buffer
    glGenTextures(1, &mgNormal);
    glBindTexture(GL_TEXTURE_2D, mgNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mgNormal, 0);

    // - position color buffer
    glGenTextures(1, &mgPosition);
    glBindTexture(GL_TEXTURE_2D, mgPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mgPosition, 0);
    
    // - color + specular color buffer
    glGenTextures(1, &mgAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, mgAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mgAlbedoSpec, 0);
    
    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // - depth buffer RBO
    glGenRenderbuffers(1, &mdefferedRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mdefferedRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mdefferedRBO);

    // make sure the frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Frame buffer incomplete" << std::endl;
    }

    // generate frame buffer object
    glGenFramebuffers(1, &mforwardFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mforwardFBO); 

    // generate texture to attach to frame buffer object
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

void Renderer::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mgBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mdefferedRBO);
    // render
    // ------
    glEnable(GL_DEPTH_TEST); 
    for (IRenderObject * object : mRenderObjects) {
        object->render();
    }
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, mgBuffer);
    glDisable(GL_DEPTH_TEST); 

    glBindFramebuffer(GL_FRAMEBUFFER, mforwardFBO);
    mambientLightShader.use();
    unsigned int shaderID = mambientLightShader.getProgramID();

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderID, "gNormal"), 0);
    glBindTexture(GL_TEXTURE_2D, mgNormal);

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shaderID, "gPosition"), 1);
    glBindTexture(GL_TEXTURE_2D, mgPosition);

    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shaderID, "gAlbedoSpec"), 2);
    glBindTexture(GL_TEXTURE_2D, mgAlbedoSpec);
    // also send light relevant uniforms
    mambientLightShader.setVec3("Ambient", mambientColor);
    mambientLightShader.setFloat("AmbientStrength", mambientStrength);
    
    glBindVertexArray(mQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, mgBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mforwardFBO);
    glBlitFramebuffer(0, 0, mFrameWidth, mFrameHeight, 0, 0, mFrameWidth, mFrameHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    
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
    glBindFramebuffer(GL_FRAMEBUFFER, mforwardFBO);
    // render
    // -----
    target.render();

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
    glDeleteFramebuffers(1, &mforwardFBO);  
    glDeleteRenderbuffers(1, &mforwardRBO);
    glDeleteRenderbuffers(1, &mdefferedRBO);

}

void Renderer::addRenderObject(IRenderObject *object)
{
    mRenderObjects.push_back(object);
}


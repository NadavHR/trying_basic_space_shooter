#include "renderer.hpp"

//-----------------------------------------------------------------
// These are for displaying the rendered object on screen
//-----------------------------------------------------------------
// static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//     // positions   // texCoords
//     -1.0f,  1.0f,  0.0f, 1.0f,
//     -1.0f, -1.0f,  0.0f, 0.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,

//     -1.0f,  1.0f,  0.0f, 1.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f
// };
// unsigned int quadVAO, quadVBO;
// glGenVertexArrays(1, &quadVAO);
// glBindVertexArray(quadVAO);
// glGenBuffers(1, &quadVBO);
// glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
// glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);


// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
// glEnableVertexAttribArray(0);
// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
// glEnableVertexAttribArray(1);


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

void Renderer::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (IRenderObject &object : mRenderObjects) {
        object.render();
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

unsigned int Renderer::getTexture() const {
    return mTexture;
}

Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &mFBO);  
    glDeleteRenderbuffers(1, &mRBO);

}

void Renderer::addRenderObject(IRenderObject &object)
{
    mRenderObjects.push_back(object);
}


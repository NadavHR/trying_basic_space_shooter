#include "screen_renderer.hpp"

static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

ScreenRenderer::ScreenRenderer(Renderer * pRenderer, Shader * pShader)
{
    mpRenderer = pRenderer;
    mpShader = pShader;

    
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

ScreenRenderer::~ScreenRenderer() {
    glDeleteVertexArrays(1, &mQuadVAO);
    glDeleteBuffers(1, &mQuadVBO);
}

void ScreenRenderer::render()
{
    glEnable(GL_DEPTH_TEST);  // enable depth test 
    mpRenderer->render();
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    mpShader->use();
    glBindVertexArray(mQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glBindTexture(GL_TEXTURE_2D, mpRenderer->getTexture());	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

Renderer * ScreenRenderer::getPRenderer() const {
    return mpRenderer;

}

unsigned int ScreenRenderer::getQuadVBO() const {
    return mQuadVBO;
}

void ScreenRenderer::setPRenderer(Renderer * mpRenderer_) {
    mpRenderer = mpRenderer_;
}

Shader * ScreenRenderer::getPShader() const {
    return mpShader;
}

void ScreenRenderer::setPShader(Shader * mpShader_) {
    mpShader = mpShader_;
}

unsigned int ScreenRenderer::getQuadVAO() const {
    return mQuadVAO;
}

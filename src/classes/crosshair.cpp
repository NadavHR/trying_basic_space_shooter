#include "../headers/crosshair.hpp"

Crosshair::Crosshair(unsigned int screenWidth, unsigned int screenHeight, Camera& cam) : mcam(cam), mcrosshairShader("shaders/crosshair.vs", "shaders/crosshair.fs"),
    mscreenRenderer(rendering::renderer, &mcrosshairShader) {
    mscreenWidth = screenWidth;
    mscreenHeight = screenHeight;
    screenRatio = (float)screenWidth / (float)screenHeight;
    mscreenPos = glm::uvec2(screenWidth / 2, screenHeight / 2);
    mcrosshairShader.use();
    mcrosshairShader.setInt("screenTexture", 0);
}

void Crosshair::setScreenPos(unsigned int x, unsigned int y) {
    mscreenPos.x = x;
    mscreenPos.y = y;
}

glm::uvec2 Crosshair::getScreenPos() {
    return mscreenPos;
}
glm::vec3 Crosshair::getPlanarDirectionVector() {
    float screenX = ((float)mscreenPos.x - (float)(mscreenWidth / 2));  // focalLengthX * x / z
    float screenY = ((float)mscreenPos.y - (float)(mscreenHeight / 2)); // focalLengthY * y / z 
    float fovRadiansY = glm::radians(mcam.FovY);
    float focalLengthY = 2 * glm::tan(fovRadiansY*0.5);  // yPlane divided by z
    float focalLengthX = focalLengthY * screenRatio;     // xPlane divided by z
    focalLengthY = mscreenHeight / focalLengthY;         // z * h / yPlane  
    focalLengthX = mscreenWidth / focalLengthX;          // z * w / xPlane

    // we start by assuming z = 1 -> 
    // * screenX = focalLengthX * x,  screenY = focalLengthY * y
    float x = screenX / focalLengthX;
    float y = screenY / focalLengthY; 
    return glm::vec3(x, y, 1);
}

glm::vec3 Crosshair::getNormalizedDirection() {
    glm::vec3 normalizedDirection = getPlanarDirectionVector();
    normalizedDirection *= (1.0/glm::length(normalizedDirection));
    return normalizedDirection;
}

glm::vec2 Crosshair::getNormalizedScreenCoords() {
    float screenX = ((float)mscreenPos.x) / ((float)mscreenWidth);  
    float screenY = ((float)mscreenPos.y) / ((float)mscreenHeight);
    return glm::vec2(screenX, screenY);
}

unsigned int Crosshair::getScreenWidth(){
    return mscreenWidth;
}

unsigned int Crosshair::getScreenHeight() {
    return mscreenHeight;
}

Camera & Crosshair::getCam() {
    return mcam;
}

void Crosshair::periodic() {
    mcrosshairShader.use();
    mcrosshairShader.setVec2("position", getNormalizedScreenCoords());
    mscreenRenderer.renderScreenEffects();
}
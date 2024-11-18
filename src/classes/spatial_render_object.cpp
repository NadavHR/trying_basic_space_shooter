#include "../headers/spatial_render_object.hpp"

const glm::vec4 X_BASE_VECTOR(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 Y_BASE_VECTOR(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 Z_BASE_VECTOR(0.0f, 0.0f, 1.0f, 1.0f);

glm::vec3 SpatialRenderObject::getPosition() const {
    return mPosition;
}

void SpatialRenderObject::setPosition(const glm::vec3 &mPosition_) {
    mPosition = mPosition_;
    mTransformChanged = true;
}

glm::vec3 SpatialRenderObject::getRotationRad() const {
    return mRotationRad;
}

void SpatialRenderObject::setRotationRad(const glm::vec3 &mRotationRad_) {
    mRotationRad = mRotationRad_;
    mTransformChanged = true;
}

glm::mat4 SpatialRenderObject::getRotationTransform() {
    glm::mat4 rotation(1.0f);  // rotation

    rotation = glm::rotate(rotation, mRotationRad.z, glm::vec3(-Z_BASE_VECTOR));
    rotation = glm::rotate(rotation, mRotationRad.y, glm::vec3(-Y_BASE_VECTOR));
    rotation = glm::rotate(rotation, mRotationRad.x, glm::vec3(-X_BASE_VECTOR));
    return rotation;
}   

glm::mat4 SpatialRenderObject::getTransformationMatrixForRendering()
{
    glm::mat4 rotation(1.0f);  // rotation

    rotation = glm::rotate(rotation, mRotationRad.z, glm::vec3(Z_BASE_VECTOR));
    rotation = glm::rotate(rotation, mRotationRad.y, glm::vec3(Y_BASE_VECTOR));
    rotation = glm::rotate(rotation, mRotationRad.x, glm::vec3(X_BASE_VECTOR));
    

    glm::mat4 scale(1.0f);

    scale = glm::scale(scale, mScale);         // scale
    glm::mat4 translation(1.0f);
    glm::vec3 position(mPosition);
    position.z *= -1.0;  // this is so spatial render objects will have +z as camera front
    translation = glm::translate(translation, position);  // translate
    return translation * rotation * scale;
}

glm::vec3 SpatialRenderObject::getScale() const {
    return mScale;
}

void SpatialRenderObject::setScale(const glm::vec3 &mScale_) {
    mScale = mScale_;
    mTransformChanged = true;
}

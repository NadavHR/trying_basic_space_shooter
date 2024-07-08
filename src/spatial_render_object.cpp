#include "src/spatial_render_object.hpp"
#include "spatial_render_object.hpp"

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

glm::mat4 SpatialRenderObject::getTransformationMatrix()
{
    glm::mat4 rotation(1.0f);

    rotation = glm::rotate(rotation, mRotationRad.x, glm::vec3(X_BASE_VECTOR));
    rotation = glm::rotate(rotation, mRotationRad.y, glm::vec3(rotation * Y_BASE_VECTOR));
    rotation = glm::rotate(rotation, mRotationRad.z, glm::vec3(rotation * Z_BASE_VECTOR));

    glm::mat4 transformation(1.0f);

    transformation = glm::scale(transformation, mScale);         // scale
    transformation = rotation * transformation;                  // rotate
    transformation = glm::translate(transformation, mPosition);  // translate
    return transformation;
}

glm::vec3 SpatialRenderObject::getScale() const {
    return mScale;
}

void SpatialRenderObject::setScale(const glm::vec3 &mScale_) {
    mScale = mScale_;
    mTransformChanged = true;
}

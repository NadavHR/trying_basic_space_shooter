
#ifndef SPATIAL_RENDER_OBJECT_HPP
#define SPATIAL_RENDER_OBJECT_HPP
#include "irender_object.hpp"
class SpatialRenderObject : public IRenderObject {
    public:
        glm::vec3 getPosition() const;
        void setPosition(const glm::vec3 &mPosition_);

        glm::vec3 getScale() const;
        void setScale(const glm::vec3 &mScale_);

        glm::vec3 getRotationRad() const;
        void setRotationRad(const glm::vec3 &mRotationRad_);

        glm::mat4 getTransformationMatrixForRendering();
        glm::mat4 getRotationTransform();
    
    protected:
        bool mTransformChanged;

    private: 
        glm::vec3 mPosition;
        glm::vec3 mScale;
        glm::vec3 mRotationRad;
    
};

#endif // SPATIAL_RENDER_OBJECT_HPP

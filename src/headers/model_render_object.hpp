
#ifndef MODEL_RENDER_OBJECT_HPP
#define MODEL_RENDER_OBJECT_HPP

#include "spatial_render_object.hpp"
#include "model.hpp"
class ModelRenderObject : public SpatialRenderObject {
    public:
        ModelRenderObject(string const &path, Shader * shader, bool gamma = false);
        ~ModelRenderObject() override;
        Model * getModel();

    private:
        Model mModel;

    protected:
        void draw() override;

       
};

#endif // MODEL_RENDER_OBJECT_HPP

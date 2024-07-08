
#ifndef MODEL_RENDER_OBJECT_HPP
#define MODEL_RENDER_OBJECT_HPP

#include "src\spatial_render_object.hpp"
#include "src\model.hpp"
class ModelRenderObject : SpatialRenderObject {
    public:
        ModelRenderObject(string const &path, Shader * shader, bool gamma = false);

    private:
        Model mModel;

    protected:
        void draw() override;

};

#endif // MODEL_RENDER_OBJECT_HPP

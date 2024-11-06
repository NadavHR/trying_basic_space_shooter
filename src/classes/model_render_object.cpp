#include "../headers/model_render_object.hpp"

ModelRenderObject::ModelRenderObject(string const &path, Shader * shader, bool gamma) : mModel(path, gamma)
{
    mShader = shader;
}

void ModelRenderObject::draw() {

    if (mTransformChanged) {
        mModel.setModelTransformation(getTransformationMatrix());
        mTransformChanged = false;
    }
    
    mModel.Draw(*mShader);
}

ModelRenderObject::~ModelRenderObject() {
    mModel.~Model();
}
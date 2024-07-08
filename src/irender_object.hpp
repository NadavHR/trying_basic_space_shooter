#include "src\shader.hpp"
#ifndef IRENDER_OBJECT_HPP
#define IRENDER_OBJECT_HPP

class IRenderObject {
    public:
        virtual void render() {
            if (!mShader->isCurrentlyUsed()) {
                mShader->use();
            }
            draw();
        }

        virtual ~IRenderObject() = default;

        Shader * getShader() const { return mShader; };

    protected: 
        Shader * mShader;
        virtual void draw() {}


};


#endif // IRENDER_OBJECT_HPP

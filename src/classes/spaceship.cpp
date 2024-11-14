#include "../headers/spaceship.hpp"


Spaceship::Spaceship(glm::mat4 projection) : mshader("shaders/modelLoading.vs", "shaders/modelLoading.fs"), 
mmodel((std::filesystem::absolute("models/backpack/backpack.obj")).generic_string(), &mshader) {
    glm::vec3 lightPos(1.0, 1.0, 0.0);
    mshader.use();
    mshader.setProjection(projection);
    mshader.setVec3("light.position", lightPos);
    mshader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    mshader.setVec3("light.diffuse", glm::vec3(0.9f, 0.9f, 0.9f)); // darken diffuse light a bit
    mshader.setVec3("light.specular",glm::vec3(1.0f, 1.0f, 1.0f)); 
    mshader.setFloat("light.constant",  1.0f);
    mshader.setFloat("light.linear",    0.045f);
    mshader.setFloat("light.quadratic", 0.0032f);
}

void Spaceship::periodic(float deltaTimeSec, glm::mat4 viewMatrix, glm::vec3 camPosition) {
    glm::vec3 position = mmodel.getPosition();

    // technically not completely physically accurate but its fine 
    #define HANDLE_ACCEL_DECEL(a)  \
    if (m ##a##Input == 0.0) {   \
        float deltaDecel =  mdecel * deltaTimeSec; \
        if (abs( mvelocity.a ) >= deltaDecel) { \
            mvelocity.a -= (signbit( mvelocity.a ) ? -1.0 : 1.0 ) * deltaDecel; \
        } else { \
            mvelocity.a = 0; \
        } \
    } else {   \
        mvelocity.a += m ##a##Input * maccel * deltaTimeSec; \
    }

    HANDLE_ACCEL_DECEL(x);
    HANDLE_ACCEL_DECEL(y);

    position += mvelocity * deltaTimeSec;

    float xBound = X_BOUND, yBound = Y_BOUND;

    #define APPLY_BOUNDARY(a) \
    if (abs(position.a) > a##Bound) {\
        position.a = ( signbit( position.a ) ? -1.0 : 1.0 ) * a##Bound ;\
        mvelocity.a = 0.0; \
    } else {\
        mvelocity.a = min(max(mvelocity.a, -MAX_VEL), MAX_VEL);\
    }

    APPLY_BOUNDARY(x);
    APPLY_BOUNDARY(y);

    mmodel.setPosition(position);

    mshader.use();
    mshader.setTransform("view", viewMatrix);
    mshader.setVec3("viewPos", camPosition);

    mxInput = 0;
    myInput = 0;
}

ModelRenderObject& Spaceship::getRenderObject() {
    return mmodel;
}

void Spaceship::inputX(float x) {
    mxInput = x;
}

void Spaceship::inputY(float y) {
    myInput = y;
}
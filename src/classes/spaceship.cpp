#include "../headers/spaceship.hpp"


Spaceship::Spaceship(Crosshair & crosshair) : mshader("shaders/modelLoading.vs", "shaders/modelLoading.fs"), 
        mmodel((std::filesystem::absolute("models/spaceship/basic_spaceship.gltf")).generic_string(), &mshader),
        mcrosshair(crosshair) {
    glm::vec3 lightPos(1.0, 1.0, 0.0);
    mshader.use();
    mshader.setProjection(glm::perspective(glm::radians(crosshair.getCam().FovY),
     (float)crosshair.getScreenWidth() / (float)crosshair.getScreenHeight(), 0.1f, 100.0f));
    mshader.setVec3("light.position", lightPos);
    mshader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    mshader.setVec3("light.diffuse", glm::vec3(0.9f, 0.9f, 0.9f)); // darken diffuse light a bit
    mshader.setVec3("light.specular",glm::vec3(1.0f, 1.0f, 1.0f)); 
    mshader.setFloat("light.constant",  1.0f);
    mshader.setFloat("light.linear",    0.045f);
    mshader.setFloat("light.quadratic", 0.0032f);
}

void Spaceship::periodic(float deltaTimeSec) {
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

    auto aimPoint = (mcrosshair.getPlanarDirectionVector() * AIM_EFFECT_STRENGTH) - mmodel.getPosition();

    glm::quat quat = glm::lookAt(glm::vec3(0.0, 0.0, -1.0), aimPoint, glm::vec3(0.0, 1.0, 0.0));
    mmodel.setRotationRad(glm::vec3(-glm::pitch(quat), glm::yaw(quat) - glm::pi<float>(), glm::roll(quat)));
    mmodel.setPosition(position);

    mshader.use();
    mshader.setTransform("view", mcrosshair.getCam().getViewMatrix());
    mshader.setVec3("viewPos", mcrosshair.getCam().position());

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
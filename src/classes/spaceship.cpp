#include "../headers/spaceship.hpp"

Shader * shaders::modelLoadingShader = NULL;
Shader * shaders::safeGetModelLoadingShader() {
    if (modelLoadingShader == NULL) {
        modelLoadingShader = new Shader("shaders/modelLoading.vs", "shaders/modelLoading.fs");
    }
    return modelLoadingShader;
}
Spaceship::Spaceship(Crosshair & crosshair) : mshader(*shaders::safeGetModelLoadingShader()),
        mmodel((std::filesystem::absolute("models/spaceship/basic_spaceship.gltf")).generic_string(), shaders::safeGetModelLoadingShader()),
        mcrosshair(crosshair), mlaserShader("shaders/laserBeam.vs", "shaders/laserBeam.fs"), 
        mlaserModel((std::filesystem::absolute("models/laser/laser.gltf")).generic_string(), &mlaserShader) {
    glm::vec3 lightPos(1.0, 1.0, 0.0);
    auto perspective = glm::perspective(glm::radians(crosshair.getCam().FovY),
     (float)crosshair.getScreenWidth() / (float)crosshair.getScreenHeight(), 0.1f, 100.0f);
    mshader.use();
    mshader.setProjection(perspective);
    mshader.setVec3("DirectionalLightDirection", glm::vec3(1, -1, 0.5));
    mshader.setVec3("DirectionalLightDiffuse", glm::vec3(0.2, 0.2, 0.2));
    mshader.setVec3("DirectionalLightSpecular", glm::vec3(0.2, 0.2, 0.2));
    mlaserShader.use();
    mlaserShader.setProjection(perspective);
    mmodel.setPosition(glm::vec3(0.0, 0.0, 3.0));
    mmodel.setRotationRad(glm::vec3(glm::radians(0.0), 0.0f, 0.0f));
    mmodel.setScale(glm::vec3(1.0, 1.0, 1.0));
    mlaserModel.setScale(glm::vec3(2.0, 2.0, 4.0));
    rendering::renderer->addRenderObject(&mmodel);
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

    glm::quat quat = glm::lookAt(mmodel.getPosition(), 
        (mcrosshair.getPlanarDirectionVector() * AIM_EFFECT_STRENGTH),
        glm::vec3(0.0, 1.0, 0.0));
    glm::vec3 rotVector = glm::vec3(glm::pitch(quat), glm::yaw(quat) - glm::pi<float>(), glm::roll(quat));
    mmodel.setRotationRad(rotVector);
    mmodel.setPosition(position);

    mlastShotSec += deltaTimeSec;
    if (mlastShotSec >= SHOOT_COOLDOWN_SEC && misShooting) {
        mlastShotSec = fmod(mlastShotSec, SHOOT_COOLDOWN_SEC);
        mlaserModel.setRotationRad(rotVector);
        mlaserModel.setPosition(position);
    }

    mshader.use();
    mshader.setTransform("view", mcrosshair.getCam().getViewMatrix());
    mshader.setVec3("viewPos", mcrosshair.getCam().position());
    mcrosshair.getNormalizedDirection(); // delete this

    if (mlastShotSec <= SHOOT_EFFECT_TIME_SEC) {
        mshader.setVec3("light.position",
         (mlastShotSec * LASER_SPEED) * glm::normalize((DEFAULT_LIGHT_DISTANCE * mcrosshair.getPlanarDirectionVector()) - position));
        mshader.setVec3("light.ambient", glm::vec3(0.3f, 0.2f, 0.2f));
        mshader.setVec3("light.diffuse", glm::vec3(1.0f, 0.5f, 0.5f)); // darken diffuse light a bit
        mshader.setVec3("light.specular",glm::vec3(1.0f, 0.5f, 0.5f)); 
        mshader.setFloat("light.constant",  0.75f);
        mshader.setFloat("light.linear",    0.05f);
        mshader.setFloat("light.quadratic", 0.001f); 
        mlaserShader.use();
        mlaserShader.setTransform("view", mcrosshair.getCam().getViewMatrix());
        mlaserShader.setVec3("viewPos", mcrosshair.getCam().position());
        glEnable(GL_DEPTH_TEST);
        mlaserModel.setPosition(mlaserModel.getPosition() 
        + ( LASER_SPEED * mlastShotSec * glm::vec3((mlaserModel.getRotationTransform() * glm::vec4(0.0, 0.0, 0.1, 0.0)))));
        rendering::renderer->renderTarget(mlaserModel); // this makes sure we wont render the laser 
    } else {
        mshader.setVec3("light.position", DEFAULT_LIGHT_DISTANCE * mcrosshair.getPlanarDirectionVector());
        mshader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        mshader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
        mshader.setVec3("light.specular",glm::vec3(1.0f, 1.0f, 1.0f)); 
        mshader.setFloat("light.constant",  0.7f);
        mshader.setFloat("light.linear",    0.045f);
        mshader.setFloat("light.quadratic", 0.0032f);
    }

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

void Spaceship::shoot(bool isShooting) {
    misShooting = isShooting;
}
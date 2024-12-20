#include "../headers/spaceship.hpp"

Spaceship::Spaceship(Crosshair & crosshair) : mshader(*shaders::safeGetModelLoadingShader()),
        mmodel((std::filesystem::absolute("models/spaceship/basic_spaceship.gltf")).generic_string(), shaders::safeGetModelLoadingShader()),
        mcrosshair(crosshair), mlaserShader("shaders/laserBeam.vs", "shaders/laserBeam.fs"), 
        mlaserModel((std::filesystem::absolute("models/laser/laser.gltf")).generic_string(), &mlaserShader),
        maimLight(), mlaserLight() {
    glm::vec3 lightPos(1.0, 1.0, 0.0);
    auto perspective = glm::perspective(glm::radians(crosshair.getCam().FovY),
     (float)crosshair.getScreenWidth() / (float)crosshair.getScreenHeight(), 0.1f, 100.0f);
    rendering::projection = perspective;
    mshader.use();
    mshader.setProjection(perspective);
    mlaserShader.use();
    mlaserShader.setProjection(perspective);
    mmodel.setPosition(glm::vec3(0.0, 0.0, -3.0));
    mmodel.setRotationRad(glm::vec3(glm::radians(0.0), 0.0f, 0.0f));
    mmodel.setScale(glm::vec3(1.0, 1.0, 1.0));
    mlaserModel.setScale(glm::vec3(2.0, 2.0, 5.0));
    rendering::renderer->addRenderObject(&mmodel);
    rendering::renderer->addForwardRenderObject(&mlaserModel);
    maimLight.mlightColor = glm::vec3(1.0, 1.0, 0.9);
    maimLight.mlightLinearIntensity = 0.1;
    maimLight.mlightQuadraticIntensity = 0.01;
    mlaserLight.mlightColor = glm::vec3(10.0, 0.1, 0.1);
    mlaserLight.mlightPosition = mlaserModel.getPosition();
    mlaserLight.mlightLinearIntensity = 0.9;
    mlaserLight.mlightQuadraticIntensity = 0.001;

    rendering::renderer->addLightSource(&maimLight);
    rendering::renderer->addLightSource(&mlaserLight);

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
        -glm::vec3(0.0, 1.0, 0.0));
    glm::vec3 rotVector = glm::vec3(glm::pitch(quat), glm::yaw(quat), glm::roll(quat));
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
    rendering::view = mcrosshair.getCam().getViewMatrix();
    rendering::viewPos =  mcrosshair.getCam().position();

    if (mlastShotSec <= SHOOT_EFFECT_TIME_SEC) {
        mlaserShader.use();
        mlaserShader.setTransform("view", mcrosshair.getCam().getViewMatrix());
        mlaserShader.setVec3("viewPos", mcrosshair.getCam().position());
        glEnable(GL_DEPTH_TEST);
        // rendering::renderer->renderTarget(mlaserModel); 
    }
    maimLight.mlightPosition = DEFAULT_LIGHT_DISTANCE * mcrosshair.getPlanarDirectionVector();
    mlaserModel.setPosition(mlaserModel.getPosition() 
        + ( LASER_SPEED * mlastShotSec * glm::vec3((mlaserModel.getRotationTransform() * glm::vec4(0.0, 0.0, 0.1, 0.0)))));
    mlaserLight.mlightPosition = mlaserModel.getPosition();
    Asteroid::allPeriodic(mcrosshair, mmodel.getPosition(), misShooting);

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
#include "../headers/timed_effect.hpp"

using namespace timing;

std::vector<TimedEffect*> TimedEffect::effects = std::vector<TimedEffect*>();

TimedEffect::TimedEffect(Shader * shader, float duration, bool renderDeferred) {
    mrenderDeferred = renderDeferred;
    mShader = shader;
    mcurrentTime = 0;
    mduration = duration;
    mindex = effects.size();
    effects.push_back(this);
}

void TimedEffect::periodic(float deltaTime) {
    mcurrentTime += deltaTime;
    if (mcurrentTime > mduration) {
        delete this;
        return;
    }
    mShader->use();
    mShader->setFloat("time", mcurrentTime);
    mShader->setFloat("duration", mduration);
    mShader->setVec3("position", getPosition());
    renderToScreen();
}

void TimedEffect::renderToScreen() {
    if (mrenderDeferred) {
        rendering::renderer->renderTarget(*this);
    } else { // froward rendering in this case
         rendering::renderer->renderTargetForward(*this);
    }
}

TimedEffect::~TimedEffect() {
    for (int i = mindex + 1; i < effects.size(); i++){
        effects[i]->mindex -= 1;
    }
    effects.erase(effects.begin() + this->mindex);
}

void TimedEffect::allPeriodic() {
    glEnable(GL_DEPTH_TEST);
    for (auto effect : effects) {
        effect->periodic(deltaTime);
    }
}
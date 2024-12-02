#include "../headers/timed_effect.hpp"

using namespace timing;

std::vector<TimedEffect*> TimedEffect::effects = std::vector<TimedEffect*>();

TimedEffect::TimedEffect(Shader * shader, float duration) {
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
    rendering::renderer->renderTarget(*this);
}

TimedEffect::~TimedEffect() {
    for (int i = mindex + 1; i < effects.size(); i++){
        effects[i]->mindex -= 1;
    }
    effects.erase(effects.begin() + this->mindex);
}

void TimedEffect::allPeriodic() {
    for (auto effect : effects) {
        effect->periodic(deltaTime);
    }
}
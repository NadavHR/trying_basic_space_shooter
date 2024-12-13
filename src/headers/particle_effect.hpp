#ifndef PARTICLE_EFFECT_HPP
#define PARTICLE_EFFECT_HPP
#include "timed_effect.hpp"
#include "model.hpp"
class ParticleEffect : public TimedEffect {
    private: 
        size_t mcount;
        Model &mmodel;

    protected:
        void draw() override;


    public: 
        ParticleEffect(Shader * shader, float duration, size_t count, Model &model, bool renderDeferred = true);
};

#endif /* PARTICLE_EFFECT_HPP */

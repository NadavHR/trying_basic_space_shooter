#ifndef TIMED_EFFECT_HPP
#define TIMED_EFFECT_HPP
#include "spatial_render_object.hpp"
#include <vector>
#include "static_utils.hpp"

class TimedEffect : public SpatialRenderObject {
    private: 
        static std::vector<TimedEffect*> effects;
        size_t mindex;
        float mduration;
        float mcurrentTime;

    protected:
        void periodic(float deltaTime);

    
    public:
     TimedEffect(Shader * shader, float duration);
     ~TimedEffect();
     static void allPeriodic();

};

#endif /* TIMED_EFFECT_HPP */

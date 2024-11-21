#include "../headers/target.hpp"

Target::Target() : mshader(*shaders::safeGetModelLoadingShader()),
  mmodel("models/asteroid/asteroid.gltf", &mshader) {
    

}
Target::~Target(){
}  

bool Target::isHit(const Crosshair& crosshair ) {

}

void Target::periodic() {

}
#include "../headers/asteroid.hpp"

ModelRenderObject* Asteroid::basicModel = NULL;
std::vector<Asteroid *> Asteroid::targets = std::vector<Asteroid *>(); 

using namespace timing;

Asteroid::Asteroid(const glm::vec3 spaceshipPosition) : mshader(*shaders::safeGetModelLoadingShader()), mhitRadius(DEFAULT_HIT_RADIUS) {
  static size_t totalAsteroidsCount = 0;
  if (basicModel == NULL) {
    srand(time(0));
    basicModel = new ModelRenderObject("models/asteroid/asteroid.gltf", &mshader);
    basicModel->setScale(glm::vec3(1, 1, 1));
  }
  mposition = glm::vec3(RAND_RANGE_F(-X_RANGE, X_RANGE), RAND_RANGE_F(-Y_RANGE, Y_RANGE), DEFAULT_DISTANCE);
  mvelocity = glm::normalize(spaceshipPosition - mposition) * INITIAL_SPEED * powf(SPEED_MULTIPLIER, totalAsteroidsCount);
  totalAsteroidsCount++;
  mindex = targets.size();
  targets.push_back(this);

}
Asteroid::~Asteroid(){
  for (int i = mindex + 1; i < targets.size(); i++){
      targets[i]->mindex -= 1;
  }
  targets.erase(targets.begin() + this->mindex);

  ParticleEffect * effect = new ParticleEffect(shaders::safeGetDebrisShader(), 
          1.0, effectOptions::PARTICLE_COUNT, *(basicModel->getModel()), false); // i know its not cleared, its fine
  glm::vec3 position = mposition;
  effect->setPosition(position);
}  

bool Asteroid::isHit(const Crosshair& crosshair ) {
  glm::vec3 laserPosition = crosshair.getPlanarDirectionVector() * glm::abs(mposition.z);
  glm::vec3 shotDistance = laserPosition - mposition;
  return glm::length(shotDistance) <= mhitRadius;
}

bool Asteroid::periodic(float deltaTimeSec) {
  mposition += (deltaTime * mvelocity);
  if (mposition.z >= MAX_Z) {
    delete this;
    return false;
  }
  basicModel->setPosition(mposition);
  rendering::renderer->renderTarget(*basicModel);
  return true;
}

void Asteroid::allPeriodic(const Crosshair& crosshair, const glm::vec3 spaceshipPosition, const bool shooting) {
  static float timer = SPAWN_TIME_SEC;
  static float spawnTime = SPAWN_TIME_SEC;
  float deltaTimeSec = deltaTime;
  timer += deltaTimeSec;
  if (timer >= spawnTime) {
    timer = fmod(timer, spawnTime);
    spawnTime *= SPAWN_TIME_MULTIPLIER;
    Asteroid * newAsteroid = new Asteroid(spaceshipPosition); // lol this is not freed here but dont worry its fine it gets freed 
  }

  glEnable(GL_DEPTH_TEST);
  for (auto asteroid : targets) {
    if (asteroid->periodic(deltaTimeSec) && shooting && asteroid->isHit(crosshair)) {
      delete asteroid;
    }
  }
}
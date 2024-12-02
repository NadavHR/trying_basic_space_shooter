#include "../headers/asteroid.hpp"
#define RAND_RANGE_F(low, high)  (low + (float)(rand()) / ( (float)(RAND_MAX/(high-(low)))))
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
}  

bool Asteroid::isHit(const Crosshair& crosshair ) {
  glm::vec3 laserPosition = crosshair.getPlanarDirectionVector() * mposition.z;
  glm::vec3 shotDistance = laserPosition - mposition;
  return glm::length(shotDistance) <= mhitRadius;
}

void Asteroid::periodic(float deltaTimeSec) {
  mposition += (deltaTime * mvelocity);
  if (mposition.z <= 0) {
    delete this;
    return;
  }
  basicModel->setPosition(mposition);
  rendering::renderer->renderTarget(*basicModel);

}

void Asteroid::allPeriodic(const Crosshair& crosshair, const glm::vec3 spaceshipPosition, const bool shooting) {
  static float timer = SPAWN_TIME_SEC;
  float deltaTimeSec = deltaTime;
  timer += deltaTimeSec;
  if (timer >= SPAWN_TIME_SEC) {
    timer = fmod(timer, SPAWN_TIME_SEC);
    Asteroid * newAsteroid = new Asteroid(spaceshipPosition); // lol this is not freed here but dont worry its fine it gets freed 
  }

  glEnable(GL_DEPTH_TEST);
  for (auto asteroid : targets) {
    asteroid->periodic(deltaTimeSec);
    if (shooting && asteroid->isHit(crosshair)) {
      delete asteroid;
    }
  }
}
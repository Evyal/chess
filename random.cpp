#include "random.h"
#include "constants.h"
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <cmath>
#include <random>

std::default_random_engine rng(static_cast<size_t>(
    std::chrono::steady_clock::now().time_since_epoch().count()));

//////////////////////////////////////////////////////////////////////////////////////////
// Generate a RANDOM INTEGER (SIZE_T) in the RANGE [min, max], inclusive

size_t randomInt(size_t min, size_t max) {

  std::uniform_int_distribution dist(min, max);
  return dist(rng);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Generate a random FLOAT in the range (min, max]

float randomFloat(float min, float max) {
  std::uniform_real_distribution dist(min, max);
  return dist(rng);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Generates a random position for a boid, all the boids spawn around the same
// center point

sf::Vector2f randomBoidPosition(sf::Vector2f center) {
  std::uniform_real_distribution angleGen(0.f, 2 * M_PIf);
  std::uniform_real_distribution moduleGen(0.f, constants::randomPositionRange);

  // Generate randomly until the boid spawns inside the margins
  bool b{true};
  sf::Vector2f position{};
  while (b) {
    position = center;

    float angle{angleGen(rng)};
    float step{moduleGen(rng)};

    position += {step * cosf(angle), step * sinf(angle)};
    if (position.x > constants::randomMinimumX &&
        position.x < constants::randomMaximumX &&
        position.y > constants::randomMinimumY &&
        position.y < constants::randomMaximumY) {
      b = false;
    }
  }

  return position;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Boids of the same flock are generated with their direction varying just a
// little between each other

sf::Vector2f randomBoidSpeed(float angleCenter) {
  std::uniform_real_distribution angle(
      angleCenter - constants::randomAngleRange,
      angleCenter + constants::randomAngleRange);
  std::uniform_real_distribution norm(constants::randomMinimumSpeed,
                                      constants::randomMaximumSpeed);
  float v{norm(rng)};
  float a{angle(rng)};
  return {cosf(a) * v, sinf(a) * v};
}
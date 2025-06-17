#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>
#include <random>

//////////////////////////////////////////////////////////////////////////////////////////
// Global random engine using inline variables
inline std::random_device eng{};
inline std::mt19937 engine{eng()};

// Optional reseeding function
inline void seedEngine(unsigned int seed) { engine.seed(seed); }

//////////////////////////////////////////////////////////////////////////////////////////
// Function to generate size_t integral types.
inline int randomInt(int min, int max) {
  assert(min <= max && "For randomInt, min must be less than or equal to max.");
  std::uniform_int_distribution<int> dist(min, max);
  return dist(eng);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function to generate size_t integral types.
inline std::size_t randomSizeType(std::size_t min, std::size_t max) {
  assert(min <= max && "For randomInt, min must be less than or equal to max.");
  std::uniform_int_distribution<std::size_t> dist(min, max);
  return dist(eng);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function to generate a random real numbers in the range [a,b).
inline float randomFloat(float min, float max) {
  assert(min < max && "For randomFloat, min must be less than max.");
  std::uniform_real_distribution<float> dist(min, max);
  return dist(eng);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function to generate a random boolean value.
inline bool randomBool() {
  std::bernoulli_distribution dist(0.5);
  return dist(eng);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Generates a random angle in range [0,2Pi)

inline float randomAngle() { return randomFloat(0.f, 2.f * M_PIf); }

#endif
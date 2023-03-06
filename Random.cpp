#include "Random.h"

std::default_random_engine Random::generator;
std::uniform_int_distribution<uint32_t> Random::distribution(0, 255);
std::chrono::high_resolution_clock::duration Random::seed = std::chrono::high_resolution_clock::now().time_since_epoch();

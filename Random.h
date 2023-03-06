#pragma once

#include <random>
#include <chrono>

class Random
{
	static std::default_random_engine generator;
	static std::uniform_int_distribution<uint32_t> distribution;
	static std::chrono::high_resolution_clock::duration seed;

public:

	static void Init()
	{
		generator.seed((unsigned)seed.count());
	}

	static uint32_t UInt()
	{
		return distribution(generator);
	}
};

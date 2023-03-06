#include "RandomPainter.h"
#include "Random.h"
#include <thread>

RandomPainter::RandomPainter()
{
	Random::Init();
}

void RandomPainter::paint(imageBuffer imb)
{
	// TODO increase speed with OMP

	for (int i = 0; i < imb.size; ++i) {
		imb.buffer[i] = Random::UInt();
	}
}

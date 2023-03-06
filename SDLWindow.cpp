#include "SDLWindow.h"

#include <iostream>
#include <cassert>
#include <charconv>
#include <algorithm>

SDLWindow::SDLWindow(int w, int h, std::string name) : imgW(w), imgH(h), windowName(name), fpsSurface(nullptr), fpsTexture(nullptr), fpsPtr(0)
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "WINDOW SYSTEM: SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		assert(0);
	}

	if (TTF_Init()) {
		std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
		assert(0);
	}

	// Create window
	window = SDL_CreateWindow(name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

	if (!window) {
		std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
		assert(0);
	}

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		assert(0);
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

	// Create texture that gets rendered
	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, imgW, imgH);
	if (!texture)
	{
		std::cerr << "Texture could not be created! SDL Error: " << SDL_GetError() << std::endl;
		assert(0);
	}
	
	font = TTF_OpenFont("Roboto-Bold.ttf", 24);
	if (!font)
	{
		std::cerr << "Error reading font file: " << TTF_GetError() << std::endl;
		assert(0);
	}

	fpsPosition = { w - 100, 0, 100, 100 };
	tp = std::chrono::high_resolution_clock::now();
	memset(fpsArr, 0, sizeOfFpsArr);

	// Create imgBuff
	buffSize = w * h * 3; // BGR buff
	imageBuff = new uint8_t[buffSize];
}

SDLWindow::~SDLWindow()
{
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		std::cout << "WINDOW: destroy the texture used in window called " << windowName << std::endl;
	}

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	std::cout << "WINDOW: destroy the renderer of the window called " << windowName << std::endl;

	SDL_DestroyWindow(window);
	window = nullptr;
	std::cout << "WINDOW: destroy the window called " << windowName << std::endl;

	if (imageBuff != nullptr) {
		delete[] imageBuff;
		imageBuff = nullptr;
	}

	SDL_Quit();
}

void SDLWindow::renderFrame()
{
	SDL_UpdateTexture(texture, NULL, imageBuff, imgW * 3);
	
 	auto now = std::chrono::high_resolution_clock::now();
	auto tpassed = 1'000'000 / std::chrono::duration_cast<std::chrono::microseconds>(now - tp).count();
	fpsArr[fpsPtr % sizeOfFpsArr] = tpassed;
	fpsPtr++;
	tp = now;

	// median of fpsArr
	uint64_t cpy[sizeOfFpsArr];
	for (int i = 0; i < 5; ++i)
		cpy[i] = fpsArr[i];
	std::nth_element(cpy, cpy + (sizeOfFpsArr>>1), cpy + sizeOfFpsArr);
	
	fpsSurface = TTF_RenderText_Solid(font, std::to_string(cpy[sizeOfFpsArr >> 1]).c_str(), fontColor);
	fpsTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderCopy(renderer, fpsTexture, NULL, &fpsPosition);
	SDL_RenderPresent(renderer);

	SDL_FreeSurface(fpsSurface);
	SDL_DestroyTexture(fpsTexture);
}

imageBuffer SDLWindow::getImageBuffer()
{
	return { imageBuff, buffSize };
}

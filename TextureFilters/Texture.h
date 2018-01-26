#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <string>
#include <iostream>

class Texture
{
public:
	Texture();
	~Texture();

	SDL_Texture* getTexture();
	SDL_Rect getClipRect();
	void* getPixels();
	bool loadFromFile(std::string fileName, SDL_Renderer* renderer);
	int getHeight();
	int getWidth();
	int getPitch();

private:
	//The actual hardware texture
	SDL_Texture* m_texture;
	SDL_Rect m_clipRect;
	Uint32* originalPixels;
	void* m_pixels;
	int m_pitch;

	//Image dimensions
	int m_width;
	int m_height;
};

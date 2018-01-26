#pragma once
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"
#include "TextureEffect.h"
class FilterManager
{
public:

	enum EdgeType
	{
		EdgeEnhance,
		EdgeDetect,
		Emboss
	};

	static FilterManager * Instance();
	bool FilterManager::createJtexture(std::string fileName, int id, SDL_Renderer* renderer);
	SDL_Texture* getTexture(int id);
	SDL_Rect getTextureBounds(int id);

	// TEXTURE FILTERS
	//////////////////////////////////
	// Texture ID
	void grayscaleFilter(int id);
	// Texture ID, Threshold 0 - 255, Blur Radius, Blend Amount 0-1.f
	void bloomFilter(int id, int threshold, int radius, float blendAmount);
	// Texture ID, Type 0-EdgeEnhance, 1-EdgeDetect, 2-Emboss
	void edgeDectection(int id, int type);
	// Texture ID, Pixel Size
	void pixelateFilter(int id, int pixelSize);
	// Texture ID, Blur Radius
	void gaussianBlur1D(int id, int radius);
	// Texture ID
	void resetPixels(int id);

private:
	const float MAX_BLEND_AMOUNT = 1.0f;
	const int MAX_THRESHOLD = 255;
	const int MAX_BLUR_RADIUS = 100;
	const int MAX_PIXEL_SIZE = 100;
	static FilterManager *m_inst;
	std::map<int, Texture*> m_textureMap;
	FilterManager();
	~FilterManager();

};

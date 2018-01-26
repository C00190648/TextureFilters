
#include "FilterManager.h"

FilterManager * FilterManager::m_inst = nullptr;
FilterManager::FilterManager()
{
}
FilterManager::~FilterManager()
{
}
FilterManager * FilterManager::Instance()
{
	if (!m_inst)
	{
		m_inst = new FilterManager();
	}
	return m_inst;
}
// Create JTexture
///////////////////////////////////////////////////////////////
bool FilterManager::createJtexture(std::string fileName, int id, SDL_Renderer * renderer)
{
	JTexture* temp = new JTexture();
	if (temp->loadFromFile(fileName, renderer))
	{
		m_textureMap[id] = temp;
		return true;
	}
	delete temp;
	temp = nullptr;
	return false;
}
// Get Texture from the map of JTextures
///////////////////////////////////////////////////////////////
SDL_Texture * FilterManager::getTexture(int id)
{
	return m_textureMap[id]->getTexture();
}
// Get Texture Rectangle Bounds
///////////////////////////////////////////////////////////////
SDL_Rect FilterManager::getTextureBounds(int id)
{
	return m_textureMap[id]->getClipRect();
}
// Grayscale function
///////////////////////////////////////////////////////////////
void FilterManager::grayscaleFilter(int id)
{
	Uint32* tempPixels = TextureEffect::grayScale(m_textureMap[id]);
	SDL_UpdateTexture(m_textureMap[id]->getTexture(), &m_textureMap[id]->getClipRect(), tempPixels, m_textureMap[id]->getPitch());

	// Clean up Pixels
	delete[] tempPixels;
	tempPixels = nullptr;
}
// Bloom function
// Brightpass -> Blur -> Bloom
///////////////////////////////////////////////////////////////
void FilterManager::bloomFilter(int id, int threshold, int radius, float blendAmount)
{
	// Limit the values
	if (threshold > MAX_THRESHOLD)
		threshold = MAX_THRESHOLD;
	if (radius > MAX_BLUR_RADIUS)
		radius = MAX_BLUR_RADIUS;
	if (blendAmount > MAX_BLEND_AMOUNT)
		blendAmount = MAX_BLEND_AMOUNT;
	Uint32* tempPixels = TextureEffect::brightPass(threshold, m_textureMap[id]);
	tempPixels = TextureEffect::gaussianBlur(radius, m_textureMap[id], tempPixels, true);
	tempPixels = TextureEffect::bloom(blendAmount, tempPixels, m_textureMap[id]);
	SDL_UpdateTexture(m_textureMap[id]->getTexture(), &m_textureMap[id]->getClipRect(), tempPixels, m_textureMap[id]->getPitch());

	// Clean up Pixels
	delete[] tempPixels;
	tempPixels = nullptr;
}
// EdgeDectection function
// 0 - EdgeEnhance, 1 - EdgeDetect, 2 - Emboss
///////////////////////////////////////////////////////////////
void FilterManager::edgeDectection(int id, int type)
{
	Uint32* tempPixels = TextureEffect::edgeDectection(type, m_textureMap[id]);
	SDL_UpdateTexture(m_textureMap[id]->getTexture(), &m_textureMap[id]->getClipRect(), tempPixels, m_textureMap[id]->getPitch());
	// Clean up Pixels
	delete[] tempPixels;
	tempPixels = nullptr;
}
// Pixelate function
///////////////////////////////////////////////////////////////
void FilterManager::pixelateFilter(int id, int pixelSize)
{
	if (pixelSize > MAX_PIXEL_SIZE)
		pixelSize = MAX_PIXEL_SIZE;

	// CHECKING PIXEL SIZE ISN'T BIGGER THAN WIDTH OR HEIGHT OF IMAGE
	int textureWidth = m_textureMap[id]->getWidth();
	int textureHeight = m_textureMap[id]->getHeight();

	if ((pixelSize > textureWidth || pixelSize > textureHeight) && textureWidth < textureHeight)
		pixelSize = textureWidth;
	else if ((pixelSize > textureWidth || pixelSize > textureHeight) && textureWidth > textureHeight)
		pixelSize = textureHeight;

	Uint32* tempPixels = TextureEffect::pixelate(pixelSize, m_textureMap[id]);
	SDL_UpdateTexture(m_textureMap[id]->getTexture(), &m_textureMap[id]->getClipRect(), tempPixels, m_textureMap[id]->getPitch());
	// Clean up Pixels
	delete[] tempPixels;
	tempPixels = nullptr;
}
// Gaussian Blur function - OPTIMIZED
// Blur is applied twice once horizontally and once vertically
///////////////////////////////////////////////////////////////
void FilterManager::gaussianBlur1D(int id, int radius)
{
	if (radius > MAX_BLUR_RADIUS)
		radius = MAX_BLUR_RADIUS;

	Uint32* tempPixels = TextureEffect::gaussianBlur(radius, m_textureMap[id], NULL, false);
	SDL_UpdateTexture(m_textureMap[id]->getTexture(), &m_textureMap[id]->getClipRect(), tempPixels, m_textureMap[id]->getPitch());
	// Clean up Pixels
	delete[] tempPixels;
	tempPixels = nullptr;
}
// Reset Texture pixels with the original pixels from the image
///////////////////////////////////////////////////////////////
void FilterManager::resetPixels(int id)
{
	Uint32* pixels = (Uint32*)m_textureMap[id]->getPixels();	// Get the Pixels from JTexture
	SDL_UpdateTexture(m_textureMap[id]->getTexture(), &m_textureMap[id]->getClipRect(), pixels, m_textureMap[id]->getPitch());
}
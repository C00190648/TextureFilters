
#include "JTexture.h"


JTexture::JTexture()
	: m_texture(NULL)
	, m_pixels(NULL)
	, m_width(0)
	, m_height(0)
	, m_pitch(0)
	, m_clipRect(SDL_Rect{0,0,0,0})
{
}
JTexture::~JTexture()
{

}
// Return Texture
///////////////////////////////////////////////////////////////
SDL_Texture * JTexture::getTexture()
{
	return m_texture;
}
// Return Texture Rect
///////////////////////////////////////////////////////////////
SDL_Rect JTexture::getClipRect()
{
	return m_clipRect;
}
// Return Pixels from Texture
///////////////////////////////////////////////////////////////
void * JTexture::getPixels()
{
	return m_pixels;
}
// Load Texture from File
///////////////////////////////////////////////////////////////
bool JTexture::loadFromFile(std::string fileName, SDL_Renderer* renderer)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(fileName.c_str());

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);
	//Lock texture for manipulation
	SDL_LockTexture(texture, NULL, &m_pixels, &m_pitch);
	//Copy loaded/formatted surface pixels
	memcpy(m_pixels, surface->pixels, surface->pitch * surface->h);
	//Unlock texture to update
	SDL_UnlockTexture(texture);

	m_clipRect = surface->clip_rect;
	m_height = surface->h;
	m_width = surface->w;

	// Enables Transparency 
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	// Clean Up Surface
	SDL_FreeSurface(surface);

	//Return success
	m_texture = texture;
	return true;
}
// Return Texture Height
///////////////////////////////////////////////////////////////
int JTexture::getHeight()
{
	return m_height;
}
// Return Texture Width
///////////////////////////////////////////////////////////////
int JTexture::getWidth()
{
	return m_width;
}
// Return Texture Pitch
///////////////////////////////////////////////////////////////
int JTexture::getPitch()
{
	return m_pitch;
}

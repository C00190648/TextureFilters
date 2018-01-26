#include "Texture.h"



Texture::Texture()
	: m_texture(NULL)
	, m_pixels(NULL)
	, m_width(0)
	, m_height(0)
	, m_pitch(0)
	, m_clipRect(SDL_Rect{ 0,0,0,0 })
{
}
Texture::~Texture()
{

}
// Return Texture
///////////////////////////////////////////////////////////////
SDL_Texture * Texture::getTexture()
{
	return m_texture;
}
// Return Texture Rect
///////////////////////////////////////////////////////////////
SDL_Rect Texture::getClipRect()
{
	return m_clipRect;
}
// Return Pixels from Texture
///////////////////////////////////////////////////////////////
void * Texture::getPixels()
{
	return m_pixels;
}
// Load Texture from File
///////////////////////////////////////////////////////////////
bool Texture::loadFromFile(std::string fileName, SDL_Renderer* renderer)
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
int Texture::getHeight()
{
	return m_height;
}
// Return Texture Width
///////////////////////////////////////////////////////////////
int Texture::getWidth()
{
	return m_width;
}
// Return Texture Pitch
///////////////////////////////////////////////////////////////
int Texture::getPitch()
{
	return m_pitch;
}

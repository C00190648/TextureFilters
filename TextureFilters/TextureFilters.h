#pragma once
#include "Texture.h"
class TextureEffect
{
public:
	static Uint32 * edgeDectection(int type, Texture * m_jTexture);
	static Uint32 * grayScale(Texture * m_jTexture);
	static Uint32 * bloom(float blendAmount, Uint32 * tempPixels, Texture * jTexture);
	static Uint32 * gaussianBlur(int radius, Texture * m_jTexture, Uint32 * tempPixels, bool bloom);
	static Uint32 * pixelate(int pixelSize, Texture * m_jTexture);
private:
	TextureEffect() {};
	static float * calc1DGaussianKernel(int radius, int kernelSize, float * kernelData);
};

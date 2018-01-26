#pragma once
#include "JTexture.h"
class TextureEffect
{
public:
	static Uint32 * edgeDectection(int type, JTexture * m_jTexture);
	static Uint32 * grayScale(JTexture * m_jTexture);
	static Uint32 * brightPass(int threshold, JTexture * jTexture);
	static Uint32 * bloom(float blendAmount, Uint32 * tempPixels, JTexture * jTexture);
	static Uint32 * gaussianBlur(int radius, JTexture * m_jTexture, Uint32 * tempPixels, bool bloom);
	static Uint32 * pixelate(int pixelSize, JTexture * m_jTexture);
private:
	TextureEffect() {};
	static float * calc1DGaussianKernel(int radius, int kernelSize, float * kernelData);
};


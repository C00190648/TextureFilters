
#include "TextureEffect.h"


int edge_enhance[3][3] = { { 0,0,0 },{ -1,1,0 },{ 0,0,0 } };
int edge_detect[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };
int emboss[3][3] = { { -2,-1,0 },{ -1,1,1 },{ 0,1,2 } };

Uint32 * TextureEffect::edgeDectection(int type, Texture * Texture)
{
	int pixelCount = static_cast<int>(Texture->getPitch() * 0.25f) *  Texture->getHeight();
	Uint32* tempPixels = new Uint32[pixelCount];
	Uint32* pixels = (Uint32*)Texture->getPixels();

	int imageWidth = Texture->getWidth();
	int imageHeight = Texture->getHeight();

	int* filter_type = NULL;
	if (type == 0)
		filter_type = (int*)edge_enhance;
	else if (type == 1)
		filter_type = (int*)edge_detect;
	else if (type == 2)
		filter_type = (int*)emboss;

	for (int x = 1; x < imageWidth - 1; x++)
	{
		for (int y = 1; y < imageHeight - 1; y++)
		{
			float totR = 0;
			float totG = 0;
			float totB = 0;

			for (int k = -1; k < 2; k++)
			{
				for (int l = -1; l < 2; l++)
				{
					int pos = ((y + l) *imageWidth) + (x + k);
					Uint8 r = pixels[pos] >> 16 & 0xFF;
					Uint8 g = pixels[pos] >> 8 & 0xFF;
					Uint8 b = pixels[pos] & 0xFF;

					// Gets the index of the 2d Array "3 = size of array"
					int arrayIndex = *(filter_type + (k + 1) * 3 + (l + 1));
					totR += r * arrayIndex;
					totG += g * arrayIndex;
					totB += b * arrayIndex;
				}
			}

			if (totR > 255) totR = 255;
			if (totG > 255) totG = 255;
			if (totB > 255) totB = 255;

			if (totR < 0) totR = 0;
			if (totG < 0) totG = 0;
			if (totB < 0) totB = 0;

			tempPixels[y * imageWidth + x] = 0xFF << 24 | Uint8(totR) << 16 | Uint8(totG) << 8 | Uint8(totB);
		}
	}
	return tempPixels;
}

Uint32 * TextureEffect::grayScale(Texture * Texture)
{
	int pixelCount = static_cast<int>(Texture->getPitch() * 0.25f) *  Texture->getHeight();
	Uint32* tempPixels = new Uint32[pixelCount];
	Uint32* pixels = (Uint32*)Texture->getPixels();

	for (int i = 0; i < pixelCount; ++i)
	{
		Uint8 a = pixels[i] >> 24 & 0xFF;
		Uint8 r = pixels[i] >> 16 & 0xFF;
		Uint8 g = pixels[i] >> 8 & 0xFF;
		Uint8 b = pixels[i] & 0xFF;
		Uint8 v = static_cast<Uint8>((0.212671f * r) + (0.715160f * g) + (0.072169f * b));
		tempPixels[i] = (a << 24) | (v << 16) | (v << 8) | v;
	}
	return tempPixels;
}

Uint32 * TextureEffect::brightPass(int threshold, Texture * Texture)
{
	int pixelCount = static_cast<int>(Texture->getPitch() * 0.25f) *  Texture->getHeight();
	Uint32* tempPixels = new Uint32[pixelCount];
	Uint32* pixels = (Uint32*)Texture->getPixels();

	for (int i = 0; i < pixelCount; ++i)
	{
		Uint8 a = pixels[i] >> 24 & 0xFF;
		Uint8 r = pixels[i] >> 16 & 0xFF;
		Uint8 g = pixels[i] >> 8 & 0xFF;
		Uint8 b = pixels[i] & 0xFF;
		Uint8 v = static_cast<Uint8>((0.212671f * r) + (0.715160f * g) + (0.072169f * b));
		if (v < threshold)
			tempPixels[i] = (a << 24) | (0 << 16) | (0 << 8) | 0;
		else
			tempPixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
	}
	return tempPixels;
}

Uint32 * TextureEffect::bloom(float blendAmount, Uint32 * tempPixels, Texture * Texture)
{
	int pixelCount = static_cast<int>(Texture->getPitch() * 0.25f) *  Texture->getHeight();
	Uint32* pixels = (Uint32*)Texture->getPixels();

	float totA = 0;
	float totR = 0;
	float totG = 0;
	float totB = 0;

	for (int i = 0; i < pixelCount; ++i)
	{
		Uint8 a1 = tempPixels[i] >> 24 & 0xFF;
		Uint8 r1 = tempPixels[i] >> 16 & 0xFF;
		Uint8 g1 = tempPixels[i] >> 8 & 0xFF;
		Uint8 b1 = tempPixels[i] & 0xFF;

		Uint8 a = pixels[i] >> 24 & 0xFF;
		Uint8 r = pixels[i] >> 16 & 0xFF;
		Uint8 g = pixels[i] >> 8 & 0xFF;
		Uint8 b = pixels[i] & 0xFF;

		totA = a + ((a1 * blendAmount));
		totR = r + ((r1 * blendAmount));
		totG = g + ((g1 * blendAmount));
		totB = b + ((b1 * blendAmount));
		if (totA > 255) totA = 255;
		if (totR > 255) totR = 255;
		if (totG > 255) totG = 255;
		if (totB > 255) totB = 255;
		tempPixels[i] = Uint8(totA) << 24 | Uint8(totR) << 16 | Uint8(totG) << 8 | Uint8(totB);
	}
	return tempPixels;
}

Uint32 * TextureEffect::gaussianBlur(int radius, Texture * Texture, Uint32 * editedPixels, bool bloom)
{
	Uint32* pixels = NULL;
	float * kernelData = NULL;
	int kernelSize = radius * 2 + 1;
	kernelData = calc1DGaussianKernel(radius, kernelSize, kernelData);

	int pixelCount = static_cast<int>(Texture->getPitch() * 0.25f) *  Texture->getHeight();
	Uint32* tempPixels = new Uint32[pixelCount];

	if (!bloom)
		pixels = (Uint32*)Texture->getPixels();
	else
		pixels = editedPixels;

	int imageWidth = Texture->getWidth();
	int imageHeight = Texture->getHeight();
	int offset = static_cast<int>(kernelSize * 0.5f);

	for (int x = 0; x < imageWidth; x++)
	{
		for (int y = 0; y < imageHeight; y++)
		{
			double totA = 0;
			double totR = 0;
			double totB = 0;
			double totG = 0;

			for (int i = -offset; i <= offset; i++)
			{
				int pos = 0;
				if (y + i < 0)
					pos = (imageHeight + i) *imageWidth + x;
				else if (y + i >= imageHeight)
					pos = ((y + i - imageHeight) *imageWidth) + x;
				else
					pos = ((y + i) *imageWidth) + x;

				Uint8 a = pixels[pos] >> 24 & 0xFF;
				Uint8 r = pixels[pos] >> 16 & 0xFF;
				Uint8 g = pixels[pos] >> 8 & 0xFF;
				Uint8 b = pixels[pos] & 0xFF;
				totA += a * kernelData[i + offset];
				totR += r * kernelData[i + offset];
				totG += g * kernelData[i + offset];
				totB += b * kernelData[i + offset];
			}
			tempPixels[y * imageWidth + x] = (Uint8(totA) << 24) | Uint8(totR) << 16 | Uint8(totG) << 8 | Uint8(totB);
		}
	}
	for (int x = 0; x < imageWidth; x++)
	{
		for (int y = 0; y < imageHeight; y++)
		{
			double totA = 0;
			double totR = 0;
			double totB = 0;
			double totG = 0;

			for (int i = -offset; i <= offset; i++)
			{
				int pos = 0;
				if (x + i < 0)
					pos = imageWidth + i;
				else if (x + i >= imageWidth)
					pos = (y *imageWidth) + x + i - imageWidth;
				else
					pos = (y *imageWidth) + x + i;
				Uint8 a = tempPixels[pos] >> 24 & 0xFF;
				Uint8 r = tempPixels[pos] >> 16 & 0xFF;
				Uint8 g = tempPixels[pos] >> 8 & 0xFF;
				Uint8 b = tempPixels[pos] & 0xFF;
				totA += a * kernelData[i + offset];
				totR += r * kernelData[i + offset];
				totG += g * kernelData[i + offset];
				totB += b * kernelData[i + offset];
			}
			tempPixels[y * imageWidth + x] = (Uint8(totA) << 24) | (Uint8(totR) << 16) | (Uint8(totG) << 8) | Uint8(totB);
		}
	}
	// Delete Array of Kernel Data as its no longer needed
	delete[] kernelData;
	kernelData = nullptr;

	if (bloom)
	{
		delete[] pixels;
		pixels = nullptr;
	}
	return tempPixels;
}

Uint32 * TextureEffect::pixelate(int pixelSize, Texture * Texture)
{
	int pixelSizeX = pixelSize;
	int pixelSizeY = pixelSize;
	int pixelCount = static_cast<int>(Texture->getPitch() * 0.25f) *  Texture->getHeight();
	Uint32* tempPixels = new Uint32[pixelCount];
	Uint32* pixels = (Uint32*)Texture->getPixels();

	int imageWidth = Texture->getWidth();
	int imageHeight = Texture->getHeight();

	for (int x = 0; x < imageWidth; x+= pixelSizeX)
	{
		for (int y = 0; y < imageHeight; y+= pixelSizeY)
		{
			float totA = 0, totR = 0, totG = 0, totB = 0, sumNum = 0;
			for (int xx = 0; xx < pixelSizeX && xx + x < imageWidth; xx++)
			{
				for (int yy = 0; yy < pixelSizeY && yy + y < imageHeight; yy++)
				{
					int pos = ((y + yy) *imageWidth) + x + xx;
					Uint8 a = pixels[pos] >> 24 & 0xFF;
					Uint8 r = pixels[pos] >> 16 & 0xFF;
					Uint8 g = pixels[pos] >> 8 & 0xFF;
					Uint8 b = pixels[pos] & 0xFF;
					totR += r;
					totG += g;
					totB += b;
					totA += a;
					sumNum++;
				}
			}
			totR /= sumNum;
			totG /= sumNum;
			totB /= sumNum;
			totA /= sumNum;

			for (int j = x; j < x + pixelSizeX && j < imageWidth; j++)
			{
				for (int k = y; k < y + pixelSizeY && k < imageHeight; k++)
				{	
					tempPixels[(k * imageWidth) + j] = Uint8(totA) << 24 | Uint8(totR) << 16 | Uint8(totG) << 8 | Uint8(totB);
				}
			}
		}	
	}
	return tempPixels;
}

float * TextureEffect::calc1DGaussianKernel(int radius, int kernelSize, float * kernelData)
{
	float * data = new float[kernelSize];
	float total = 0.0f;
	float sigma = radius / 3.0f;
	float twoSigmaSquare = 2.0f * sigma * sigma;
	float sigmaRoot = (float)sqrt(twoSigmaSquare * M_PI);

	for (int i = -radius; i <= radius; i++)
	{
		int distance = i * i;
		int index = i + radius;
		data[index] = (float)exp(-distance / twoSigmaSquare) / sigmaRoot;
		total += data[index];
	}
	for (int i = 0; i < kernelSize; i++)
	{
		data[i] = data[i] / total;
	}
	return data;
}


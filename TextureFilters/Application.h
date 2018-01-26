#pragma once
#include <iostream>
#include "SDL.h"
#include "LTimer.h"
#include "FilterManager.h"
#include <vector>

class Application
{
public:
	enum Vars
	{
		Background,
		Cursor,
		Image,
		Pixel,
		Blur,
		Threshold,
		Blend
	};

	Application();
	~Application();
	bool Initialize(const char*, int, int, int, int, int);
	void CleanUp();
	void Loop();


private:
	const int CURSOR_OFFESET = 17;
	const int MAX_BLEND_AMOUNT = 10;
	const int MAX_THRESHOLD = 255;
	const int MAX_BLUR_RADIUS = 50;
	const int MAX_PIXEL_SIZE = 100;
	const int NUM_OF_VARS = 7;
	const int NUM_OF_IMAGES = 2;
	const int FONT_SIZE = 19;
	const SDL_Color color = { 255, 255, 255, 255 };

	std::vector<int> m_values;
	std::vector<SDL_Point> m_position;
	std::vector<SDL_Texture*> m_texture;
	std::vector<SDL_Rect> m_textureRect;

	bool m_hide;
	bool m_quit;
	int m_currentImage;
	unsigned int m_lastTime;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Event m_event;

	void LoadContent();
	void UnloadContent();
	void Render();
	void Update();
	void HandleEvents();
	SDL_Texture* LoadTexture(const std::string fileName);
};

#pragma once
#include "FilterManager.h"

class App
{
public:
	App();
	~App();
	bool initialize(const char*, int, int, int, int, int);
	void cleanUp();
	void loop();

private:
	bool m_quit;
	float m_lastTime;

	SDL_Event m_event;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	void loadContent();
	void unloadContent();
	void render();
	void update();
	void handleEvents();
};
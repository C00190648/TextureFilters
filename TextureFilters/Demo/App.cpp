#include "App.h"

#include "stdafx.h"



App::App()
	: m_quit(false)
{
}


App::~App()
{
}

bool App::initialize(const char * title, int xPos, int yPos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	}
	loadContent();
	return true;
}

void App::cleanUp()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
}

void App::loop()
{
	while (!m_quit)
	{
		handleEvents();
		update();
		render();

	}
}

void App::loadContent()
{
	FilterManager::Instance()->createJtexture("../assets/image2.png", 0, m_renderer);
}

void App::unloadContent()
{
}

void App::render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	SDL_RenderCopy(m_renderer, FilterManager::Instance()->getTexture(0), NULL, &FilterManager::Instance()->getTextureBounds(0));

	SDL_RenderPresent(m_renderer);
}

void App::update()
{
}

void App::handleEvents()
{
	while (SDL_PollEvent(&m_event) != 0)
	{
		if (m_event.type == SDL_QUIT)
		{
			m_quit = true;
		}
		else
		{
			switch (m_event.type)
			{
			case SDL_KEYDOWN:
				switch (m_event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					m_quit = true;
					break;
				case SDLK_1:
					FilterManager::Instance()->resetPixels(0);
					break;
				case SDLK_2:
					FilterManager::Instance()->grayscaleFilter(0);
					break;
				case SDLK_3:
					FilterManager::Instance()->pixelateFilter(0, 10);
					break;
				case SDLK_4:
					FilterManager::Instance()->gaussianBlur1D(0, 15);
					break;
				case SDLK_5:
					FilterManager::Instance()->bloomFilter(0, 150, 20, 0.7f);
					break;
				}
			}
		}
	}
}


#include "Application.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


Application::Application()
    : m_hide(false)
	, m_quit(false)
	, m_lastTime(LTimer::gameTime())
	, m_currentImage(0)
	, m_values(NUM_OF_VARS)
	, m_texture(NUM_OF_VARS)
	, m_textureRect(NUM_OF_VARS)
	, m_position(NUM_OF_VARS)
{
	m_textureRect[Background] = SDL_Rect{ 0,0,241,414 };
	m_textureRect[Cursor] = SDL_Rect{ 5,305,30,19 };
	m_position[Image] = SDL_Point{ 218, 302 };
	m_position[Pixel] = SDL_Point{ 168, 319 };
	m_position[Blur] = SDL_Point{ 190, 336 };
	m_position[Threshold] = SDL_Point{ 179, 353 };
	m_position[Blend] = SDL_Point{ 124, 370 };

	for (int i = 0; i < NUM_OF_VARS; i++)
	{
		if (i == Image)
		{
			m_values[i] = 0;
		}
		else if(i == Cursor || i == Pixel)
		{
			m_values[i] = 2;
		}
		else
		{
			m_values[i] = 1;
		}
	}
}


Application::~Application()
{
}

bool Application::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL init success" << std::endl;
		m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	}
	LoadContent();
	return true;
}

void Application::CleanUp()
{
	std::cout << "Cleaning Up" << std::endl;
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
}

void Application::Loop()
{
	LTimer capTimer;
	int frameNum = 0;

	while (!m_quit)
	{
		capTimer.start();

		HandleEvents();
		Update();
		Render();

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}

void Application::LoadContent()
{
	//SetText(m_values[Image], Image);
	//SetText(m_values[Pixel], Pixel);
	//SetText(m_values[Blur], Blur);
	//SetText(m_values[Threshold], Threshold);
	//SetText(static_cast<float>(m_values[Blend]) / 10.f, Blend);

	FilterManager::Instance()->createJtexture("assets/image0.png", 0, m_renderer);
	FilterManager::Instance()->createJtexture("assets/image1.png", 1, m_renderer);
	FilterManager::Instance()->createJtexture("assets/image2.png", 2, m_renderer);
	m_texture[Background] = LoadTexture("assets/bg.png");
	m_texture[Cursor] = LoadTexture("assets/icon.png");
}

void Application::UnloadContent()
{
}

void Application::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	SDL_RenderClear(m_renderer);

	SDL_RenderCopy(m_renderer, FilterManager::Instance()->getTexture(m_values[Image]), NULL, &FilterManager::Instance()->getTextureBounds(m_values[Image]));
	if (!m_hide)
	{
		SDL_RenderCopy(m_renderer, m_texture[Background], NULL, &m_textureRect[Background]);
		for (int i = Cursor; i < NUM_OF_VARS; i++)
		{
			SDL_RenderCopy(m_renderer, m_texture[i], NULL, &m_textureRect[i]);
		}
	}
	SDL_RenderPresent(m_renderer);
}

void Application::Update()
{
	unsigned int currentTime = LTimer::gameTime();
	float deltaTime = (currentTime - m_lastTime) / 1000.f;
	// Update in here
	m_lastTime = currentTime;
}

void Application::HandleEvents()
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
					case SDLK_h:
						if (m_hide == false)
							m_hide = true;
						else
							m_hide = false;
						break;
					case SDLK_1:
						FilterManager::Instance()->resetPixels(m_values[Image]);
						break;
					case SDLK_2:
						FilterManager::Instance()->grayscaleFilter(m_values[Image]);
						break;
					case SDLK_3:
						FilterManager::Instance()->pixelateFilter(m_values[Image], m_values[Pixel]);
						break;
					case SDLK_4:
						FilterManager::Instance()->gaussianBlur1D(m_values[Image], m_values[Blur]);
						break;
					case SDLK_5:
						FilterManager::Instance()->bloomFilter(m_values[Image], m_values[Threshold],m_values[Blur], m_values[Blend] / 10.0f);
						break;
					case SDLK_6:
						FilterManager::Instance()->edgeDectection(m_values[Image],FilterManager::EdgeDetect);
						break;
					case SDLK_7:
						FilterManager::Instance()->edgeDectection(m_values[Image], FilterManager::EdgeEnhance);
						break;
					case SDLK_8:
						FilterManager::Instance()->edgeDectection(m_values[Image], FilterManager::Emboss);
						break;
					/*case SDLK_LEFT:
						if (m_values[Cursor] == Image && m_values[Image] > 0)
						{
							m_values[Image]--;
							SetText(m_values[Image], Image);
						}
						if (m_values[Cursor] == Pixel && m_values[Pixel] > 2)
						{
							m_values[Pixel]--;
							SetText(m_values[Pixel], Pixel);
						}
						if (m_values[Cursor] == Blend && m_values[Blend] > 1)
						{
							m_values[Blend]--;
							SetText(static_cast<float>(m_values[Blend]) / 10.f, Blend);
						}
						if (m_values[Cursor] == Threshold && m_values[Threshold] > 1)
						{
							m_values[Threshold]--;
							SetText(m_values[Threshold], Threshold);
						}
						if (m_values[Cursor] == Blur && m_values[Blur] > 1)
						{
							m_values[Blur]--;
							SetText(m_values[Blur], Blur);
						}
						break;
					case SDLK_RIGHT:

						if (m_values[Cursor] == Image && m_values[Image] < NUM_OF_IMAGES)
						{
							m_values[Image]++;
							SetText(m_values[Image], Image);
						}
						if (m_values[Cursor] == Pixel && m_values[Pixel] < MAX_PIXEL_SIZE)
						{
							m_values[Pixel]++;
							SetText(m_values[Pixel], Pixel);
						}
						if (m_values[Cursor] == Blend && m_values[Blend] < MAX_BLEND_AMOUNT)
						{
							m_values[Blend]++;
							SetText(static_cast<float>(m_values[Blend]) / 10.f, Blend);
						}
						if (m_values[Cursor] == Threshold && m_values[Threshold] < MAX_THRESHOLD)
						{
							m_values[Threshold]++;
							SetText(m_values[Threshold], Threshold);
						}
						if (m_values[Cursor] == Blur && m_values[Blur] < MAX_BLUR_RADIUS)
						{
							m_values[Blur]++;
							SetText(m_values[Blur], Blur);
						}
						break;*/
					case SDLK_DOWN:
						if (m_values[Cursor] < Blend)
						{
							m_values[Cursor]++;
							m_textureRect[Cursor].y += CURSOR_OFFESET;
						}
						break;
					case SDLK_UP:
						if (m_values[Cursor] > Image)
						{
							m_values[Cursor]--;
							m_textureRect[Cursor].y -= CURSOR_OFFESET;
						}
						break;
				}
			}
		}
	}
}


SDL_Texture * Application::LoadTexture(const std::string fileName)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
	if (pTempSurface == 0)
	{
		return NULL;
	}

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_renderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);

	if (pTexture != 0)
	{
		return pTexture;
	}
	// reaching here means something went wrong
	return NULL;
}

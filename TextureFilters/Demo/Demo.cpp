// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "App.h"

int main()
{
	App* app = new App();

	app->initialize("Implementation Texture Filter", 500, 200, 800, 600, SDL_WINDOW_INPUT_FOCUS);
	app->loop();
	app->cleanUp();
	system("Pause");
	return 0;
}


#pragma once

#include <SDL2/SDL.h>


#define APP_RELATIVE_MOUSE_MODE 1 << 0

struct Application
{
	//SDL_Window * window;
	uint32_t flags;
	uint32_t flags_update;
};


void app_update (struct Application * app)
{
	if (app->flags != app->flags_update)
	{
		app->flags_update = app->flags;
		SDL_SetRelativeMouseMode (app->flags & APP_RELATIVE_MOUSE_MODE);
	}
}



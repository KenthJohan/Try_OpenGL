#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "debug.h"
#include "v.h"

void test ()
{
	float a [8] = 
	{
		100.0f, 100.0f,
		200.0f, 200.0f,
		300.0f, 300.0f,
		400.0f, 400.0f
	};
	
	float b [2] = {1.0f, 1.0f};
	float r [8];
	

	vf32_sub2 (r, a, b, 4, 2);
	
	vf32_print  (stdout, b, 2, "%f ");
	fprintf (stdout, "\n");
	vf32_print2 (stdout, a, 4, 2, "%f ");
	fprintf (stdout, "\n");
	vf32_print2 (stdout, r, 4, 2, "%f ");
	fprintf (stdout, "\n");	
}



void show (SDL_Renderer * renderer, float x [])
{
	SDL_Rect r;
	r.x = (int) x [0];
	r.y = (int) x [1];
	r.w = (int) x [2] - r.x;
	r.h = (int) x [3] - r.y;
	SDL_RenderDrawRect (renderer, &r);
}



#define APP_QUIT 1 << 0

int main (int argc, char *argv[])
{
	
	float rects [8] = 
	{
		0.0f, 0.0f,
		100.0f, 100.0f,
		200.0f, 200.0f,
		300.0f, 300.0f,
	};
	float diff [8];


	SDL_Init (SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow
	(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);
	ASSERT (window != NULL);
	SDL_Renderer * renderer = SDL_CreateRenderer (window, -1, 0);
	ASSERT (renderer != NULL);
	SDL_SetRenderDrawColor (renderer, 0, 0, 50, 255);
	SDL_RenderClear(renderer);
	
	uint32_t flags = 0;
	float m [2];
	
	while (1)
	{
		if (flags & APP_QUIT) {break;}
		SDL_Event e;
		while (SDL_PollEvent (&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
				flags |= APP_QUIT;
				break;

				case SDL_MOUSEMOTION:
				m [0] = e.motion.x;
				m [1] = e.motion.y;
				vf32_print (stdout, m, 2, "%f ");
				vf32_sub2 (diff, rects, m, 4, 2);
				vf32_print2 (stdout, diff, 2, 2, "%f ");
				break;
			}
		}
		SDL_SetRenderDrawColor (renderer, 0, 255, 0, 255);
		show (renderer, rects);
		SDL_RenderPresent (renderer);
		SDL_Delay (100);
	}


	SDL_DestroyWindow (window);
	SDL_Quit ();
	
	return 0;
}











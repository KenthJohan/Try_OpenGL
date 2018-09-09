#pragma once

#include <SDL2/SDL.h>
#include "debug.h"
#include "debug_gl.h"
#include "ui.h"


//Soft application exit
#define APP_QUIT 1 << 0

//When assertion is disabled for OpenGL error checking
//this should be used for soft application exit.
#define APP_OPENGL_ERROR 1 << 1



#define APP_WINDOW_WIDTH 1024
#define APP_WINDOW_HEIGHT 768
#define APP_TITLE "My OpenGL test window"



struct Application
{
	//Can be used for checking if app should exit, etc.
	uint32_t flags;
	
	//User input.
	const Uint8 * keyboard;
	SDL_Event event;
	
	SDL_Window * window;
	SDL_GLContext context;
	GLuint program;
	
	float mouse_pos [2];
};












void app_init (struct Application * app, int argc, char * argv [])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	app->flags = 0;
	{
		int r = SDL_Init (SDL_INIT_VIDEO);
		ASSERT_F (r == 0, "SDL_Init %i\n", SDL_GetError ());
	}
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 5);
	
	app->keyboard = SDL_GetKeyboardState (NULL);
	
	app->window = SDL_CreateWindow 
	(
		APP_TITLE, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		APP_WINDOW_WIDTH, 
		APP_WINDOW_WIDTH, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ASSERT_F (app->window != NULL, "SDL_CreateWindow: %s", SDL_GetError());
	
	app->context = SDL_GL_CreateContext (app->window);
	ASSERT_F (app->context != NULL, "SDL_GL_CreateContext: %s", SDL_GetError());
	
	{
		glewExperimental = GL_TRUE;
		GLenum err = glewInit ();
		ASSERT (err == GLEW_OK);
	}
	
	//glEnable (GL_DEPTH_TEST);
	glEnable (GL_DEBUG_OUTPUT);
	glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback (MessageCallback, 0);
	
	glDebugMessageCallback (glDebugOutput, 0);
	glDebugMessageControl (GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	
	printf ("Using opengl version %s....\n", glGetString (GL_VERSION));
	printf ("Using glsl version %s.....\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	
	
	struct Shader shaders [2] = 
	{
		{0, GL_VERTEX_SHADER, "src/test_gui/shader.glvs"},
		{0, GL_FRAGMENT_SHADER, "src/test_gui/shader.glfs"}
	};
	app->program = program_create ("SimpleShaderProgram1", shaders, COUNTOF (shaders));
	glUseProgram (app->program);
	
	glClearColor (0.0f, 0.0f, 0.1f, 0.0f);
	glPointSize (4.0f);
}


void app_destroy (struct Application * app)
{
	SDL_GL_DeleteContext (app->context);
	SDL_DestroyWindow (app->window);
}











void app_frame_begin (struct Application * app)
{
	glClear (GL_COLOR_BUFFER_BIT);
}


void app_frame_end (struct Application * app)
{
	SDL_GL_SwapWindow (app->window);	
	SDL_Delay (1);
}






void app_update_window_size (struct Application * app)
{
	int w;
	int h;
	SDL_GetWindowSize (app->window, &w, &h);
	glViewport (0, 0, w, h);
}









void app_handle_keydown (struct Application * app)
{
	SDL_Event e = app->event;
	ASSERT (e.type == SDL_KEYDOWN);
	switch (e.key.keysym.sym)
	{
		case SDLK_ESCAPE:
		e.type = SDL_QUIT;
		SDL_PushEvent (&e);
		break;
		
		case SDLK_c:
		glUniform4f (glGetUniformLocation (app->program, "objectColor"), 1.0, 1.0, 1.0, 1.0);
		break;
	}
}


void app_handle_keyup (struct Application * app)
{
	SDL_Event e = app->event;
	ASSERT (e.type == SDL_KEYUP);
	switch (e.key.keysym.sym)
	{
		case SDLK_ESCAPE:
		e.type = SDL_QUIT;
		SDL_PushEvent (&e);
		break;
		
		case SDLK_c:
		glUniform4f (glGetUniformLocation (app->program, "objectColor"), 0.0, 0.0, 0.0, 0.0);
		break;
	}
}


void app_handle_input (struct Application * app)
{
	SDL_Event e = app->event;
	switch (e.type)
	{
		case SDL_KEYDOWN:
		app_handle_keydown (app);
		break;
		
		case SDL_KEYUP:
		app_handle_keyup (app);
		break;
		
		case SDL_QUIT:
		app->flags |= APP_QUIT;
		break;

		case SDL_MOUSEMOTION:
		break;
		
		case SDL_WINDOWEVENT:
		switch (app->event.window.event)
		{
			case SDL_WINDOWEVENT_CLOSE:
			printf ("SDL_WINDOWEVENT_CLOSE");
			break;
		
			case SDL_WINDOWEVENT_RESIZED:
			app_update_window_size (app);
			break;
		}
		break;
		
		case SDL_MOUSEBUTTONDOWN:
		printf ("Mouse button down\n");
		break;
	}
}











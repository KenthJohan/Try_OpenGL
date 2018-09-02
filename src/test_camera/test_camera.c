#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "debug.h"
#include "debug_gl.h"
#include "misc.h"
#include "camera.h"
#include "gmesh.h"
#include "gbuf.h"
#include "gen.h"
#include "shader.h"
#include "vertex.h"
#include "app.h"
#include "q.h"
 
#define FMT_INT TCOL (TCOL_NORMAL, TCOL_YELLOW, TCOL_DEFAULT) "%02i " TCOL_RESET
#define FMT_FLT TCOL (TCOL_NORMAL, TCOL_CYAN, TCOL_DEFAULT) "%04.1f " TCOL_RESET
#define APP_WINDOW_WIDTH 1024
#define APP_WINDOW_HEIGHT 768
#define APP_TITLE "My OpenGL test window"




void sdl_get_mouse (SDL_Window * window)
{
	int w;
	int h;
	SDL_GetWindowSize (window, &w, &h);
	int x;
	int y;
	SDL_GetMouseState (&x, &y);
	//printf ("(%f %f)\n", (float)x/(float)w - 0.5f, (float)y/(float)h - 0.5f);
}


void sdl_get_rotation_vector (float a [4], uint8_t const * keyboard)
{
	a [0] = (keyboard [SDL_SCANCODE_DOWN] - keyboard [SDL_SCANCODE_UP]);
	a [1] = (keyboard [SDL_SCANCODE_RIGHT] - keyboard [SDL_SCANCODE_LEFT]);
	a [2] = (keyboard [SDL_SCANCODE_E] - keyboard [SDL_SCANCODE_Q]);
	a [3] = 0.01f;
}


void sdl_get_translation_vector (float t [4], uint8_t const * keyboard)
{
	t [0] = (keyboard [SDL_SCANCODE_A] - keyboard [SDL_SCANCODE_D]);
	t [1] = (keyboard [SDL_SCANCODE_LCTRL] - keyboard [SDL_SCANCODE_SPACE]);
	t [2] = (keyboard [SDL_SCANCODE_W] - keyboard [SDL_SCANCODE_S]);
	t [3] = 0.01f;
}


void sdl_get_mouse2 (SDL_Event * e, float a [4])
{
	a [0] = e->motion.yrel;
	a [1] = e->motion.xrel;
	a [2] = 0;
	a [3] = 0.01f;
	printf ("(%f %f)\n", (float)e->motion.xrel, (float)e->motion.yrel);
	fflush (stdout);
}




void app_create_mesh (struct GMesh * mesh, struct Cam * cam)
{
	{
		struct GMesh * m = gbuf_add (mesh);
		ASSERT (m != NULL);
		struct Vertex * grid = gbuf_init (NULL, 20 * 20 * 2, sizeof (struct Vertex), GBUF_MALLOC);
		float cc [4] = {0.4, 0.2, 0.2, 1.0};
		gen1_grid (grid, 0.0f, -10.0f, 10.0f, -10.0f, 10.0f, 0.1f, cc);
		m->flags = GMESH_DRAW;
		m->mode = GL_LINES;
		m->data = grid;
		m->cam = cam;
		m4f32_identity (m->mm);
	}
	
	{
		struct GMesh * m = gbuf_add (mesh);
		ASSERT (m != NULL);
		struct Vertex * grid = gbuf_init (NULL, 20 * 20 * 2, sizeof (struct Vertex), GBUF_MALLOC);
		float cc [4] = {0.2, 0.4, 0.2, 1.0};
		gen1_grid (grid, 0.0f, -10.0f, 10.0f, -10.0f, 10.0f, 0.1f, cc);
		m->flags = GMESH_DRAW;
		m->mode = GL_LINES;
		m->data = grid;
		m->cam = cam;
		m4f32_identity (m->mm);
		float q [4] = {0.0f, 1.0f, 0.0f, M_PI * 2.0f * 0.1f};
		qf32_axis_angle (q, q);
		qf32_normalize (q, q);
		qf32_m4 (q, m->mm, M_COLMAJ);
	}
}














int main (int argc, char *argv[])
{
	struct Application app;
	app.flags = 0;
	
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	
	{
		int r = SDL_Init (SDL_INIT_VIDEO);
		ASSERT_F (r == 0, "SDL_Init %i\n", SDL_GetError ());
	}

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 5);

	//App variables:
	SDL_Window * window;
	SDL_GLContext context;
	GLuint program;
	GLuint uniform_mvp;
	struct Cam cam;
	
	window = SDL_CreateWindow 
	(
		APP_TITLE, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		APP_WINDOW_WIDTH, 
		APP_WINDOW_WIDTH, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ASSERT_F (window != NULL, "SDL_CreateWindow: %s", SDL_GetError());
	
	context = SDL_GL_CreateContext (window);
	ASSERT_F (context != NULL, "SDL_GL_CreateContext: %s", SDL_GetError());

	glewExperimental = GL_TRUE;
	glewInit ();

	printf ("Using opengl version %s....\n", glGetString (GL_VERSION));
	printf ("Using glsl version %s.....\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	//During init, enable debug output
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_DEBUG_OUTPUT);
	glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback (MessageCallback, 0);
	glDebugMessageCallback (glDebugOutput, 0);
	glDebugMessageControl (GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	
	struct Shader shaders [2] = 
	{
		{0, GL_VERTEX_SHADER, "shader.glvs"},
		{0, GL_FRAGMENT_SHADER, "shader.glfs"}
	};
	program = program_create (shaders, COUNTOF (shaders));
	uniform_mvp = glGetUniformLocation (program, "mvp");
	ASSERT_F (uniform_mvp >= 0, "glGetUniformLocation no uniform found.");


	struct GMesh * mesh = gbuf_init (NULL, 3, sizeof (struct GMesh), GBUF_MALLOC);
	app_create_mesh (mesh, &cam);
	gmesh_init (mesh, program);
	
	const Uint8 * keyboard = SDL_GetKeyboardState (NULL);



	cam_init (&cam, window);
	//M4_PRINT (cam.mp, FMT_FLT);
	glClearColor (0.0f, 0.0f, 0.1f, 0.0f);

	printf ("main loop:\n");
	while (1)
	{
		
		sdl_get_rotation_vector (cam.q1, keyboard);
		sdl_get_translation_vector (cam.p1, keyboard);
		
		
		SDL_Event event;
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				goto main_quit;
				
				case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
					printf ("SDL_WINDOWEVENT_CLOSE");
					break;
				
					case SDL_WINDOWEVENT_RESIZED:
					//printf ("Window %d resized to %dx%d\n",event.window.windowID, event.window.data1, event.window.data2);
					gl_update_projection (window, cam.P);
					break;
				}
				break;
				
				case SDL_MOUSEMOTION:
				//sdl_get_mouse (window);
				if (app.flags & APP_RELATIVE_MOUSE_MODE)
				{
					sdl_get_mouse2 (&event, cam.q1);
				}
				break;


				case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					//Maybe SDL_PushEvent should be used for error handling?
					event.type = SDL_QUIT;
					SDL_PushEvent (&event);
					break;
					
					case SDLK_r:
					app.flags ^= APP_RELATIVE_MOUSE_MODE;
					break;

					case SDLK_SPACE:
					case SDLK_LCTRL:
					case SDLK_LEFT:
					case SDLK_RIGHT:
					case SDLK_UP:
					case SDLK_DOWN:
					//M4_PRINT (cam.mvp, FMT_FLT);
					//printf ("\n");
					break;
				}
				
				app_update (&app);
				break;
			}
		}
		glUseProgram (program);

		cam_update (&cam, keyboard);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gmesh_draw (mesh, uniform_mvp);
		SDL_Delay (1);
		SDL_GL_SwapWindow (window);
		GL_CHECK_ERROR;
	}
	
main_quit:
	
	SDL_GL_DeleteContext (context);
	SDL_DestroyWindow (window);

	return 0;
}

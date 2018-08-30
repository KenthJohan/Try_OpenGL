#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "debug.h"
#include "debug_gl.h"
#include "mat.h"
#include "misc.h"
#include "camera.h"
//#include "mesh.h"
#include "gmesh.h"
#include "gbuf.h"
#include "gen.h"
#include "shader.h"
#include "vertex.h"
#include "graphics.h"
#include "q.h"
 
#define FMT_INT TCOL (TCOL_NORMAL, TCOL_YELLOW, TCOL_DEFAULT) "%02i " TCOL_RESET
#define FMT_FLT TCOL (TCOL_NORMAL, TCOL_CYAN, TCOL_DEFAULT) "%04.1f " TCOL_RESET
#define APP_WINDOW_WIDTH 1024
#define APP_WINDOW_HEIGHT 768
#define APP_TITLE "My OpenGL test window"










void app_create_mesh (struct GMesh * mesh, struct Camera * cam)
{
	{
		struct GMesh * m = gbuf_add (mesh);
		ASSERT (m != NULL);
		struct Vertex * grid = gbuf_init (NULL, 20 * 20 * 2, sizeof (struct Vertex), GBUF_MALLOC);
		gen1_grid (grid, -1.0f, -10.0f, 10.0f, -10.0f, 10.0f, 0.1f);
		m->flags = GMESH_DRAW;
		m->mode = GL_LINES;
		m->data = grid;
		m->cam = cam;
		M4_IDENTITY (m->mm);
		//m->mm [M4_V0 + 0] =  4.0f;
		//m->mm [M4_V1 + 1] =  4.0f;
	}
}














int main (int argc, char *argv[])
{
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
	struct Camera cam;
	
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



	camera_init (&cam, window);
	//M4_PRINT (cam.mp, FMT_FLT);
	glClearColor (0.0f, 0.0f, 0.1f, 0.0f);

	printf ("main loop:\n");
	while (1)
	{
		
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
					gl_update_projection (window, cam.m_p);
					break;
				}
				break;
				
				case SDL_MOUSEMOTION:
				sdl_get_mouse (window);
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
					//CLR_V (4*4, mvp);
					case SDLK_a:
					case SDLK_d:
					case SDLK_w:
					case SDLK_s:

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
				break;
			}
		}
		glUseProgram (program);
		camera_update (&cam, keyboard);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gmesh_draw (mesh, uniform_mvp);
		SDL_Delay (10);
		SDL_GL_SwapWindow (window);
		GL_CHECK_ERROR;
	}
	
main_quit:
	
	SDL_GL_DeleteContext (context);
	SDL_DestroyWindow (window);

	return 0;
}

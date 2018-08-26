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
 
#define FMT_INT TCOL (TCOL_NORMAL, TCOL_YELLOW, TCOL_DEFAULT) "%02i " TCOL_RESET
#define FMT_FLT TCOL (TCOL_NORMAL, TCOL_CYAN, TCOL_DEFAULT) "%04.1f " TCOL_RESET
#define APP_WINDOW_WIDTH 1024
#define APP_WINDOW_HEIGHT 768
#define APP_TITLE "My OpenGL test window"





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



	struct Vertex * v0 = gbuf_init (6, sizeof (struct Vertex), GBUF_MALLOC, NULL);
	struct Vertex * v;
	v = gbuf_add (v0);
	V4_SET (v->pos, -0.5f, -0.5f, -1.0f, 1.0f);
	V4_SET (v->col, 1.0f, 0.0f, 0.0f, 1.0f);
	v = gbuf_add (v0);
	V4_SET (v->pos,  0.5f, -0.5f, -1.0f, 1.0f);
	V4_SET (v->col, 0.0f, 1.0f, 0.0f, 1.0f);
	v = gbuf_add (v0);
	V4_SET (v->pos,  0.5f,  0.5f, -1.0f, 1.0f);
	V4_SET (v->col, 0.0f, 0.0f, 1.0f, 1.0f);
	v = gbuf_add (v0);
	V4_SET (v->pos,  0.5f,  0.5f, -1.0f, 1.0f);
	V4_SET (v->col, 0.0f, 0.0f, 1.0f, 1.0f);
	v = gbuf_add (v0);
	V4_SET (v->pos, -0.5f,  0.5f, -1.0f, 1.0f);
	V4_SET (v->col, 0.0f, 1.0f, 0.0f, 1.0f);
	v = gbuf_add (v0);
	V4_SET (v->pos, -0.5f, -0.5f, -1.0f, 1.0f);
	V4_SET (v->col, 1.0f, 0.0f, 0.0f, 1.0f);

	struct Vertex * v1 = gbuf_init (20 * 20 * 2, sizeof (struct Vertex), GBUF_MALLOC, NULL);
	gen1_grid (v1, -10.0f, 10.0f, -10.0f, 10.0f, 0.1f);
	
	
	struct GMesh * m0 = gbuf_init (2, sizeof (struct GMesh), GBUF_MALLOC, NULL);
	struct GMesh * m;
	m = gbuf_add (m0);
	ASSERT (m != NULL);
	m->flags = GMESH_DRAW;
	m->mode = GL_TRIANGLES;
	m->program = program;
	m->vao = vertex_get_vao (program);
	m->draw_offset = 0;
	m->draw_count = gbuf (v0)->n;
	m->data = v0;
	m->vbo_flags = GL_DYNAMIC_STORAGE_BIT;
	
	m = gbuf_add (m0);
	ASSERT (m != NULL);
	m->flags = GMESH_DRAW;
	m->mode = GL_LINES;
	m->program = program;
	m->vao = vertex_get_vao (program);
	m->draw_offset = 0;
	m->draw_count = gbuf (v1)->n;
	m->data = v1;
	m->vbo_flags = GL_DYNAMIC_STORAGE_BIT;

	
	
	gmesh_init (m0);
	
	
	
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
					gl_update_projection (window, cam.mp);
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
		glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, cam.mvp);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		gmesh_draw (m0);
		
		SDL_Delay (10);
		SDL_GL_SwapWindow (window);
		GL_CHECK_ERROR;
	}
	
main_quit:
	
	SDL_GL_DeleteContext (context);
	SDL_DestroyWindow (window);

	return 0;
}

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
#include "mesh.h"
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

	
	struct Vertex tri_data [3] = 
	{
		[0].pos = { -0.5f, -0.5f, 0.0f, 1.0f}, [0].col = { 1.0f, 0.0f, 0.0f, 1.0f },
		[1].pos = {  0.5f, -0.5f, 0.0f, 1.0f}, [1].col = { 0.0f, 1.0f, 0.0f, 1.0f },
		[2].pos = {  0.0f,  0.5f, 0.0f, 1.0f}, [2].col = { 0.0f, 0.0f, 1.0f, 1.0f }
	};

	struct Vertex sqr_data [6] = 
	{
		[0].pos = { -0.5f, -0.5f, -1.0f, 1.0f }, [0].col = { 1.0f, 0.0f, 0.0f, 1.0f },
		[1].pos = {  0.5f, -0.5f, -1.0f, 1.0f }, [1].col = { 0.0f, 1.0f, 0.0f, 1.0f },
		[2].pos = {  0.5f,  0.5f, -1.0f, 1.0f }, [2].col = { 0.0f, 0.0f, 1.0f, 1.0f },
		[3].pos = {  0.5f,  0.5f, -1.0f, 1.0f }, [3].col = { 0.0f, 0.0f, 1.0f, 1.0f },
		[4].pos = { -0.5f,  0.5f, -1.0f, 1.0f }, [4].col = { 0.0f, 1.0f, 0.0f, 1.0f },
		[5].pos = { -0.5f, -0.5f, -1.0f, 1.0f }, [5].col = { 1.0f, 0.0f, 0.0f, 1.0f }
	};

	
	struct GR_Object obj [10];
	
	obj [0].flags = GR_ALLOCATE | GR_UPDATE_ONCE | GR_DRAW;
	obj [0].mode = GL_LINE_LOOP;
	obj [0].program = program;
	obj [0].vao = vertex_get_vao (program);
	obj [0].count = 100;
	obj [0].size8 = obj [0].count * sizeof (struct Vertex);
	obj [0].offset = 0;
	obj [0].offset8 = 0;
	obj [0].data = NULL;
	obj [0].vbo_flags = GL_DYNAMIC_STORAGE_BIT;
	
	obj [1].flags = GR_ALLOCATE | GR_UPDATE_ONCE;
	obj [1].mode = GL_TRIANGLE_FAN;
	obj [1].program = program;
	obj [1].vao = vertex_get_vao (program);
	obj [1].count = 100;
	obj [1].size8 = obj [1].count * sizeof (struct Vertex);
	obj [1].offset = 0;
	obj [1].offset8 = 0;
	obj [1].data = NULL;
	obj [1].vbo_flags = GL_DYNAMIC_STORAGE_BIT;
	
	obj [2].flags = GR_ALLOCATE | GR_UPDATE_ONCE | GR_DRAW;
	obj [2].mode = GL_LINES;
	obj [2].program = program;
	obj [2].vao = vertex_get_vao (program);
	obj [2].count = 100;
	obj [2].size8 = obj [2].count * sizeof (struct Vertex);
	obj [2].offset = 0;
	obj [2].offset8 = 0;
	obj [2].data = NULL;
	obj [2].vbo_flags = GL_DYNAMIC_STORAGE_BIT;
	
	obj [3].flags = GR_DRAW;
	obj [3].mode = GL_TRIANGLES;
	obj [3].program = program;
	obj [3].vao = vertex_get_vao (program);
	obj [3].count = COUNTOF (tri_data);
	obj [3].size8 = obj [3].count * sizeof (struct Vertex);
	obj [3].offset = 0;
	obj [3].offset8 = 0;
	obj [3].data = tri_data;
	obj [3].vbo_flags = GL_DYNAMIC_STORAGE_BIT;
	
	obj [4].flags = GR_DRAW;
	obj [4].mode = GL_TRIANGLES;
	obj [4].program = program;
	obj [4].vao = vertex_get_vao (program);
	obj [4].count = COUNTOF (sqr_data);
	obj [4].size8 = obj [4].count * sizeof (struct Vertex);
	obj [4].offset = 0;
	obj [4].offset8 = 0;
	obj [4].data = sqr_data;
	obj [4].vbo_flags = GL_DYNAMIC_STORAGE_BIT;
	
	
	gr_init (obj, 5);
	
	gen_circle (obj [0].data, obj [0].count, 2.0f, 1.0f, 1.0f, -2.0f);
	gen_circle1 (obj [1].data, obj [1].count, 2.0f, 4.0f, 1.0f, -2.0f);
	gen_grid (obj [2].data, obj [2].count, -10.0f, 10.0f, -10.0f, 10.0f, 1.0f);
	
	gr_update (obj, 5);
	
	
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
		
		camera_update (&cam, keyboard);
		
		
		glUseProgram (program);
		glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, cam.mvp);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL_CHECK_ERROR;
		
		gr_draw (obj, 5);
		
		//printf ("glGetError %i\n", glGetError ());
		fflush (stdout);
		SDL_Delay (10);
		SDL_GL_SwapWindow (window);
	}
	
main_quit:
	
	SDL_GL_DeleteContext (context);
	SDL_DestroyWindow (window);

	return 0;
}

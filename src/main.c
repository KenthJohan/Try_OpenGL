#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "debug.h"
#include "mat.h"
#include "misc.h"
 
#define FMT_INT ANSIC (ANSIC_NORMAL, ANSIC_YELLOW, ANSIC_DEFAULT) "%02i " ANSIC_RESET
#define FMT_FLT ANSIC (ANSIC_NORMAL, ANSIC_CYAN, ANSIC_DEFAULT) "%04.1f " ANSIC_RESET
#define APP_WINDOW_WIDTH 1024
#define APP_WINDOW_HEIGHT 768
#define APP_TITLE "My OpenGL test window"

void GLAPIENTRY MessageCallback
(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam)
{
	fprintf (stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message);
}


struct Camera
{
	float mvp [4*4];
	float mt [4*4];
	float mp [4*4];
	float mrx [4*4];
	float mry [4*4];
	float ax;
	float ay;
	float az;
};





int main (int argc, char *argv[])
{
	fprintf (stderr, "main\n");
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
	//glEnable (GL_DEBUG_OUTPUT);
	//glDebugMessageCallback (MessageCallback, 0);
	
	struct app_shader shaders [2] = 
	{
		{0, GL_VERTEX_SHADER, "shader.glvs"},
		{0, GL_FRAGMENT_SHADER, "shader.glfs"}
	};
	program = app_create_program (shaders, COUNTE (shaders));
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
	
	#define GRID_COUNT 84
	struct Vertex grid_data [GRID_COUNT] = {0};
	gen_grid (grid_data, GRID_COUNT, -10.0f, 10.0f, -10.0f, 10.0f, 1.0f);
	
	struct Mesh triangle;
	struct Mesh square;
	struct Mesh grid;
	
	triangle.mode = GL_TRIANGLES;
	triangle.vert_count = 3;
	triangle.vert_data = tri_data;
	triangle.vao = gpu_load_verts (program, triangle.vert_data, triangle.vert_count);
	
	square.mode = GL_TRIANGLES;
	square.vert_count = 6;
	square.vert_data = sqr_data;
	square.vao = gpu_load_verts (program, square.vert_data, square.vert_count);
	
	grid.mode = GL_LINES;
	grid.vert_count = GRID_COUNT;
	grid.vert_data = grid_data;
	grid.vao = gpu_load_verts (program, grid.vert_data, GRID_COUNT);
	
	GLfloat scr_col[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	const Uint8 * keyboard = SDL_GetKeyboardState (NULL);



	
	IDENTITY_M (4, 4, cam.mvp);
	IDENTITY_M (4, 4, cam.mrx);
	IDENTITY_M (4, 4, cam.mry);
	IDENTITY_M (4, 4, cam.mt);
	app_update_projection (window, cam.mp);
	PRINT_M4 (cam.mp, FMT_FLT);
	cam.ax = 0.0f;
	cam.ay = 0.0f;
	cam.az = 0.0f;

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
					app_update_projection (window, cam.mp);
					break;
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
					PRINT_M4 (cam.mvp, FMT_FLT);
					printf ("\n");
					break;
				}
				break;
			}
		}
		
		cam.mt [TX_M4] += keyboard [SDL_SCANCODE_D] * -0.01f;
		cam.mt [TX_M4] += keyboard [SDL_SCANCODE_A] * 0.01f;
		cam.mt [TZ_M4] += keyboard [SDL_SCANCODE_W] * 0.01f;
		cam.mt [TZ_M4] += keyboard [SDL_SCANCODE_S] * -0.01f;
		cam.mt [TY_M4] += keyboard [SDL_SCANCODE_SPACE] * -0.01f;
		cam.mt [TY_M4] += keyboard [SDL_SCANCODE_LCTRL] * 0.01f;
		cam.ax += keyboard [SDL_SCANCODE_UP]*0.01f;
		cam.ax += keyboard [SDL_SCANCODE_DOWN]*-0.01f;
		cam.ay += keyboard [SDL_SCANCODE_LEFT]*0.01f;
		cam.ay += keyboard [SDL_SCANCODE_RIGHT]*-0.01f;
		ROTX_M4 (cam.mrx, cam.ax);
		ROTY_M4 (cam.mry, cam.ay);
		mul_m4 (cam.mvp, cam.mrx, cam.mry);
		mul_m4 (cam.mvp, cam.mt, cam.mvp);
		mul_m4 (cam.mvp, cam.mp, cam.mvp);
		
		glUseProgram (program);
		glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, cam.mvp);
		glClearBufferfv (GL_COLOR, 0, scr_col);
		
		mesh_draw (&triangle);
		mesh_draw (&square);
		mesh_draw (&grid);
		
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

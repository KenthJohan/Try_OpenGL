#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "debug.h"
#include "mat.h"
#include "misc.h"
 
 
void GLAPIENTRY MessageCallback
(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam)
{
	fprintf (stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message);
}


struct Vertex
{
	GLfloat pos[4];
	GLfloat col[4];
};


GLuint Mesh_Setup(GLuint shader_prog, struct Vertex *verts, GLuint num_verts)
{
	GLuint vao;
	glCreateVertexArrays(1, &vao);

	GLuint pos_attrib_loc = glGetAttribLocation(shader_prog, "pos");
	GLuint col_attrib_loc = glGetAttribLocation(shader_prog, "col");

	glVertexArrayAttribFormat(vao, pos_attrib_loc, 3, GL_FLOAT, GL_FALSE, (GLuint)offsetof(struct Vertex, pos));
	glVertexArrayAttribFormat(vao, col_attrib_loc, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof(struct Vertex, col));

	glVertexArrayAttribBinding(vao, pos_attrib_loc, 0);
	glVertexArrayAttribBinding(vao, col_attrib_loc, 0);

	glEnableVertexArrayAttrib(vao, pos_attrib_loc);
	glEnableVertexArrayAttrib(vao, col_attrib_loc);

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(struct Vertex) * num_verts, verts, 0);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(struct Vertex));
	return vao;
}

 
 
int main(int argc, char *argv[])
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
	printf ("%s %i\n", "SDL_GL_CONTEXT_PROFILE_MASK", SDL_GL_CONTEXT_PROFILE_MASK);
	printf ("%s %i\n", "SDL_GL_CONTEXT_MAJOR_VERSION", SDL_GL_CONTEXT_MAJOR_VERSION);
	printf ("%s %i\n", "SDL_GL_CONTEXT_MINOR_VERSION", SDL_GL_CONTEXT_MINOR_VERSION);

	//App variables:
	SDL_Window * window;
	SDL_GLContext context;
	GLuint program;
	GLuint uniform_mvp;
	float mvp [4*4];
	float mrx [4*4];
	float mp [4*4];
	float ax = 0.0f;
	float mt [4*4];
	float temp [4*4];
	IDENTITY_M (4, 4, mvp);
	IDENTITY_M (4, 4, mrx);
	IDENTITY_M (4, 4, mt);
	CLR_V (4*4, mp);
	PERSPECTIVE_M4X4 (mp, 90.0f, 1024.0f/768.0f, 0.1f, 100.0f);
	PRINT_M4X4 (mp);
	
	window = SDL_CreateWindow ("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
	ASSERT_F (window != NULL, "SDL_CreateWindow: %s", SDL_GetError());
	
	context = SDL_GL_CreateContext (window);
	ASSERT_F (context != NULL, "SDL_GL_CreateContext: %s", SDL_GetError());

	glewExperimental = GL_TRUE;
	glewInit ();

	printf ("Using opengl version %s.\n", glGetString (GL_VERSION));
	printf ("Using glsl version %s.\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	//During init, enable debug output
	//glEnable (GL_DEBUG_OUTPUT);
	//glDebugMessageCallback (MessageCallback, 0);
	
	struct app_shader shaders [2] = 
	{
		{0, GL_VERTEX_SHADER, "shader.glvs"},
		{0, GL_FRAGMENT_SHADER, "shader.glfs"}
	};
	program = app_create_program (shaders, 2);
	uniform_mvp = glGetUniformLocation (program, "mvp");
	ASSERT_F (uniform_mvp >= 0, "glGetUniformLocation no uniform found.");

	
	struct Vertex tri_data [3] = 
	{
		[0].pos = { -0.5f, -0.5f, 0.0f, 1.0f }, [0].col = { 1.0f, 0.0f, 0.0f, 1.0f },
		[1].pos = {  0.5f, -0.5f, 0.0f, 1.0f }, [1].col = { 0.0f, 1.0f, 0.0f, 1.0f },
		[2].pos = {  0.0f,  0.5f, 0.0f, 1.0f }, [2].col = { 0.0f, 0.0f, 1.0f, 1.0f }
	};

	struct Vertex sqr_data [6] = 
	{
		[0].pos = { -0.5f, -0.5f, 0.0f, 1.0f }, [0].col = { 1.0f, 0.0f, 0.0f, 1.0f },
		[1].pos = {  0.5f, -0.5f, 0.0f, 1.0f }, [1].col = { 0.0f, 1.0f, 0.0f, 1.0f },
		[2].pos = {  0.5f,  0.5f, 0.0f, 1.0f }, [2].col = { 0.0f, 0.0f, 1.0f, 1.0f },
		[3].pos = {  0.5f,  0.5f, 0.0f, 1.0f }, [3].col = { 0.0f, 0.0f, 1.0f, 1.0f },
		[4].pos = { -0.5f,  0.5f, 0.0f, 1.0f }, [4].col = { 0.0f, 1.0f, 0.0f, 1.0f },
		[5].pos = { -0.5f, -0.5f, 0.0f, 1.0f }, [5].col = { 1.0f, 0.0f, 0.0f, 1.0f }
	};

	GLuint tri_vao = Mesh_Setup (program, tri_data, 3);
	GLuint sqr_vao = Mesh_Setup (program, sqr_data, 6);
	GLuint num_verts = 0;
	GLfloat scr_col[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	const Uint8 * keyboard = SDL_GetKeyboardState (NULL);

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
					
					case SDLK_z:
					glBindVertexArray (tri_vao);
					num_verts = 3;
					break;
					
					case SDLK_x:
					glBindVertexArray (sqr_vao);
					num_verts = 6;
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
					PRINT_M4X4 (mvp);
					printf ("\n");
					break;
					
					
					
				}
				break;
			}
		}
		
		mt [TX_M4X4] += keyboard [SDL_SCANCODE_D] * -0.01f;
		mt [TX_M4X4] += keyboard [SDL_SCANCODE_A] * 0.01f;
		mt [TZ_M4X4] += keyboard [SDL_SCANCODE_W] * 0.01f;
		mt [TZ_M4X4] += keyboard [SDL_SCANCODE_S] * -0.01f;
		mt [TY_M4X4] += keyboard [SDL_SCANCODE_SPACE] * -0.01f;
		mt [TY_M4X4] += keyboard [SDL_SCANCODE_LCTRL] * 0.01f;
		ax += keyboard [SDL_SCANCODE_LEFT]*0.1f;
		ax += keyboard [SDL_SCANCODE_RIGHT]*-0.1f;
		ROTY_M4X4 (mrx, ax);
		//MUL_M4X4 (mvp, mrx, mt);
		MUL_M4X4 (temp, mp, mt);
		MUL_M4X4 (mvp, mrx, temp);
		
		glUseProgram (program);
		glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, mvp);
		glClearBufferfv (GL_COLOR, 0, scr_col);
		glDrawArrays (GL_TRIANGLES, 0, num_verts);
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
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "debug.h"
#include "debug_gl.h"
#include "misc.h"
#include "camera.h"
#include "gen.h"
#include "shader.h"
#include "vertex.h"
#include "app.h"
#include "q.h"
#include "poi.h"
#include "map.h"




GLuint setup_verts ()
{
	float vertices [] = 
	{
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices [] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	glGenVertexArrays (1, &vao);
	glGenBuffers (1, &vbo);
	glGenBuffers (1, &ebo);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray (0);

	// color attribute
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray (1);

	// texture coord attribute
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray (2);

	return vao;
}






int main (int argc, char *argv[])
{
	struct Application app;
	app_init (&app, argc, argv);
	
	GLuint vao = setup_verts ();
	GLuint u_max = glGetUniformLocation (app.program, "maxu");
	GLuint u_iter = glGetUniformLocation (app.program, "iteru");
	GLuint u_translate = glGetUniformLocation (app.program, "translate");
	GLuint u_scale = glGetUniformLocation (app.program, "scale");
	
	glProgramUniform1f (app.program, u_max, 100);
	glProgramUniform1f (app.program, u_iter, 2000);
	
	float d = 0.1f;
	float s = 1.0f;
	float t [2] = {-0.63f, 0.0f};
		
	while (1)
	{
		if (app.flags & APP_QUIT) {break;}
		if (app.flags & APP_OPENGL_ERROR) {break;}
		app_frame_begin (&app);
		while (SDL_PollEvent (&app.event))
		{
			app_handle_input (&app);
			switch (app.event.type)
			{
				case SDL_KEYDOWN:
				switch (app.event.key.keysym.sym)
				{
					case SDLK_KP_6:
					t [0] += d;
					break;
					case SDLK_KP_4:
					t [0] -= d;
					break;
					case SDLK_KP_8:
					t [1] += d;
					break;
					case SDLK_KP_2:
					t [1] -= d;
					break;
					
					case SDLK_KP_PLUS:
					s += d;
					break;
					case SDLK_KP_MINUS:
					s -= d;
					break;
				}
				glProgramUniform2f (app.program, u_translate, t [0], t [1]);
				glProgramUniform1f (app.program, u_scale, 1.0f / s);
				break;
			}
		}
		

		app_update_camera (&app);
		
        glBindVertexArray (vao);
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}











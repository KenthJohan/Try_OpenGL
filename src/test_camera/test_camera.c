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
	struct Vertex
	{
		float p [3];
	};
	
	struct Vertex vertices [] =
	{
		{{ 0.5f,  0.5f, 0.0f}},
		{{ 0.5f, -0.5f, 0.0f}},
		{{-0.5f, -0.5f, 0.0f}},
		{{-0.5f,  0.5f, 0.0f}}
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
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*)0);
	glEnableVertexAttribArray (0);
	
	return vao;
}






int main (int argc, char *argv[])
{
	struct Application app;
	app_init (&app, argc, argv);
	
	GLuint vao = setup_verts ();
	
	while (1)
	{
		if (app.flags & APP_QUIT) {break;}
		if (app.flags & APP_OPENGL_ERROR) {break;}
		app_frame_begin (&app);
		while (SDL_PollEvent (&app.event))
		{
			app_handle_input (&app);
		}
		
		app_update_camera (&app);
		
        glBindVertexArray (vao);
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}











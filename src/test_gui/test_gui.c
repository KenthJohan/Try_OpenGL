#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "debug.h"
#include "debug_gl.h"
#include "shader.h"
#include "app.h"
#include "v.h"
#include "v2.h"
#include "map.h"
#include "gui.h"


struct Vertex
{
	float p [2];
	float c [4];
};


GLuint setup_verts (struct Vertex v [], size_t vn, unsigned int e [], size_t en)
{
	size_t const v8 = vn * sizeof (struct Vertex);
	size_t const e8 = en * sizeof (unsigned int);
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	glGenVertexArrays (1, &vao);
	glGenBuffers (1, &vbo);
	glGenBuffers (1, &ebo);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, v8, v, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, e8, e, GL_STATIC_DRAW);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*)0);
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*) (2 * sizeof (float)));
	glEnableVertexAttribArray (1);
	return vao;
}


void convert (struct Vertex v [], size_t vn, struct Rectangle_SWNE * x, size_t xn)
{
	srand(0);
	size_t j = 0;
	for (size_t i = 0; i < xn; ++ i)
	{
		vf32_cpy (v [j].p, (float *)&x [i].ne, 2);
		vf32_random (v [j].c, 3);
		v [j].c [3] = 1.0f;
		j ++;
		Rectangle_SWNE_SE ((struct v2f32_xy *)v [j].p, x + i);
		vf32_random (v [j].c, 3);
		v [j].c [3] = 1.0f;
		j ++;
		vf32_cpy (v [j].p, (float *)&x [i].sw, 2);
		vf32_random (v [j].c, 3);
		v [j].c [3] = 1.0f;
		j ++;
		Rectangle_SWNE_NW ((struct v2f32_xy *)v [j].p, x + i);
		vf32_random (v [j].c, 3);
		v [j].c [3] = 1.0f;
		j ++;
	}
}



GLuint test2 ()
{	
	struct Rectangle_SWNE x [2] =
	{
		{{0.0f, 0.0f},{0.1f, 0.1f}},
		{{0.2f, 0.0f},{0.3f, 0.1f}}
	};
	
	struct Vertex v [8];
	convert (v, 8, x, 2);
	
	unsigned int e [] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3, // second triangle
		
		4+0, 4+1, 4+3, // first triangle
		4+1, 4+2, 4+3  // second triangle
	};
	
	for (size_t i = 0; i < 8; ++ i)
	{
		printf ("%f %f : %f %f %f %f\n", v [i].p [0], v [i].p [1], v [i].c [0], v [i].c [1], v [i].c [2], v [i].c [3]);
	}
	fflush (stdout);
	
	return setup_verts (v, 8, e, 12);
}









GLuint test1 ()
{
	struct Vertex v [] =
	{
		{{ 0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{ 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
		{{-0.5f,  0.5f}, {0.5f, 0.0f, 0.5f, 1.0f}}
	};
	
	unsigned int e [] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	return setup_verts (v, 4, e, 6);
}



















int main (int argc, char *argv[])
{
	struct Application app;
	app_init (&app, argc, argv);
	
	GLuint vao1 = test1 ();
	GLuint vao2 = test2 ();

	
	while (1)
	{
		if (app.flags & APP_QUIT) {break;}
		if (app.flags & APP_OPENGL_ERROR) {break;}
		app_frame_begin (&app);
		
		while (SDL_PollEvent (&app.event))
		{
			app_handle_input (&app);
		}
		
		glBindVertexArray (vao1);
		glDrawElements (GL_LINES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray (vao2);
		glDrawElements (GL_LINES, 12, GL_UNSIGNED_INT, 0);
		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}











#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "debug.h"
#include "debug_gl.h"
#include "misc.h"
#include "gen.h"
#include "shader.h"
#include "vertex.h"
#include "app.h"
#include "q.h"
#include "poi.h"
#include "map.h"


/*
struct Vertex_Info
{
	char * name;
	GLuint location;
	GLuint dim;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	size_t offset;
};


void vinfo_print (struct Vertex_Info * x)
{
	printf ("%20s | ", x->name);
	printf ("%4i | ", (int)x->location);
	printf ("%4i | ", (int)x->dim);
	printf ("%4i | ", (int)x->type);
	printf ("%4i | ", (int)x->normalized);
	printf ("%4i | ", (int)x->stride);
	printf ("%4i", (int)x->offset);
	printf ("\n");
	fflush (stdout);
}



void show_structure ()
{
	struct Vertex_Info v;
	v.name = "Position";
	v.location = 0;
	v.dim = 4;
	v.type = GL_FLOAT;
	v.normalized = GL_FALSE;
	v.stride = sizeof (float) * 4;
	v.offset = 0;
	vinfo_print (&v);
}
*/

#define GUI_RECTANGLE 1
#define GUI_FLOAT 2

struct GUI_Type1
{
	uint8_t flags;
	uint8_t type;
	uint8_t dim;
	uint8_t shape;
	float * v;
	float * hit;
};




// v : ((SW, NE), (SW, NE), ...)
// r : ((SW, NE), (SW, NE), ...)
void intersect2d (float r [], float v [], size_t n, float x [2])
{
	size_t dim = 2;
	for (size_t i = 0; i < n; i = i + 1)
	{
		float * sw = v + i * (dim + 0);
		float * ne = v + i * (dim + 1);
		r [i] = vf32_lt_all (x, sw, dim) && vf32_lt_all (x, ne, dim);
	}
}


void gui1_hit_test (struct GUI_Type1 * g)
{
	float * v;
	size_t vn = 10;
	float * r;
	float p [2];
	size_t pn = 2;
	vf32_sub1 (r, v, vn, p, pn);
}








GLuint setup_verts ()
{
	struct Vertex
	{
		float p [2];
		float c [4];
	};
	
	struct Vertex vertices [] =
	{
		{{ 0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{ 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
		{{-0.5f,  0.5f}, {0.5f, 0.0f, 0.5f, 1.0f}}
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
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*)0);
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*) (2 * sizeof (float)));
	glEnableVertexAttribArray (1);
	
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
		
        glBindVertexArray (vao);
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}











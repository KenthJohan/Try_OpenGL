#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "debug_gl.h"
#include "vertex.h"
#include "mat.h"
#include "debug.h"

GLuint gpu_load_verts (GLuint program, struct Vertex * verts, GLuint count)
{
	GLuint vao;
	glCreateVertexArrays (1, &vao);

	GLuint pos_attrib_loc = glGetAttribLocation (program, "pos");
	ASSERT_F (pos_attrib_loc >= 0, "glGetAttribLocation no attribute found.");
	GL_CHECK_ERROR;
	
	GLuint col_attrib_loc = glGetAttribLocation (program, "col");
	ASSERT_F (col_attrib_loc >= 0, "glGetAttribLocation no attribute found.");
	GL_CHECK_ERROR;

	glVertexArrayAttribFormat (vao, pos_attrib_loc, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, pos));
	GL_CHECK_ERROR;
	
	glVertexArrayAttribFormat (vao, col_attrib_loc, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, col));
	GL_CHECK_ERROR;

	glVertexArrayAttribBinding (vao, pos_attrib_loc, 0);
	glVertexArrayAttribBinding (vao, col_attrib_loc, 0);

	glEnableVertexArrayAttrib (vao, pos_attrib_loc);
	glEnableVertexArrayAttrib (vao, col_attrib_loc);

	GLuint vbo;
	glCreateBuffers (1, &vbo);
	glNamedBufferStorage (vbo, sizeof (struct Vertex) * count, verts, 0);

	glVertexArrayVertexBuffer (vao, 0, vbo, 0, sizeof (struct Vertex));
	return vao;
}


void gen_grid (struct Vertex * verts, size_t count, float x1, float x2, float y1, float y2, float r)
{
	float xx;
	float yy;
	//float const xd = (x2 - x1);
	//float const yd = (y2 - y1);
	size_t j = 0;
	for (size_t i = 0; i < count; i = i + 1)
	{

		if (j == count){return;}
		xx = x1 + r * (float)i;
		yy = y1;
		V4_SET (verts [j].pos, xx, -0.5f, yy, 1.0f);
		V4_SET (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;
		
		if (j == count){return;}
		xx = x1 + r * (float)i;
		yy = y2;
		V4_SET (verts [j].pos, xx, -0.5f, yy, 1.0f);
		V4_SET (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;

		if (j == count){return;}
		xx = x1;
		yy = y1 + r * (float)i;
		V4_SET (verts [j].pos, xx, -0.5f, yy, 1.0f);
		V4_SET (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;
		
		if (j == count){return;}
		xx = x2;
		yy = y1 + r * (float)i;
		V4_SET (verts [j].pos, xx, -0.5f, yy, 1.0f);
		V4_SET (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;
		
	}
}


void gen_circle (struct Vertex * v, size_t n, float r, float x, float y, float z)
{
	for (size_t i = 0; i < n; i = i + 1)
	{
		V4_SET (v [i].col, 0.0f, 0.0f, 1.0f, 1.0f);
		float a = (2 * M_PI) * ((float)i / (float)n);
		v [i].pos [0] = x + r * cos (a);
		v [i].pos [1] = y + r * sin (a);
		v [i].pos [2] = z;
		v [i].pos [3] = 1.0f;
		//printf ("%f %f %f %f : %f %f %f %f\n", v [i].pos [0], v [i].pos [1], v [i].pos [2], v [i].pos [3], v [i].col [0], v [i].col [1], v [i].col [2], v [i].col [3]);
	}
}

void gen_circle1 (struct Vertex * v, size_t n, float r, float x, float y, float z)
{
	size_t i = 0;
	float a;
	if (i >= n) {return;}
	V4_SET (v [i].col, 0.0f, 0.0f, 1.0f, 1.0f);
	v [i].pos [0] = x;
	v [i].pos [1] = y;
	v [i].pos [2] = z;
	v [i].pos [3] = 1.0f;
	i ++;
	
	while (1)
	{
		if (i >= n) {break;}
		a = (2 * M_PI) * ((float)i / (float)(n));
		v [i].pos [0] = x + r * cos (a);
		v [i].pos [1] = y + r * sin (a);
		v [i].pos [2] = z;
		v [i].pos [3] = 1.0f;
		i ++;
	}
}



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




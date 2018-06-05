#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "mat.h"
#include "vertex.h"
#include "debug.h"
#include "debug_gl.h"


char * app_malloc_file (char * filename)
{
	ASSERT_F (filename != NULL, "filename is NULL%s", "");
	FILE * file = fopen (filename, "rb");
	ASSERT_F (file != NULL, "file is NULL%s", "");
	fseek (file, 0, SEEK_END);
	int length = ftell (file);
	fseek (file, 0, SEEK_SET);
	char * buffer = malloc (length + 1);
	ASSERT_F (buffer != NULL, "buffer is NULL%s", "");
	memset (buffer, 0, length + 1);
	//buffer [length + 1] = 0;
	{
		size_t r = fread (buffer, length, 1, file);
		ASSERT_F (r == 1, "fread error %i %i", (int)r, (int)length);
	}
	fclose (file);
	return buffer;
}


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


void sdl_get_mouse (SDL_Window * window)
{
	int w;
	int h;
	SDL_GetWindowSize (window, &w, &h);
	int x;
	int y;
	SDL_GetMouseState (&x, &y);
	printf ("(%f %f)\n", (float)x/(float)w - 0.5f, (float)y/(float)h - 0.5f);
}

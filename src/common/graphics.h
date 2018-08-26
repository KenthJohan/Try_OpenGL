#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "debug_gl.h"
#include "vertex.h"
#include "mat.h"
#include "debug.h"


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



//GR_ALLOCATE: Data is dynamic allocated. 
#define GR_ALLOCATE (1 << 0)
//GR_UPDATE: Update GPU buffer next call.
#define GR_UPDATE (1 << 1)
#define GR_UPDATE_ONCE (1 << 2)
#define GR_DRAW (1 << 2)
#define GR_DRAW_ONCE (1 << 3)
#define GR_FLAG3 (1 << 4)


struct GR_Object
{
	uint32_t flags;
	GLuint program;
	GLuint vao;
	GLenum mode;
	GLuint vbo;
	GLbitfield vbo_flags;
	GLuint offset8;
	GLuint offset;
	size_t count;
	size_t size8;
	void * data;
};


//Copy object data to VBO for all objects that has GR_UPDATE flag set.
void gr_update (struct GR_Object * v0, size_t n)
{
	for (struct GR_Object * v = v0; v < (v + n); v ++)
	{
		if (v->flags & GR_UPDATE_ONCE)
		{
			v->flags &= ~GR_UPDATE_ONCE;
			glNamedBufferSubData (v->vbo, v->offset8, v->size8, v->data);
			GL_CHECK_ERROR;
		}
		else if (v->flags & GR_UPDATE)
		{
			glNamedBufferSubData (v->vbo, v->offset8, v->size8, v->data);
			GL_CHECK_ERROR;
		}
	}
}


//Unefficent render loop.
//Use this for unsorted objects.
void gr_draw (struct GR_Object * v0, size_t n)
{
	for (struct GR_Object * v = v0; v < (v + n); v ++)
	{
		glUseProgram (v->program);
		glBindVertexArray (v->vao);
		glDrawArrays (v->mode, v->offset, v->count);
		GL_CHECK_ERROR;
	}
}


void gr_init (struct GR_Object * v0, size_t n)
{
	for (struct GR_Object * v = v0; v < (v + n); v ++)
	{
		glCreateBuffers (1, &(v->vbo));
		glNamedBufferStorage (v->vbo, v->size8, v->data, v->vbo_flags);
		glVertexArrayVertexBuffer (v->vao, 0, v->vbo, 0, sizeof (struct Vertex));
		GL_CHECK_ERROR;
		if ((v->data == NULL) && (v->flags & GR_ALLOCATE)) 
		{
			v->data = malloc (v->size8);
		}
	}
}



//Get common objects that has common program and vao.
size_t gr_get_common 
(
	struct GR_Object * xv, size_t xn, 
	struct GR_Object * yv, size_t yn,
	GLuint program,
	GLuint vao
)
{
	size_t xi = 0;
	size_t yi = 0;
	while (1)
	{
		if (xi >= xn) {return yi;}
		if (yi >= yn) {return yi;}
		if (xv [xi].program != program) {xi ++; continue;}
		if (xv [xi].vao != vao) {xi ++; continue;}
		yv [yi] = xv [xi];
		xi ++;
		yi ++;
	}
}













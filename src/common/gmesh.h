#pragma once

#include <stdint.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "debug_gl.h"
#include "vertex.h"
#include "mat.h"
#include "debug.h"
#include "gbuf.h"

#define GMESH_DRAW (1 << 0)
#define GMESH_DRAW_ONCE (1 << 1)
#define GMESH_UPDATE (1 << 2)
#define GMESH_UPDATE_ONCE (1 << 3)


struct GMesh
{
	uint32_t flags;
	GLuint program;
	GLuint vao;
	GLenum mode;
	GLuint vbo;
	GLbitfield vbo_flags;
	GLuint draw_offset;
	GLuint draw_count;
	void * data;
};



void gmesh_init (struct GMesh * m)
{
	GBUF_LOOP (size_t, i, m)
	{
		struct GBuffer * g = gbuf (m [i].data);
		size_t const cap8 = g->cap * g->esize8;
		glCreateVertexArrays (1, &(m [i].vao));
		GLuint l [2];
		l [0] = glGetAttribLocation (m [i].program, "pos");
		l [1] = glGetAttribLocation (m [i].program, "col");
		ASSERT_F (l [0] >= 0, "glGetAttribLocation no attribute found.");
		ASSERT_F (l [1] >= 0, "glGetAttribLocation no attribute found.");
		glVertexArrayAttribFormat (m [i].vao, l [0], 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, pos));
		glVertexArrayAttribFormat (m [i].vao, l [1], 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, col));
		glVertexArrayAttribBinding (m [i].vao, l [0], 0);
		glVertexArrayAttribBinding (m [i].vao, l [1], 0);
		glEnableVertexArrayAttrib (m [i].vao, l [0]);
		glEnableVertexArrayAttrib (m [i].vao, l [1]);
		glCreateBuffers (1, &(m [i].vbo));
		glNamedBufferStorage (m [i].vbo, cap8, m [i].data, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayVertexBuffer (m [i].vao, 0, m [i].vbo, 0, sizeof (struct Vertex));
		GL_CHECK_ERROR;
	}
}


void gmesh_draw (struct GMesh * m)
{
	GBUF_LOOP (size_t, i, m)
	{
		if (m->flags & GMESH_DRAW)
		{
			glBindVertexArray (m [i].vao);
			glDrawArrays (m [i].mode, m [i].draw_offset, m [i].draw_count);
			GL_CHECK_ERROR;
		}
		if (m [i].flags & GMESH_DRAW_ONCE)
		{
			m [i].flags &= ~GMESH_DRAW;
		}
	}
}




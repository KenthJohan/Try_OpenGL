#pragma once

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

#include "vertex.h"
#include "gbuf.h"
#include "camera.h"

#define GMESH_DRAW (1 << 0)
#define GMESH_DRAW_ONCE (1 << 1)
#define GMESH_UPDATE (1 << 2)
#define GMESH_UPDATE_ONCE (1 << 3)


struct GMesh
{
	GLuint program;
	GLuint vao;
	GLuint vbo;
	uint32_t flags;
	GLenum mode;
	struct Cam * cam;
	float mm [16];
	void * data;
};


void gmesh_init (struct GMesh * m, GLuint program)
{
	GBUF_LOOP (size_t, i, m)
	{
		if (m [i].flags == 0) {continue;}
		glCreateVertexArrays (1, &(m [i].vao));
		GLuint l [2];
		l [0] = glGetAttribLocation (program, "pos");
		l [1] = glGetAttribLocation (program, "col");
		ASSERT_F (l [0] >= 0, "glGetAttribLocation no attribute found.");
		ASSERT_F (l [1] >= 0, "glGetAttribLocation no attribute found.");
		glVertexArrayAttribFormat (m [i].vao, l [0], 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, pos));
		glVertexArrayAttribFormat (m [i].vao, l [1], 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, col));
		glVertexArrayAttribBinding (m [i].vao, l [0], 0);
		glVertexArrayAttribBinding (m [i].vao, l [1], 0);
		glEnableVertexArrayAttrib (m [i].vao, l [0]);
		glEnableVertexArrayAttrib (m [i].vao, l [1]);
		glCreateBuffers (1, &(m [i].vbo));
		glNamedBufferStorage (m [i].vbo, gbuf_cap8 (m [i].data), m [i].data, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayVertexBuffer (m [i].vao, 0, m [i].vbo, 0, sizeof (struct Vertex));
		TRACE_F ("");
		GL_CHECK_ERROR;
	}
}


void gmesh_draw (struct GMesh * m, GLuint uniform_mvp)
{
	GBUF_LOOP (size_t, i, m)
	{
		uint32_t n = gbuf_count (m [i].data);
		//TRACE_F ("%i", n);
		if (n == 0) {continue;}
		if (m [i].flags == 0) {continue;}
		if (m [i].flags & GMESH_DRAW) 
		{
			cam_mvp_update (m [i].cam, m [i].mm, uniform_mvp);
			glBindVertexArray (m [i].vao);
			glDrawArrays (m [i].mode, 0, n);
		}
		else if (m [i].flags & GMESH_DRAW_ONCE)
		{
			m [i].flags &= ~GMESH_DRAW;
		}
		GL_CHECK_ERROR;
	}
}


void mesh_update (struct GMesh * m)
{
	GBUF_LOOP (size_t, i, m)
	{
		if (m [i].flags == 0) {continue;}
		GLsizeiptr const size8 = gbuf_count (m);
		GLintptr const offset8 = 0;
		if (m [i].flags & GMESH_UPDATE) 
		{
			glNamedBufferSubData (m [i].vbo, offset8, size8, m [i].data);
		}
		if (m [i].flags & GMESH_UPDATE_ONCE) 
		{
			m [i].flags &= ~GMESH_UPDATE;
		}
		GL_CHECK_ERROR;
	}
}

















#define MESH_INVALID 1 << 0
#define MESH_INITIALIZED 1 << 1
#define MESH_DESTROY 1 << 2
#define MESH_UPDATE_ONCE 1 << 3



struct Mesh
{
	uint32_t flags;
	GLuint program;
	GLuint vao;
	GLuint vbo;
	
	//Specifies what kind of primitives to render. 
	//* GL_POINTS, 
	//* GL_LINE_STRIP, 
	//* GL_LINE_LOOP, 
	//* GL_LINES, 
	//* GL_LINE_STRIP_ADJACENCY, 
	//* GL_LINES_ADJACENCY, 
	//* GL_TRIANGLE_STRIP, 
	//* GL_TRIANGLE_FAN, 
	//* GL_TRIANGLES, 
	//* GL_TRIANGLE_STRIP_ADJACENCY, 
	//* GL_TRIANGLES_ADJACENCY
	//* GL_PATCHES.
	GLenum mode;
	
	//Model matrix.
	float mm [16];
	
	//Model data.
	void * data;
};



void mesh_init (struct Mesh * m, size_t n, GLuint program)
{
	for (size_t i = 0; i < n; ++ i)
	{
		if (m [i].flags & MESH_INVALID) {continue;}
		
		glCreateVertexArrays (1, &(m [i].vao));
		GLuint l [2];
		l [0] = glGetAttribLocation (program, "pos");
		l [1] = glGetAttribLocation (program, "col");
		ASSERT_F (l [0] >= 0, "glGetAttribLocation no attribute found.");
		ASSERT_F (l [1] >= 0, "glGetAttribLocation no attribute found.");
		glVertexArrayAttribFormat (m [i].vao, l [0], 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, pos));
		glVertexArrayAttribFormat (m [i].vao, l [1], 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, col));
		glVertexArrayAttribBinding (m [i].vao, l [0], 0);
		glVertexArrayAttribBinding (m [i].vao, l [1], 0);
		glEnableVertexArrayAttrib (m [i].vao, l [0]);
		glEnableVertexArrayAttrib (m [i].vao, l [1]);
		glCreateBuffers (1, &(m [i].vbo));
		glNamedBufferStorage (m [i].vbo, gbuf_cap8 (m [i].data), m [i].data, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayVertexBuffer (m [i].vao, 0, m [i].vbo, 0, sizeof (struct Vertex));
		TRACE_F ("");
		GL_CHECK_ERROR;
		
		m [i].flags |= MESH_INITIALIZED;
	}
}







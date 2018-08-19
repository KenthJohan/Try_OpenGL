#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>

struct Vertex
{
	float pos [4];
	float col [4];
};


GLuint vertex_get_vao (GLuint program)
{
	GLuint vao;
	glCreateVertexArrays (1, &vao);
	//TODO: This is too unflexiable. Seperate away specific AttribFormat.
	GLuint p = glGetAttribLocation (program, "pos");
	ASSERT_F (p >= 0, "glGetAttribLocation no attribute found.");
	GLuint c = glGetAttribLocation (program, "col");
	ASSERT_F (c >= 0, "glGetAttribLocation no attribute found.");
	glVertexArrayAttribFormat (vao, p, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, pos));
	glVertexArrayAttribFormat (vao, c, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, col));
	glVertexArrayAttribBinding (vao, p, 0);
	glVertexArrayAttribBinding (vao, c, 0);
	glEnableVertexArrayAttrib (vao, p);
	glEnableVertexArrayAttrib (vao, c);
	return vao;
}

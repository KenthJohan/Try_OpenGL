#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>

struct Mesh
{
	GLenum mode;
	GLuint vao;
	struct Vertex * vert_data;
	size_t vert_count;
};


void mesh_draw (struct Mesh * mesh)
{
	glBindVertexArray (mesh->vao);
	glDrawArrays (mesh->mode, 0, mesh->vert_count);
}

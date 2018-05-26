#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "mat.h"


struct app_shader
{
	GLuint handle;
	GLenum kind;
	char * filename;
};


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


void app_create_shader (struct app_shader * shader)
{
	ASSERT_F 
	(
		(shader->kind == GL_VERTEX_SHADER) || 
		(shader->kind == GL_FRAGMENT_SHADER), 
		"Unsupported shader kind%s", ""
	);
	shader->handle = glCreateShader (shader->kind);
	ASSERT_F (shader != 0, "glCreateShader error%s", "");
	char * buffer = app_malloc_file (shader->filename);
	glShaderSource (shader->handle, 1, (const GLchar **) &buffer, NULL);
	free (buffer);
	glCompileShader (shader->handle);
	GLint status;
	glGetShaderiv (shader->handle, GL_COMPILE_STATUS, &status);
	char err_buf [512];
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog (shader->handle, sizeof (err_buf), NULL, err_buf);
		err_buf [sizeof (err_buf) - 1] = '\0';
	}
	ASSERT_F (status == GL_TRUE, "Vertex shader compilation failed: %s", err_buf);
}


GLuint app_create_program (struct app_shader * shaders, size_t count)
{
	GLuint program;
	program = glCreateProgram ();
	ASSERT_F (program != 0, "glCreateProgram error%s", "");
	for (size_t i = 0; i < count; i = i + 1)
	{
		app_create_shader (shaders + i);
		glAttachShader (program, shaders [i].handle);
	}
	glLinkProgram (program);
	//The shader objects are needed only for linking the program.
	//We can delete them after the program is linked.
	for (size_t i = 0; i < count; i = i + 1)
	{
		glDetachShader (program, shaders [i].handle);
		glDeleteShader (shaders [i].handle);
		shaders [i].handle = 0;
	}
	return program;
}


struct Vertex
{
	GLfloat pos [4];
	GLfloat col [4];
};


GLuint gpu_load_verts (GLuint program, struct Vertex * verts, GLuint count)
{
	GLuint vao;
	glCreateVertexArrays (1, &vao);

	GLuint pos_attrib_loc = glGetAttribLocation (program, "pos");
	GLuint col_attrib_loc = glGetAttribLocation (program, "col");

	glVertexArrayAttribFormat (vao, pos_attrib_loc, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, pos));
	glVertexArrayAttribFormat (vao, col_attrib_loc, 4, GL_FLOAT, GL_FALSE, (GLuint)offsetof (struct Vertex, col));

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
		SET_V4 (verts [j].pos, xx, -0.5f, yy, 1.0f);
		SET_V4 (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;
		
		if (j == count){return;}
		xx = x1 + r * (float)i;
		yy = y2;
		SET_V4 (verts [j].pos, xx, -0.5f, yy, 1.0f);
		SET_V4 (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;

		if (j == count){return;}
		xx = x1;
		yy = y1 + r * (float)i;
		SET_V4 (verts [j].pos, xx, -0.5f, yy, 1.0f);
		SET_V4 (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;
		
		if (j == count){return;}
		xx = x2;
		yy = y1 + r * (float)i;
		SET_V4 (verts [j].pos, xx, -0.5f, yy, 1.0f);
		SET_V4 (verts [j].col, 1.0f, 0.0f, 0.0f, 1.0f);
		j = j + 1;
		
	}
}


void app_make_perspective (SDL_Window * window, float m [4*4])
{
	int w;
	int h;
	SDL_GetWindowSize (window, &w, &h);
	CLR_V (4*4, m);
	perspective_m4x4 (m, 45.0f, (float)w/(float)h, 0.1f, 100.0f);
}


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




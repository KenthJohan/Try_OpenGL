#pragma once
#include <GL/glew.h>


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
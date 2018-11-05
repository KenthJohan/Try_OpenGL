#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

//Common simple c functions
#include "SDLGL.h"
#include "debug.h"
#include "debug_gl.h"
#include "v4.h"
#include "shader.h"


#include "app.h"
#include "color.h"


uint32_t map [] = 
{
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};






void update_square (GLuint vbo, uint32_t index, float x, float y, float w, float h)
{
	ASSERT (glIsBuffer (vbo));
	float v [4*6 * 2];
	gen_grid (4*6 * 2, v);
	GLenum const target = GL_ARRAY_BUFFER;
	GLintptr const offset = index * sizeof (float) * 4 * 6;
	GLsizeiptr const size = sizeof (v);
	glBindBuffer (target, vbo);
	glBufferSubData (target, offset, size, v); 
}


void update_layer (GLuint vbo, uint32_t index, uint32_t layer)
{
	ASSERT (glIsBuffer (vbo));
	float v [6] = {layer,layer,layer,layer,layer,layer};
	GLenum const target = GL_ARRAY_BUFFER;
	GLintptr const offset = index * sizeof (v);
	GLsizeiptr const size = sizeof (v);
	glBindBuffer (target, vbo);
	glBufferSubData (target, offset, size, v); 
}







void setup_font (FT_Face face)
{
	GLuint texture;
	GLenum const target = GL_TEXTURE_2D_ARRAY;
	GLsizei const width = 50;
	GLsizei const height = 50;
	GLsizei const layerCount = 128;
	GLsizei const mipLevelCount = 1;
	GLenum const internalformat = GL_R8;
	glGenTextures (1, &texture);
	glBindTexture (target, texture);
	glTexStorage3D (target, mipLevelCount, internalformat, width, height, layerCount);
	GL_CHECK_ERROR;
	
	//IMPORTANT. Disable byte-alignment restriction.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	for (GLubyte c = 0; c < 128; c++)
	{
		{
			int r = FT_Load_Char (face, c, FT_LOAD_RENDER);
			ASSERT_F (r == 0, "ERROR::FREETYTPE: Failed to load Glyph %i", r);
			if (r) {continue;}
		}
		
		GLint xoffset = 0;
		GLint yoffset = 0;
		GLint zoffset = c;
		
		TRACE_F ("glyph %i %i", face->glyph->bitmap.width, face->glyph->bitmap.rows);
		
		glTexSubImage3D 
		(
			target, 
			0, 
			xoffset, 
			yoffset, 
			zoffset, 
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			1, 
			GL_RED,
			GL_UNSIGNED_BYTE, 
			face->glyph->bitmap.buffer
		);
		GL_CHECK_ERROR;
	}
	
	// Always set reasonable texture parameters
	glTexParameteri (target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}




void update_text 
(
	GLuint vbo [2], 
	uint32_t vfirst, 
	uint32_t * vcount, 
	uint32_t vcapacity, 
	char const * text,
	float x, 
	float y
)
{
	uint32_t const vdim0 = 4;
	uint32_t const vdim1 = 1;
	uint32_t const vsize0 = sizeof (float) * vdim0;
	uint32_t const vsize1 = sizeof (float) * vdim1;
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	GLintptr const offset0 = vfirst * vsize0;
	GLintptr const offset1 = vfirst * vsize1;
	GLsizeiptr const length0 = vcapacity * vsize0;
	GLsizeiptr const length1 = vcapacity * vsize1;
	float w = 0.1f;
	float h = 0.1f;
	
	glBindBuffer (target, vbo [0]);
	float * p = glMapBufferRange (target, offset0, length0, access);
	GL_CHECK_ERROR;
	glBindBuffer (target, vbo [1]);
	float * l = glMapBufferRange (target, offset1, length1, access);
	char const * c = text;
	uint32_t vi;
	for (vi = 0; vi < vcapacity; vi += 6)
	{
		if ((*c) == '\0') {break;}
		gen_square (p + (vdim0 * vi), x, y, w, h);
		gen_layer (l + (vdim1 * vi), (float)(*c));
		x += w;
		c ++;
	}
	(*vcount) = vi;
	
	glBindBuffer (target, vbo [0]);
	glUnmapBuffer (target);
	glBindBuffer (target, vbo [1]);
	glUnmapBuffer (target);
	GL_CHECK_ERROR;
}








int main (int argc, char *argv[])
{
	uint32_t flags = 0;
	app_init (&flags);
	
	SDL_Window * window;
	SDL_GLContext glcontext;
	const Uint8 * keyboard;
	SDL_Event event;
	FT_Library ft;
	FT_Face face;
	{
		int r = FT_Init_FreeType (&ft);
		ASSERT_F (r == 0, "ERROR::FREETYPE: Could not init FreeType Library %i", r);
	}
	{
		int r = FT_New_Face(ft, "fonts/arial.ttf", 0, &face);
		ASSERT_F (r == 0, "ERROR::FREETYPE: Failed to load font %i", r);
	}
	FT_Set_Pixel_Sizes (face, 0, 50);
	
	window = app_create_window ();
	
	glcontext = SDL_GL_CreateContext_CC (window);
	keyboard = SDL_GetKeyboardState (NULL);
	
	glDebugMessageCallback (glDebugOutput, 0);
	glDebugMessageControl (GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	//glEnable (GL_SCISSOR_TEST);
	//glEnable (GL_STENCIL_TEST);
	//glEnable (GL_DEPTH_TEST);
	glEnable (GL_DEBUG_OUTPUT);
	glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClearColor (0.1f, 0.1f, 0.2f, 0.0f);
	glPointSize (4.0f);

	GLuint program = gl_program_from_filename ("src/template1/shader.glfs;src/template1/shader.glvs");
	gl_shader_debug1 (program);
	glUseProgram (program);
	
	
	#define DRAWGROUP_COUNT 10
	struct
	{
		uint32_t first [DRAWGROUP_COUNT];
		uint32_t count [DRAWGROUP_COUNT];
		uint32_t capacity [DRAWGROUP_COUNT];
	} drawdata;
	vu32_set1 (DRAWGROUP_COUNT, drawdata.first, 0);
	vu32_set1 (DRAWGROUP_COUNT, drawdata.count, 0);
	vu32_set1 (DRAWGROUP_COUNT, drawdata.capacity, 0);
	vu32_setl (drawdata.capacity, DRAWGROUP_COUNT, 100, 0, 0, 0, 0, 0, 24, 0, 0, 0);
	gen_buffer_slots (DRAWGROUP_COUNT, drawdata.first, drawdata.capacity);
	
	
	GLuint vao;
	GLuint vbo [2];
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	gpu_setup_vertex (vbo, drawdata.first [DRAWGROUP_COUNT-1]);
	setup_font (face);

	update_text 
	(
		vbo, 
		drawdata.first [0], 
		drawdata.count + 0, 
		drawdata.capacity [0], 
		"Hello",
		0.0f,
		0.0f
	);
	
	update_text 
	(
		vbo, 
		drawdata.first [6], 
		drawdata.count + 6, 
		drawdata.capacity [6], 
		"XYZÅÄÖ",
		0.0f,
		0.5f
	);





	while (1)
	{
		if (flags & APP_QUIT) {break;}
		if (flags & APP_ERROR) {break;}
		
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				flags |= APP_QUIT;
				break;
				
				case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					event.type = SDL_QUIT;
					SDL_PushEvent (&event);
					break;
					case SDLK_c:
					update_layer (vbo [1], 0, 0);
					break;
					case SDLK_v:
					update_layer (vbo [1], 0, 1);
					break;
				}
				break;
			}
		}
		
		
		
		glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);
		
		app_draw (DRAWGROUP_COUNT, drawdata.first, drawdata.count);
		//glDrawArrays (GL_TRIANGLES, 0, 5*6);
		//glDrawArrays (GL_POINTS, 0, 12);
		//glDrawArrays (GL_LINES, 0, 6*100);

		
		SDL_GL_SwapWindow (window);	
		SDL_Delay (1);
	}
	
	
	SDL_GL_DeleteContext (glcontext);
	
	return 0;
}












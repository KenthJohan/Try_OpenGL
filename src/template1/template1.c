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


struct v4f64_xyuv
{
	float x;
	float y;
	float u;
	float v;
};


void gen_square (float v [48], float x, float y, float w, float h)
{
	float v0 [] =
	{
		x,     y + h,   0.0, 0.0,            
		x,     y,       0.0, 1.0,
		x + w, y,       1.0, 1.0,
		x,     y + h,   0.0, 0.0,
		x + w, y,       1.0, 1.0,
		x + w, y + h,   1.0, 0.0       
	};
	memcpy (v, v0, sizeof (v0));
}


void gen_grid (uint32_t n, float v [])
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.1f;
	float h = 0.1f;
	for (uint32_t i = 0; i < n; i += 4*6)
	{
		gen_square (v + i, x, y, w, h);
		x += 0.1f;
		y += 0.1f;
	}
}


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


void setup (GLuint vbo [], uint32_t n)
{
	GLenum target = GL_ARRAY_BUFFER;
	GLenum type = GL_FLOAT;
	GLboolean normalized = GL_FALSE;
	const GLvoid * pointer = 0;
	GLsizei stride0 = sizeof (float) * 4;
	GLsizei stride1 = sizeof (float) * 1;
	GLuint index0 = 0;
	GLuint index1 = 1;
	GLint size0 = 4;
	GLint size1 = 1;
	glGenBuffers (2, vbo);
	
	glBindBuffer (target, vbo [0]);
	glBufferData (target, stride0 * n, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray (index0);
	glVertexAttribPointer (index0, size0, type, normalized, stride0, pointer);
	
	glBindBuffer (target, vbo [1]);
	glBufferData (target, stride1 * n, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray (index1);
	glVertexAttribPointer (index1, size1, type, normalized, stride1, pointer);
}


void setup_texture ()
{
	GLuint texture;
	GLsizei const width = 2;
	GLsizei const height = 2;
	GLsizei const layerCount = 2;
	GLsizei const mipLevelCount = 1;
	// Read you texels here. In the current example, we have 2*2*2 = 8 texels, with each texel being 4 GLubytes.
	GLubyte texels[32] = 
	{
		 // Texels for first image.
		 0,   0,   0,   255,
		 255, 0,   0,   255,
		 0,   255, 0,   255,
		 0,   0,   255, 255,
		 // Texels for second image.
		 255, 255, 255, 255,
		 255, 255,   0, 255,
		 0,   255, 255, 255,
		 255, 0,   255, 255,
	};
	glGenTextures (1,&texture);
	glBindTexture (GL_TEXTURE_2D_ARRAY,texture);
	// Allocate the storage.
	glTexStorage3D (GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);
	// Upload pixel data.
	// The first 0 refers to the mipmap level (level 0, since there's only 1)
	// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
	// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
	// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
	glTexSubImage3D (GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	// Always set reasonable texture parameters
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}


void setup_font (FT_Face face)
{
	GLuint texture;
	GLsizei const width = 50;
	GLsizei const height = 50;
	GLsizei const layerCount = 128;
	GLsizei const mipLevelCount = 1;
	GLenum const internalformat = GL_R8;
	glGenTextures (1,&texture);
	glBindTexture (GL_TEXTURE_2D_ARRAY,texture);
	glTexStorage3D (GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_R8, width, height, layerCount);
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
			GL_TEXTURE_2D_ARRAY, 
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
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
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
	//glClearColor (0.1f, 0.1f, 0.2f, 0.0f);
	glPointSize (4.0f);

	GLuint program = gl_program_from_filename ("src/template1/shader.glfs;src/template1/shader.glvs");
	gl_shader_debug1 (program);
	glUseProgram (program);
	
	GLuint vao;
	GLuint vbo [2];
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	setup (vbo, 12);
	update_square (vbo [0], 0, 0.0f, 0.0f, 0.4f, 0.4f);
	update_layer (vbo [1], 0, 'X');
	update_layer (vbo [1], 1, 'W');
	//setup_texture ();
	setup_font (face);


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
		glDrawArrays (GL_TRIANGLES, 0, 12);

		
		SDL_GL_SwapWindow (window);	
		SDL_Delay (1);
	}
	
	
	SDL_GL_DeleteContext (glcontext);
	
	return 0;
}












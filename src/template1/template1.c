/*
https://github.com/KerryL/LibPlot2D/blob/f0b7deca6c9af24c8daab13cc17dd5ab30d1ac06/src/renderer/text.cpp

*/

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

//Common simple c functions
#include <csc/SDLGL.h>
#include <csc/debug.h>
#include <csc/debug_gl.h>
#include <csc/v4.h>
#include <csc/shader.h>
#include <csc/gen.h>
#include <csc/gtext.h>
#include <csc/xxgl.h>

#include "app.h"


uint32_t map [] = 
{
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};
 


float * glMapBufferRange_4fv (uint32_t offset, uint32_t length)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	uint32_t const vdim = 4;
	uint32_t const vsize = sizeof (float) * vdim;
	GLintptr const offset8 = offset * vsize;
	GLsizeiptr const length8 = length * vsize;
	float * buffer = glMapBufferRange (target, offset8, length8, access);
	ASSERT (buffer);
	return buffer;
}



void RenderBox 
(
	GLuint vbo [3],
	float x, 
	float y, 
	float w, 
	float h, 
	uint32_t offset, 
	uint32_t length
)
{
	float * v;
	glBindBuffer (GL_ARRAY_BUFFER, vbo [0]);
	v = glMapBufferRange_4fv (offset, length);
	gen_square_pos (v, x, y, w, h);
	glUnmapBuffer (GL_ARRAY_BUFFER);
	glDrawArrays (GL_TRIANGLES, offset, length);
}




struct gui_textbox
{
	uint32_t di;
	float color [4];
	float pos [4];
	char * text;
};












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
	FT_Set_Pixel_Sizes (face, 0, 48);
	
	window = app_create_window ();
	
	glcontext = SDL_GL_CreateContext_CC (window);
	keyboard = SDL_GetKeyboardState (NULL);
	ASSERT (keyboard);
	
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
	
	
	struct xxgl_drawrange dr;
	dr.n = DI_N;
	xxgl_drawrange_calloc (&dr);
	dr.capacity  [DI_BOX1] = 6*10*10;
	dr.length    [DI_BOX1] = 6*10*10;
	dr.primitive [DI_BOX1] = GL_TRIANGLES;
	dr.capacity  [DI_TEXT] = 6*7;
	dr.length    [DI_TEXT] = 6*7;
	dr.primitive [DI_TEXT] = GL_TRIANGLES;
	dr.capacity  [DI_BOX2] = 6*10;
	dr.length    [DI_BOX2] = 6*10;
	dr.primitive [DI_BOX2] = GL_TRIANGLES;
	
	
	vu32_ladder (dr.n, dr.offset, dr.capacity);
	
	
	GLuint vao [1];
	GLuint tex [2];
	glGenVertexArrays (1, vao);
	glGenTextures (2, tex);
	glBindVertexArray (vao [0]);
	gpu_setup_vertex1 (dr.vbo, xxgl_drawrange_cap (&dr));
	
	struct gtext_fdim fdim;
	fdim.n = 128;
	gtext_fdim_calloc (&fdim);
	
	gtext_setup (tex [0], face, &fdim);
	vf32_mus (128, fdim.x, fdim.x, 0.0011f);
	vf32_mus (128, fdim.y, fdim.y, 0.0011f);
	vf32_mus (128, fdim.w, fdim.w, 0.0011f);
	vf32_mus (128, fdim.h, fdim.h, 0.0011f);
	vf32_mus (128, fdim.a, fdim.a, 0.0011f);
	
	xxgl_dr_v4f32_repeat4           (&dr, VBO_COL, DI_TEXT, 1.0f, 0.4f, 0.4f, 1.0f);
	xxgl_dr_v4f32_randomcolor       (&dr, VBO_COL, DI_BOX1, 10 * 10);
	xxgl_dr_v4f32_grid              (&dr, VBO_POS, DI_BOX1, 10, 10);
	xxgl_dr_v4f32_squaretex_countup (&dr, VBO_TEX, DI_BOX1, 10 * 10, (uint32_t)'!');
	
	
	
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
					//update_tex_random (dr.vbo [2], dr.offset [0], dr.length + 0, dr.capacity [0], 10 * 10);
					break;
					case SDLK_v:
					//update_col_random (dr.vbo [1], dr.offset [0], dr.length + 0, dr.capacity [0], 10 * 10);
					break;
					case SDLK_z:
					vf32_add1 (128, fdim.y, fdim.y, -0.01f);
					break;
				}
				break;
			}
		}
		
		
		
		glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);
		
		gtext_draw ("Goggle", &fdim, &dr, DI_TEXT, 0.0f, 0.0f, 1.0, 2.0f);
		xxgl_drawrange_draw (&dr, DI_BOX1);
		xxgl_drawrange_draw (&dr, DI_TEXT);
		
		SDL_GL_SwapWindow (window);	
		SDL_Delay (1);
	}
	
	
	SDL_GL_DeleteContext (glcontext);
	
	return 0;
}

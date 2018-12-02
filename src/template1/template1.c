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
#include "gen.h"

#include "app.h"
#include "color.h"


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



void RenderText 
(
	GLuint vbo [3],
	char const * text, 
	float x, 
	float y, 
	uint32_t ca [],
	float const cx [],
	float const cy [],
	float const cw [],
	float const ch [],
	float scale,
	uint32_t offset, 
	uint32_t length
)
{
	float * v;
	glBindBuffer (GL_ARRAY_BUFFER, vbo [0]);
	v = glMapBufferRange_4fv (offset, length);
	char const * c;
	c = text;
	while (*c)
	{
		uint8_t i = *c;
		float xx = x + cx [i] * scale;
		float yy = y - (ch [i] - cy [i]) * scale;
		float w = cw [i] * scale;
		float h = ch [i] * scale;
		TRACE_F ("x %f", (double)xx);
		gen_square_pos (v, xx, yy, w, h);
		v += 24;
		x += (ca [i] >> 6) * scale;
		c++;
	}
	glUnmapBuffer (GL_ARRAY_BUFFER);
	
	
	glBindBuffer (GL_ARRAY_BUFFER, vbo [1]);
	v = glMapBufferRange_4fv (offset, length);
	srand (1);
	v4f32_repeat_random (length, v);
	v4f32_repeat_channel (length, v, 3, 1.0f);
	glUnmapBuffer (GL_ARRAY_BUFFER);
	
	
	glBindBuffer (GL_ARRAY_BUFFER, vbo [2]);
	v = glMapBufferRange_4fv (offset, length);
	c = text;
	while (*c)
	{
		uint8_t i = *c;
		gen4x6_square_tex1 (v, cw [i] / 50.0f, ch [i] / 50.0f, i);
		v += 24;
		c++;
	}
	//gen4x6_square_tex (strlen (text), v, (uint8_t const *)text);
	glUnmapBuffer (GL_ARRAY_BUFFER);
	
	
	glDrawArrays (GL_TRIANGLES, offset, length);
	//glDrawArrays (GL_LINES, offset, length);
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
	
	
	struct drawrange dr;
	dr.n = 10;
	drawrange_init (&dr);
	vu32_setl (dr.capacity, dr.n, 6*10*10, 6*7, 0, 0, 0, 0, 0, 0, 0, 0);
	vu32_ladder (dr.n, dr.offset, dr.capacity);
	
	GLuint vao [1];
	GLuint vbo [3];
	GLuint tex [2];
	glGenVertexArrays (1, vao);
	glGenBuffers (3, vbo);
	glGenTextures (2, tex);
	glBindVertexArray (vao [0]);
	gpu_setup_vertex1 (vbo, drawrange_cap (&dr));
	uint32_t ca [128];
	float cw [128];
	float ch [128];
	float cx [128];
	float cy [128];
	setup_font (tex [0], face, ca, cx, cy, cw, ch);
	
	update_pos_grid   (vbo [0], dr.offset [0], dr.length + 0, dr.capacity [0], 10, 10);
	update_col_random (vbo [1], dr.offset [0], dr.length + 0, dr.capacity [0], 10 * 10);
	update_tex_alpha  (vbo [2], dr.offset [0], dr.length + 0, dr.capacity [0], 10 * 10);



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
					update_tex_random  (vbo [2], dr.offset [0], dr.length + 0, dr.capacity [0], 10 * 10);
					break;
					case SDLK_v:
					update_col_random (vbo [1], dr.offset [0], dr.length + 0, dr.capacity [0], 10 * 10);
					break;
				}
				break;
			}
		}
		
		
		
		glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);
		
		RenderText (vbo, "abcdefg", 0.0f, 0.0f, ca, cx, cy, cw, ch, 0.005f, dr.offset [1], dr.capacity [1]);
		
		//RenderBox (vbo, 0.0f, 0.0f, 0.2f, 0.2f, dr.offset [1], dr.capacity [1]);
		
		//app_draw (dr.n, dr.offset, dr.capacity);
		//glDrawArrays (GL_TRIANGLES, 0, 5*6);
		//glDrawArrays (GL_POINTS, 0, 12);
		//glDrawArrays (GL_LINES, 0, 6*100);

		
		SDL_GL_SwapWindow (window);	
		SDL_Delay (1);
	}
	
	
	SDL_GL_DeleteContext (glcontext);
	
	return 0;
}












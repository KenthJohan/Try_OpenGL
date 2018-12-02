#pragma once

#include "SDLGL.h"
#include "debug.h"
#include "debug_gl.h"
#include "glwrap.h"


#include <stdio.h>
#include <unistd.h>


#define APP_WINDOW_WIDTH 1024
#define APP_WINDOW_HEIGHT 768
#define APP_TITLE "Video and plot"

#define APP_QUIT 1U << 0
#define APP_ERROR 1U << 1
#define APP_VIDEO_END 1U << 2


void app_init (uint32_t * flags)
{
	int r = SDL_Init (SDL_INIT_VIDEO);
	ASSERT_F (r == 0, "SDL_Init %i\n", SDL_GetError ());
}


void app_set_icon (SDL_Window * window)
{
	uint16_t pixels [16*16] = 
	{
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
		0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
		0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
		0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
		0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
		0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
		0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
		0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
		0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
		0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
		0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
		0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
		0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
		0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
		0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
		0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
		0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
		0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
		0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
		0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
		0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
		0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
	};
	SDL_Surface * surface = SDL_CreateRGBSurfaceFrom (pixels,16,16,16,16*2,0x0f00,0x00f0,0x000f,0xf000);
	ASSERT (surface);
	SDL_SetWindowIcon (window, surface);
	SDL_FreeSurface (surface);
}


SDL_Window * app_create_window ()
{
	SDL_Window * window = NULL;
	window = SDL_CreateWindow 
	(
		APP_TITLE, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		APP_WINDOW_WIDTH, 
		APP_WINDOW_HEIGHT, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ASSERT_F (window != NULL, "SDL_CreateWindow: %s", SDL_GetError());
	app_set_icon (window);
	//SDL_SetWindowBordered (window, SDL_FALSE);
	//SDL_SetWindowOpacity (window, 0.5);
	return window;
}



void gen4x6_square_pos (float v [24], float x, float y, float w, float h)
{
	float v0 [] =
	{
		x,     y + h,   0.0f, 0.0f,
		x,     y,       0.0f, 0.0f,
		x + w, y,       0.0f, 0.0f,
		x,     y + h,   0.0f, 0.0f,
		x + w, y,       0.0f, 0.0f,
		x + w, y + h,   0.0f, 0.0f       
	};
	memcpy (v, v0, sizeof (v0));
}


void gen4x6_square_tex1 (float v [24], float w, float h, float l)
{
	float v0 [24] =
	{
		0.0f, 0.0f, l, 0.0f,
		0.0f, h   , l, 0.0f,
		w   , h   , l, 0.0f,
		0.0f, 0.0f, l, 0.0f,
		w   , h   , l, 0.0f,
		w   , 0.0f, l, 0.0f       
	};
	memcpy (v, v0, sizeof (v0));
}


void gen4x6_square_tex (uint32_t n, float v [24], uint8_t const l [])
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		gen4x6_square_tex1 (v, 0.0f, 0.0f, (float)l [i]);
		v += 24;
	}
}


void gen4x6_grid_pos 
(
	float v [],
	uint32_t w,
	uint32_t h
)
{
	float dx = 2.0f / (float) w;
	float dy = 2.0f / (float) h;
	for (uint32_t y = 0; y < w; ++y)
	for (uint32_t x = 0; x < h; ++x)
	{
		float xx = (float)x * dx - 1.0f;
		float yy = (float)y * dy - 1.0f;
		gen4x6_square_pos (v, xx, yy, dx, dy);
		v += 24;
	}
}


void gpu_setup_vertex1 (GLuint vbo [3], uint32_t vn)
{
	GLenum const target [] = {GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER};
	uint32_t const index [] = {0, 1, 2};
	GLenum const type [] = {GL_FLOAT, GL_FLOAT, GL_FLOAT};
	GLboolean const normalized [] = {GL_FALSE, GL_FALSE, GL_FALSE};
	uint32_t const offset [] = {0, 0, 0};
	uint32_t const dim [] = {4, 4, 4};
	uint32_t const stride [] = {sizeof (float) * dim [0], sizeof (float) * dim [1], sizeof (float) * dim [2]};
	GLbitfield const flags [] = {GL_MAP_WRITE_BIT, GL_MAP_WRITE_BIT, GL_MAP_WRITE_BIT};
	
	xxgl_layout 
	(
		3,
		index,
		dim,
		type,
		normalized,
		stride,
		offset,
		target,
		vbo
	);
	
	xxgl_allocate 
	(
		3,
		target,
		vbo,
		stride,
		NULL,
		flags,
		vn
	);
}


//first: Specifies the starting index in the enabled arrays.
//count: Specifies the number of indices to be rendered.
void app_draw (uint32_t n, uint32_t first [], uint32_t count [])
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		glDrawArrays (GL_TRIANGLES, (GLint)first [i], (GLsizei)count [i]);
		//glDrawArrays (GL_LINES, (GLint)first [i], (GLsizei)count [i]);
	}
}


void setup_test_texture ()
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
	glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


struct drawrange
{
	uint32_t n;
	uint32_t * offset;
	uint32_t * length;
	uint32_t * capacity;
};


void drawrange_init (struct drawrange * dr)
{
	dr->offset = calloc (dr->n, sizeof (uint32_t));
	dr->length = calloc (dr->n, sizeof (uint32_t));
	dr->capacity = calloc (dr->n, sizeof (uint32_t));
	ASSERT (dr->offset);
	ASSERT (dr->length);
	ASSERT (dr->capacity);
}


uint32_t drawrange_cap (struct drawrange * dr)
{
	return dr->offset [dr->n-1] + dr->capacity [dr->n-1];
}


void setup_font 
(
	GLuint tex, 
	FT_Face face, 
	uint32_t a [],
	float x [],
	float y [],
	float w [],
	float h []
)
{
	GLenum const target = GL_TEXTURE_2D_ARRAY;
	GLsizei const width = 50;
	GLsizei const height = 50;
	GLsizei const layerCount = 128;
	GLsizei const mipLevelCount = 1;
	GLenum const internalformat = GL_R8;
	glBindTexture (target, tex);
	glTexStorage3D (target, mipLevelCount, internalformat, width, height, layerCount);
	GL_CHECK_ERROR;
	
	//IMPORTANT. Disable byte-alignment restriction.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	
	for (uint8_t c = 0; c < 128; c++)
	{
		{
			int r = FT_Load_Char (face, c, FT_LOAD_RENDER);
			ASSERT_F (r == 0, "ERROR::FREETYTPE: Failed to load Glyph %i", r);
			if (r) {continue;}
		}
		
		a [c] = face->glyph->advance.x;
		x [c] = face->glyph->bitmap_left;
		y [c] = face->glyph->bitmap_top;
		w [c] = face->glyph->bitmap.width;
		h [c] = face->glyph->bitmap.rows;
		
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
	
	glTexParameteri (target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri (target, GL_TEXTURE_MAX_LEVEL, 0);
	
	
}


void * app_map_glbuffer 
(
	GLuint vbo,
	uint32_t vfirst, 
	uint32_t vcapacity
)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	uint32_t const vdim = 4;
	uint32_t const vsize = sizeof (float) * vdim;
	GLintptr const offset = vfirst * vsize;
	GLsizeiptr const length = vcapacity * vsize;
	TRACE_F ("offset %i", offset);
	TRACE_F ("length %i", length);
	glBindBuffer (target, vbo);
	void * data = glMapBufferRange (target, offset, length, access);
	GL_CHECK_ERROR;
	return data;
}


void app_unmap_glbuffer (GLuint vbo)
{
	GLenum const target = GL_ARRAY_BUFFER;
	glBindBuffer (target, vbo);
	glUnmapBuffer (target);
	GL_CHECK_ERROR;
}


void update_tex_alpha 
(
	GLuint vbo, 
	uint32_t vfirst, 
	uint32_t * vcount, 
	uint32_t vcapacity,
	uint32_t n
)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	uint32_t const vdim = 4;
	uint32_t const vsize = sizeof (float) * vdim;
	GLintptr const offset8 = vfirst * vsize;
	GLsizeiptr const length8 = vcapacity * vsize;
	TRACE_F ("offset %i", offset8);
	TRACE_F ("length %i", length8);
	glBindBuffer (target, vbo);
	float * v = glMapBufferRange (target, offset8, length8, access);
	uint8_t * layer = malloc (sizeof (uint8_t) * n);
	for (uint32_t i = 0; i < n; ++i)
	{
		layer [i] = '0' + i;
	}
	gen4x6_square_tex (n, v, layer);
	glUnmapBuffer (target);
	free (layer);
}


void update_tex_random 
(
	GLuint vbo, 
	uint32_t vfirst, 
	uint32_t * vcount, 
	uint32_t vcapacity,
	uint32_t n
)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	uint32_t const vdim = 4;
	uint32_t const vsize = sizeof (float) * vdim;
	GLintptr const offset8 = vfirst * vsize;
	GLsizeiptr const length8 = vcapacity * vsize;
	TRACE_F ("offset %i", offset8);
	TRACE_F ("length %i", length8);
	glBindBuffer (target, vbo);
	float * v = glMapBufferRange (target, offset8, length8, access);
	for (uint32_t i = 0; i < n; ++i)
	{
		//gen4x6_square_tex1 (v, (float)(rand () % 256));
		v += 24;
	}
	glUnmapBuffer (target);
}


void update_col_random 
(
	GLuint vbo, 
	uint32_t vfirst, 
	uint32_t * vcount, 
	uint32_t vcapacity,
	uint32_t n
)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	uint32_t const vdim = 4;
	uint32_t const vsize = sizeof (float) * vdim;
	GLintptr const offset8 = vfirst * vsize;
	GLsizeiptr const length8 = vcapacity * vsize;
	TRACE_F ("offset %i", offset8);
	TRACE_F ("length %i", length8);
	glBindBuffer (target, vbo);
	float * v = glMapBufferRange (target, offset8, length8, access);
	uint32_t vi = 0;
	for (uint32_t i = 0; i < n; ++i)
	{
		float r = (float)rand () / (float)RAND_MAX;
		float g = (float)rand () / (float)RAND_MAX;
		float b = (float)rand () / (float)RAND_MAX;
		float a = 1.0f;
		v4f32_repeat4 (6, v + (vdim * vi), r, g, b, a);
		vi += 6;
	}
	(*vcount) = vi;
	glUnmapBuffer (target);
}


void update_pos_grid 
(
	GLuint vbo, 
	uint32_t vfirst, 
	uint32_t * vcount, 
	uint32_t vcapacity,
	uint32_t w,
	uint32_t h
)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLbitfield const access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	uint32_t const vdim = 4;
	uint32_t const vsize = sizeof (float) * vdim;
	GLintptr const offset8 = vfirst * vsize;
	GLsizeiptr const length8 = vcapacity * vsize;
	TRACE_F ("offset %i", offset8);
	TRACE_F ("length %i", length8);
	glBindBuffer (target, vbo);
	float * v = glMapBufferRange (target, offset8, length8, access);
	gen4x6_grid_pos (v, w, h);
	glUnmapBuffer (target);
}


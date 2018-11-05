#pragma once

#include "SDLGL.h"
#include "debug.h"
#include "debug_gl.h"


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
	// Declare an SDL_Surface to be filled in with pixel data from an image file
	SDL_Surface * surface;
	Uint16 pixels[16*16] = 
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
	
	surface = SDL_CreateRGBSurfaceFrom(pixels,16,16,16,16*2,0x0f00,0x00f0,0x000f,0xf000);
	
	// The icon is attached to the window pointer
	SDL_SetWindowIcon(window, surface);
	
	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(surface);
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


void gen_layer (float v [6], float l)
{
	float v0 [] = {l, l, l, l, l, l};
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




void gpu_setup_vertex (GLuint vbo [2], uint32_t n)
{
	GLenum const target = GL_ARRAY_BUFFER;
	GLenum const type = GL_FLOAT;
	GLboolean const normalized = GL_FALSE;
	GLvoid const * pointer = 0;
	GLint const dim0 = 4;
	GLint const dim1 = 1;
	GLsizei const stride0 = sizeof (float) * dim0;
	GLsizei const stride1 = sizeof (float) * dim1;
	GLuint const index0 = 0;
	GLuint const index1 = 1;
	GLsizeiptr const size0 = stride0 * n;
	GLsizeiptr const size1 = stride1 * n;
	GLvoid const * data = NULL;
	GLbitfield const flags = GL_MAP_WRITE_BIT;
	glGenBuffers (2, vbo);
	
	glBindBuffer (target, vbo [0]);
	glBufferStorage(target, size0, data, flags);
	glEnableVertexAttribArray (index0);
	glVertexAttribPointer (index0, dim0, type, normalized, stride0, pointer);
	GL_CHECK_ERROR;
	
	glBindBuffer (target, vbo [1]);
	glBufferStorage(target, size1, data, flags);
	glEnableVertexAttribArray (index1);
	glVertexAttribPointer (index1, dim1, type, normalized, stride1, pointer);
	GL_CHECK_ERROR;
}




void gen_buffer_slots (uint32_t n, uint32_t first [], uint32_t capacity [])
{
	uint32_t f = 0;
	for (uint32_t i = 0; i < n; ++ i)
	{
		first [i] = f;
		f += capacity [i];
	}
}


//first: Specifies the starting index in the enabled arrays.
//count: Specifies the number of indices to be rendered.
void app_draw (uint32_t n, uint32_t first [], uint32_t count [])
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		glDrawArrays (GL_TRIANGLES, (GLint)first [i], (GLsizei)count [i]);
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





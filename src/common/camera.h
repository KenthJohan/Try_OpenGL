#pragma once

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

#include "m4.h"
#include "v4.h"
#include "q.h"


void gl_update_projection (SDL_Window * window, float m [4*4])
{
	int w;
	int h;
	SDL_GetWindowSize (window, &w, &h);
	m4f32_perspective (m, 45.0f, (float)w/(float)h, 0.1f, 100.0f, M_COLMAJ);
	glViewport (0, 0, w, h);
}


void sdl_get_translation_vector (float t [4], uint8_t const * keyboard)
{
	t [0] = (keyboard [SDL_SCANCODE_A] - keyboard [SDL_SCANCODE_D]);
	t [1] = (keyboard [SDL_SCANCODE_LCTRL] - keyboard [SDL_SCANCODE_SPACE]);
	t [2] = (keyboard [SDL_SCANCODE_W] - keyboard [SDL_SCANCODE_S]);
	t [3] = 0;
	v4f32_normalize (t, t);
}


void sdl_get_rotation_vector (float a [4], uint8_t const * keyboard)
{
	a [0] = (keyboard [SDL_SCANCODE_DOWN] - keyboard [SDL_SCANCODE_UP]);
	a [1] = (keyboard [SDL_SCANCODE_RIGHT] - keyboard [SDL_SCANCODE_LEFT]);
	a [2] = (keyboard [SDL_SCANCODE_E] - keyboard [SDL_SCANCODE_Q]);
	a [3] = 0;
	v4f32_normalize (a, a);
}


struct Cam
{
	float p [4]; //Position
	float q [4]; //Looking direction
	float P [16]; //Projection matrix
	float PV [16]; //Projection * View matrix
};


void cam_init (struct Cam * cam, SDL_Window * win)
{
	vf32_set_scalar (0, cam->p, 4);
	m4f32_identity (cam->P);
	qf32_unit (cam->q);
	gl_update_projection (win, cam->P);
}


void cam_update (struct Cam * cam, uint8_t const * keyboard)
{
	float v [4]; //Position velocity.
	float o [4]; //Rotation velocity.
	float u [4]; //Quaternion velocity.
	float R [16]; //Rotation matrix
	float T [16]; //Translation matrix
	float * p = cam->p;
	float * P = cam->P;
	float * PV = cam->PV;
	float * q = cam->q; //Camera rotation quaternion.
	
	//### Rotatate camera from user input
	sdl_get_rotation_vector (o, keyboard);
	qf32_axis_angle (u, o, 0.01f);
	qf32_normalize  (u, u);
	qf32_mul        (u, q, q);
	qf32_normalize  (q, q);
	qf32_m4         (q, R, M_COLMAJ);
	
	
	//### Move camera from user input
	//1. Get the user input (v).
	//2. Set the velocity speed (v).
	//3. Convert velocity vector (v) to rotated (R) coordinate system.
	//4. Move camera position (p) to a new position (p).
	//5. Convert camera position (p) to a translation matrix (T).
	//1. v := userinput;
	//2. v := 0.01 * v
	//3. v := R^T * v
	//4. p := p + v;
	//5. T := convert (t);
	sdl_get_translation_vector (v, keyboard);
	v4f32_mul_scalar  (v, 0.01f, v);
	m4v4f32_mul       (R, v, v, M_COLMAJ | M_TLEFT);
	v4f32_add         (v, p, p);
	m4f32_translation (p, T);
	
	//### Build projection*view matrix.
	m4f32_identity (PV);
	m4f32_mul (T, PV, PV, M_COLMAJ); //Apply translation to view matrix
	m4f32_mul (R, PV, PV, M_COLMAJ); //Apply rotation to view matrix
	m4f32_mul (P, PV, PV, M_COLMAJ); //Apply projection to view matrix
}


void cam_mvp_update (struct Cam * cam, float M [16], GLuint uniform)
{
	float PVM [16]; // ProjecitonMatrix * ViewMatrix * ModelMatrix
	float * PV = cam->PV;
	// Apply model matrix to projection*view matrix
	// PVM := M * PV;
	m4f32_mul (M, PV, PVM, M_COLMAJ);
	m4f32_print (PVM, M_COLMAJ, stdout);
	glUniformMatrix4fv (uniform, 1, GL_FALSE, PVM);
}






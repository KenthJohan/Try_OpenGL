#pragma once

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

#include "mat.h"
#include "m.h"
#include "q.h"
#include "v.h"

struct Camera
{
	float m_mvp [4*4]; //Matrix model view projection (mvp). 
	float m_vp [4*4]; //Matrix view projection (vp). 
	float m_v [4*4]; //Matrix view (v).
	float m_vt [4*4]; //Matrix view translation (vt).
	float m_vr [4*4]; //Matrix view rotation (vr).
	float m_vrx [4*4]; //Matrix view rotation around x (vrx).
	float m_vry [4*4]; //Matrix view rotation around y (vry).
	float m_vrz [4*4]; //Matrix view rotation around z (vrz).
	float m_p [4*4]; //Matrix projection (p).
	
	float ax;
	float ay;
	float az;
	float move_speed;
	float angle_speed;
	
	float q [4];
	float qm [16];
};


void gl_update_projection (SDL_Window * window, float m [4*4])
{
	int w;
	int h;
	SDL_GetWindowSize (window, &w, &h);
	m4f_perspective (m, 45.0f, (float)w/(float)h, 0.01f, 1000.0f);
	glViewport (0, 0, w, h);
}


void camera_update_projection (struct Camera * cam, SDL_Window * window)
{
	gl_update_projection (window, cam->m_p);
}


void camera_init (struct Camera * cam, SDL_Window * window)
{
	M4_IDENTITY (cam->m_mvp);
	M4_IDENTITY (cam->m_vrx);
	M4_IDENTITY (cam->m_vry);
	M4_IDENTITY (cam->m_vrz);
	M4_IDENTITY (cam->m_vt);
	M4_IDENTITY (cam->m_vp);
	//M4_IDENTITY (cam->mm);
	M4_FRUSTUM_INIT (cam->m_p);
	gl_update_projection (window, cam->m_p);
	cam->ax = 0.0f;
	cam->ay = 0.0f;
	cam->az = 0.0f;
	cam->move_speed = 0.1f;
	cam->angle_speed = 0.1f;
	cam->q [0] = 0.0f;
	cam->q [1] = 0.0f;
	cam->q [2] = 0.0f;
	cam->q [3] = 1.0f;
}


void sdl_get_translation_vector (float t [4], uint8_t const * keyboard)
{
	t [0] = (keyboard [SDL_SCANCODE_A] - keyboard [SDL_SCANCODE_D]);
	t [1] = (keyboard [SDL_SCANCODE_LCTRL] - keyboard [SDL_SCANCODE_SPACE]);
	t [2] = (keyboard [SDL_SCANCODE_W] - keyboard [SDL_SCANCODE_S]);
	t [3] = 0;
	v4f_normalize (t, t);
}


void sdl_get_rotation_vector (float a [4], uint8_t const * keyboard)
{
	a [0] = (keyboard [SDL_SCANCODE_DOWN] - keyboard [SDL_SCANCODE_UP]);
	a [1] = (keyboard [SDL_SCANCODE_RIGHT] - keyboard [SDL_SCANCODE_LEFT]);
	a [2] = (keyboard [SDL_SCANCODE_E] - keyboard [SDL_SCANCODE_Q]);
	a [3] = 0;
	v4f_normalize (a, a);
}


void testing_q (struct Camera * cam, float dt [4])
{
	float q [4];
	float m [16];
	qf32_axis_angle (q, dt, 0.1f);
	qf32_normalize (q, q);
	fflush (stdout);
	qf32_mul (q, cam->q, cam->q);
	qf32_normalize (cam->q, cam->q);
	qf32_m4 (cam->q, cam->qm, M_COLMAJ);
	//qf32_print (cam->q, stdout);
	m4_print (cam->qm, M_COLMAJ, stdout);
}


void camera_update (struct Camera * cam, uint8_t const * keyboard)
{
	float dt [4];
	sdl_get_translation_vector (dt, keyboard);
	V4_MUL_SCALAR (dt, dt, cam->move_speed);
	

	
	
	
	float da [4];
	sdl_get_rotation_vector (da, keyboard);
	V4_MUL_SCALAR (da, da, cam->angle_speed);
	testing_q (cam, da); //TESTING
	
	
	//TODO: Change this to quaternion instead.
	cam->ax += da [0];
	cam->ay += da [1];
	cam->az += da [2];
	M4_ROTX (cam->m_vrx, cam->ax);
	M4_ROTY (cam->m_vry, cam->ay);
	M4_ROTZ (cam->m_vrz, cam->az);
	M4_IDENTITY (cam->m_vr);
	m4f_mul (cam->m_vr, cam->m_vry, cam->m_vr);
	m4f_mul (cam->m_vr, cam->m_vrx, cam->m_vr);
	m4f_mul (cam->m_vr, cam->m_vrz, cam->m_vr);
	
	//The translation vector is based on user input coordinate system.
	//Convert the translation vector to the view coordinate system and accumulate the 
	//result in the translation column of the view translation matrix.
	//M4_MAC_TRANSPOSE (cam->m_vt + M4_VT, cam->m_vr, dt);
	M4_MAC_TRANSPOSE (cam->m_vt + M4_VT, cam->qm, dt);
	
	/*
	cam->mt [M4_TX] += t [0] * cam->mr [0];
	cam->mt [M4_TX] += t [1] * cam->mr [1];
	cam->mt [M4_TX] += t [2] * cam->mr [2];
	
	cam->mt [M4_TZ] += t [0] * cam->mr [8];
	cam->mt [M4_TZ] += t [1] * cam->mr [9];
	cam->mt [M4_TZ] += t [2] * cam->mr [10];
	
	cam->mt [M4_TY] += t [0] * cam->mr [4];
	cam->mt [M4_TY] += t [1] * cam->mr [5];
	cam->mt [M4_TY] += t [2] * cam->mr [6];
	*/


	//Combine view matrix and projection matrix.
	M4_IDENTITY (cam->m_vp);
	m4f_mul (cam->m_vp, cam->m_vt, cam->m_vp);
	//m4f_mul (cam->m_vp, cam->m_vr, cam->m_vp);
	m4f_mul (cam->m_vp, cam->qm, cam->m_vp);
	m4f_mul (cam->m_vp, cam->m_p, cam->m_vp);
}


void camera_mvp_update (struct Camera * cam, float mm [16], GLuint uniform_mvp)
{
	//Combine view matrix and model matrix.
	m4f_mul (cam->m_mvp, cam->m_vp, mm);
	glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, cam->m_mvp);
}






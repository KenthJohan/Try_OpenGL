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








struct Cam
{
	float p1 [4]; //Position
	float p [4]; //Position
	float q1 [4]; //
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
	//### Positional components.
	//Position velocity (p1) is used for moving the camera.
	//Position absolute (p) is used for position the camera.
	float * p1 = cam->p1;
	float * p = cam->p;
	
	//### Rotational components.
	//Quaternion velocity (q1) is used for rotate the camera.
	//Quaternion absolute (q) is used for angle the camera.
	float * q1 = cam->q1;
	float * q = cam->q;
	
	//### Transformational components.
	//Translation matrix (T).
	//Rotation matrix (R).
	//Camera projection (P).
	//Camera perspective view (PV).
	float T [16];
	float R [16];
	float * P = cam->P; 
	float * PV = cam->PV;
	
	//### Rotatate camera from user input
	//sdl_get_rotation_vector (q1, keyboard);
	qf32_axis_angle (q1, q1);
	qf32_normalize  (q1, q1);
	qf32_mul        (q1, q, q);
	qf32_normalize  (q, q);
	qf32_m4         (q, R, M_COLMAJ);
	
	//### Move camera from user input
	//2. Set the velocity speed (p1).
	//3. Convert velocity vector (p1) to rotated (R) coordinate system.
	//4. Move camera position (p) to a new position (p).
	//5. Convert camera position (p) to a translation matrix (T).
	//2. v := 0.01 * v
	//3. v := R^T * v
	//4. p := p + v;
	//5. T := convert (t);
	v4f32_normalize   (p1, p1);
	vf32_mul_scalar   (p1, p1 [3], p1, 3);
	m4v4f32_mul       (R, p1, p1, M_COLMAJ | M_TLEFT);
	v4f32_add         (p1, p, p);
	m4f32_translation (p, T);
	
	//### Build PV matrix.
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
	// PVM := PV * M;
	m4f32_mul (PV, M, PVM, M_COLMAJ);
	//m4f32_print (PVM, M_COLMAJ, stdout);
	glUniformMatrix4fv (uniform, 1, GL_FALSE, PVM);
}






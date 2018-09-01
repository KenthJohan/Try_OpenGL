#pragma once

#include <math.h>

#include "m.h"


//Translation vector
//Independent from row/col major 
#define M4_TX 12
#define M4_T0 12
#define M4_TY 13
#define M4_T1 13
#define M4_TZ 14
#define M4_T2 14
#define M4_TW 15
#define M4_T3 15


//Column vectors
//Independent from row/col major 
#define M4_V0 0
#define M4_VX 0
#define M4_V1 4
#define M4_VY 4
#define M4_V2 8
#define M4_VZ 8
#define M4_V3 12
#define M4_VT 12


//Scale scalars
//Independent from row/col major 
#define M4_S0 0
#define M4_S1 5
#define M4_S2 10
#define M4_S3 15


void m4f32_print (float m [16], uint8_t flags, FILE * f)
{
	mf32_print (m, 4, 4, flags, f);
}


void m4f32_set (float c, float m [16])
{
	mf32_set (c, m, 16);
}


void m4f32_translation (float t [4], float m [16])
{
	m4f32_set (0, m);
	m [M4_S0] = 1.0f;
	m [M4_S1] = 1.0f;
	m [M4_S2] = 1.0f;
	m [M4_S3] = 1.0f;
	m [M4_T0 + 0] = t [0]; 
	m [M4_T0 + 1] = t [1]; 
	m [M4_T0 + 2] = t [2];
}


void m4f32_identity (float m [16])
{
	m4f32_set (0, m);
	m [M4_S0] = 1.0f;
	m [M4_S1] = 1.0f;
	m [M4_S2] = 1.0f;
	m [M4_S3] = 1.0f;
}


void m4f32_mul (float a [16], float b [16], float r [16], uint8_t flags)
{
	float t [16];
	mf32_mulcpy (a, b, t, 4, 4, 4, flags);
	memcpy (r, t, sizeof (float) * 16);
}


void m4v4f32_mul (float a [16], float v [4], float r [4], uint8_t flags)
{
	float t [4];
	mf32_mulcpy (a, v, t, 4, 4, 1, flags);
	memcpy (r, t, sizeof (float) * 4);
}


void m4f32_frustum (float m [16], float l, float r, float b, float t, float n, float f, uint8_t flags)
{
	m4f32_set (0, m);
	m [0] = (2 * n) / (r - l);
	m [5] = (2 * n) / (t - b);
	m [8] = (r + l) / (r - l);
	m [9] = (t + b) / (t - b);
	m [10] = (-f - n) / (f - n);
	m [11] = -1;
	m [14] = (-2 * f * n) / (f - n);
}


void m4f32_perspective (float m [16], float fov1, float aspect1, float near1, float far1, uint8_t flags)
{
	float tangent = tan ((M_PI/180.0f) * (fov1 / 2.0f));
	float height = near1 * tangent;
	float width = height * aspect1;
	m4f32_frustum (m, -width, width, -height, height, near1, far1, flags);
}


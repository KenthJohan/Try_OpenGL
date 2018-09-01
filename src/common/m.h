#pragma once

#include <stdio.h>
#include "debug.h"


//Matrix can be stored in column major or row major.
//Column major means that a continuous array is a column vector.
//Row major means that a continuous array is a row vector.
#define M_ROWMAJ 1 << 0
#define M_COLMAJ 1 << 1
#define M_TLEFT 1 << 2
#define M_TRIGHT 1 << 3

size_t m_index (size_t r, size_t c, size_t rn, size_t cn, uint8_t flags)
{
	if (flags & M_ROWMAJ) {return (r * cn) + (1  * c);}
	if (flags & M_COLMAJ) {return (r * 1 ) + (rn * c);}
	ASSERT (1);
	return 0;
}


void mf32_print (float * m, size_t rn, size_t cn, uint8_t flags, FILE * f)
{
	for (size_t r = 0; r < rn; ++ r)
	{
		for (size_t c = 0; c < cn; ++ c)
		{
			fprintf (f, "%f ", m [m_index (r, c, rn, cn, flags)]);
		}
		fprintf (f, "\n");
	}
	fprintf (f, "\n");
	fflush (f);
}


void mf32_set (float c, float * m, size_t n)
{
	for (size_t i = 0; i < n; ++ i)
	{
		m [i] = c;
	}
}


void mf32_sum (float * A, float * B, float * C, size_t r, size_t c, size_t rn, size_t kn, size_t cn, uint8_t flags)
{
	size_t ci = m_index (r, c, rn, cn, flags);
	C [ci] = 0;
	for (size_t k = 0; k < kn; k = k + 1)
	{
		size_t ai;
		size_t bi;
		if (flags & M_TLEFT)  {ai = m_index (k, r, rn, cn, flags);}
		else                  {ai = m_index (r, k, rn, cn, flags);}
		if (flags & M_TRIGHT) {bi = m_index (c, k, rn, cn, flags);}
		else                  {bi = m_index (k, c, rn, cn, flags);}
		//TRACE_F ("%i %i %i", k, A [ai], B [bi]);
		C [ci] += A [ai] * B [bi];
	}
}


void mf32_mulcpy (float * A, float * B, float * C, size_t rn, size_t kn, size_t cn, uint8_t flags)
{
	for (size_t r = 0; r < rn; r = r + 1)
	for (size_t c = 0; c < cn; c = c + 1)
	{
		mf32_sum (A, B, C, r, c, rn, kn, cn, flags);
	}
}




//static void m4f32_perspective (float m [4*4], float fov, float aspect, float near, float far)
//{}

/*
void m4f32_1perspective (float m [16], float fov, float aspect, float near, float far, uint8_t flags)
{
	float tangent = tan ((M_PI/180.0f) * (fov / 2.0f));
	float height = near * tangent;
	float width = height * aspect;
	m4f32_frustum (m, -width, width, -height, height, near, far, flags);
}
*/
/*
#define M4_FRUSTUM_UPDATE(m, l, r, b, t, n, f)\
(m)[0] = (2*(n))/((r)-(l));\
(m)[5] = (2*(n))/((t)-(b));\
(m)[8] = ((r)+(l))/((r)-(l));\
(m)[9] = ((t)+(b))/((t)-(b));\
(m)[10] = (-(f)-(n))/((f)-(n));\
(m)[14] = (-2*(f)*(n))/((f)-(n));\
*/






#pragma once

/*
#define SF32 10
#define SF64 11

#define SS16 20
#define SS32 21

#define SU16 30
#define SU32 31
*/



//#define V4_MAG2(q) (((q) [0] * (q) [0]) + ((q) [1] * (q) [1]) + ((q) [2] * (q) [2]) + ((q) [3] * (q) [3]))



float vf32_dot (float const * a, float const * b, size_t n)
{
	float r = 0;
	for (size_t i = 0; i < n; ++ i)
	{
		r += a [i] * b [i];
	}
	return r;
}


void vf32_add (float const * a, float const * b, float * r, size_t n)
{
	for (size_t i = 0; i < n; ++ i)
	{
		r [i] = a [i] + b [i];
	}
}


void vf32_mul_scalar (float const * a, float const b, float * r, size_t n)
{
	for (size_t i = 0; i < n; ++ i)
	{
		r [i] = a [i] * b;
	}
}


float vf32_norm2 (float const * a, size_t n)
{
	return vf32_dot (a, a, n);
}

float vf32_norm (float const * a, size_t n)
{
	return sqrtf (vf32_norm2 (a, n));
}


void vf32_normalize (float const * a, float * r, size_t n)
{
	float const l = vf32_norm (a, n);
	float const s = l > 0.0f ? 1.0f / l : 0.0f;
	vf32_mul_scalar (a, s, r, n);
}










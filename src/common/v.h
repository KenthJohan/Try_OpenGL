#pragma once


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


void vf32_set_scalar (float const b, float * r, size_t n)
{
	for (size_t i = 0; i < n; ++ i)
	{
		r [i] = b;
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












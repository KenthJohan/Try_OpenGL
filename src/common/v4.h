#pragma once

#include "v.h"


void v4f32_add (float const * a, float const * b, float * r)
{
	vf32_add (a, b, r, 4);
}


void v4f32_mul_scalar (float const * a, float const b, float * r)
{
	vf32_mul_scalar (a, b, r, 4);
}


void v4f32_normalize (float const * a, float * r)
{
	vf32_normalize (a, r, 4);
}


float v4f32_norm2 (float const * a)
{
	return vf32_dot (a, a, 4);
}

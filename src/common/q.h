#pragma once

#include <stdio.h>
#include "v4.h"
#include "m4.h"


void qf32_unit (float q [4])
{
	q [0] = 0.0f;
	q [1] = 0.0f;
	q [2] = 0.0f;
	q [3] = 1.0f;
}


float qf32_norm2 (float q [4])
{
	return v4f32_norm2 (q);
}


float qf32_norm (float const q [4])
{
	return vf32_norm2 (q, 4);
}


void qf32_normalize (float const q [4], float r [4])
{
	v4f32_normalize (q, r);
}


void qf32_axis_angle (float const v [4], float q [4])
{
	float const a = v [3] * 0.5f;
	float const c = cosf (a);
	float const s = sinf (a);
	q [0] = s * v [0];
	q [1] = s * v [1];
	q [2] = s * v [2];
	q [3] = c;
}


void qf32_mul (float p [4], float q [4], float r [4])
{
	(r) [0] = (p) [3] * (q) [0] + (p) [0] * (q) [3] + (p) [1] * (q) [2] - (p) [2] * (q) [1];
	(r) [1] = (p) [3] * (q) [1] - (p) [0] * (q) [2] + (p) [1] * (q) [3] + (p) [2] * (q) [0];
	(r) [2] = (p) [3] * (q) [2] + (p) [0] * (q) [1] - (p) [1] * (q) [0] + (p) [2] * (q) [3];
	(r) [3] = (p) [3] * (q) [3] - (p) [0] * (q) [0] - (p) [1] * (q) [1] - (p) [2] * (q) [2];
}


void qf32_m4 (float const q [4], float m [16], int flags)
{
	float const l = qf32_norm (q);
	float const s = l > 0.0f ? 2.0f / l : 0.0f;

	float const x = q [0];
	float const y = q [1];
	float const z = q [2];
	float const w = q [3];

	float const xx = s * x * x;
	float const xy = s * x * y;
	float const xz = s * x * z;
	float const xw = s * x * w;
	
	float const yy = s * y * y;
	float const yz = s * y * z;
	float const yw = s * y * w;
	
	float const zz = s * z * z;
	float const zw = s * z * w;

	m [0] = 1.0f - yy - zz;
	m [5] = 1.0f - xx - zz;
	m [10] = 1.0f - xx - yy;

	if (flags & M_ROWMAJ)
	{
		m [M4_V1 + 2] = yz - xw;
		m [M4_V2 + 0] = xz - yw;
		m [M4_V0 + 1] = xy - zw;
		m [M4_V2 + 1] = yz + xw;
		m [M4_V0 + 2] = xz + yw;
		m [M4_V1 + 0] = xy + zw;
	}
	
	if (flags & M_COLMAJ)
	{
		m [M4_V1 + 2] = yz + xw;
		m [M4_V2 + 0] = xz + yw;
		m [M4_V0 + 1] = xy + zw;
		m [M4_V2 + 1] = yz - xw;
		m [M4_V0 + 2] = xz - yw;
		m [M4_V1 + 0] = xy - zw;
	}


	m [M4_V0 + 3] = 0.0f;
	m [M4_V1 + 3] = 0.0f;
	m [M4_V2 + 3] = 0.0f;

	m [M4_V3 + 0] = 0.0f;
	m [M4_V3 + 1] = 0.0f;
	m [M4_V3 + 2] = 0.0f;
	
	m [M4_S3] = 1.0f;
}


void qf32_print (float q [4], FILE * f)
{
	for (size_t i = 0; i < 4; ++ i)
	{
		fprintf (f, "%f ", q [i]);
	}
	fprintf (f, "\n");
}




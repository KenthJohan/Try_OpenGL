#pragma once


#include <stdio.h>
#include "v.h"
#include "m.h"



/*
#define Q_MUL(p, q, r)\
{\
	(r) [0] = (p) [3] * (q) [0] + (p) [0] * (q) [3] + (p) [1] * (q) [2] - (p) [2] * (q) [1];\
	(r) [1] = (p) [3] * (q) [1] - (p) [0] * (q) [2] + (p) [1] * (q) [3] + (p) [2] * (q) [0];\
	(r) [2] = (p) [3] * (q) [2] + (p) [0] * (q) [1] - (p) [1] * (q) [0] + (p) [2] * (q) [3];\
	(r) [3] = (p) [3] * (q) [3] - (p) [0] * (q) [0] - (p) [1] * (q) [1] - (p) [2] * (q) [2];\
}


#define Q_TO_M4(type, q, m)\
{\
	type _n = Q4_MAG2 ((q));\
	type _s = (_n > (type)0) ? (2 / (type)_n) : (type)0;\
	type _00 = _s * (q) [0] * (q) [0];\
	type _01 = _s * (q) [0] * (q) [1];\
	type _02 = _s * (q) [0] * (q) [2];\
	type _03 = _s * (q) [0] * (q) [3];\
	type _11 = _s * (q) [1] * (q) [1];\
	type _12 = _s * (q) [1] * (q) [2];\
	type _13 = _s * (q) [1] * (q) [3];\
	type _22 = _s * (q) [2] * (q) [2];\
	type _23 = _s * (q) [2] * (q) [3];\
	(m) [M4_S0] = (type)1 - (_11 + _22);\
	(m) [M4_S1] = (type)1 - (_00 + _22);\
	(m) [M4_S2] = (type)1 - (_00 + _11);\
	(m) [M4_V0 + 1] = _01 + _23;\
	(m) [M4_V1 + 0] = _01 - _23;\
	(m) [M4_V0 + 2] = _02 - _13;\
	(m) [M4_V2 + 0] = _02 + _13;\
	(m) [M4_V1 + 2] = _12 + _03;\
	(m) [M4_V2 + 1] = _12 - _03;\
	(m) [M4_V0 + 3] = 0;\
	(m) [M4_V1 + 3] = 0;\
	(m) [M4_V2 + 3] = 0;\
	(m) [M4_T0] = 0;\
	(m) [M4_T1] = 0;\
	(m) [M4_T2] = 0;\
	(m) [M4_T3] = 0;\
}
*/


float qf32_norm2 (float q [4])
{
	return vf32_norm2 (q, 4);
}


float qf32_norm (float const q [4])
{
	return vf32_norm2 (q, 4);
}


void qf32_normalize (float const q [4], float r [4])
{
	vf32_normalize (q, r, 4);
}


void qf32_axis_angle (float q [4], float const axis [3], float const angle)
{
	float const a = angle * 0.5f;
	float const c = cosf (a);
	float const s = sinf (a);
	q [0] = s * axis [0];
	q [1] = s * axis [1];
	q [2] = s * axis [2];
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













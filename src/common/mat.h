#pragma once
#include <math.h>


// Set vector scalar
// (v := {x | x = s})
#define SET_VS(N, v, s)                                                 \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (s);                                                  \
}

// Set vector vector
// (v := u)
#define SET_VV(N, v, u)                                                 \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i];                                              \
}

// Add vector vector scalar
// (v := u + s)
#define ADD_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] + (s);                                        \
}

// Add vector vector vector
// (v := u + s)
#define ADD_VVV(N, v, u, w)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] + (w)[_i];                                    \
}

// Sub vector vector vector
// (v := u - w)
#define SUB_VVV(N, v, u, w)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] - (w)[_i];                                    \
}

// Sub vector vector scalar
// (v := u - s)
#define SUB_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] - (s);                                        \
}


// Clear vector
// (v := {x | x = 0})
#define CLR_V(N, v)                                                     \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = 0.0;                                                  \
}


#define UNIT_V(N, v, j)                                                 \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (_i == (j) ? 1.0 : 0.0);                              \
}


#define MUL_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] * (s);                                        \
}


#define DIV_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] / (s);                                        \
}


#define DOTP_SVV(N, s, v, u)                                            \
{                                                                       \
    int _i;                                                             \
    (s) = 0.0;                                                          \
    for (_i = 0; _i < N; _i++)                                          \
        (s) += (v)[_i] * (u)[_i];                                       \
}


#define ABS_SV(N, s, v)                                                 \
{                                                                       \
    real _tmp;                                                          \
    int _i;                                                             \
    _tmp = 0.0;                                                         \
    for (_i = 0; _i < (N); _i++)                                        \
        _tmp += (v)[_i] * (v)[_i];                                      \
    (s) = rsqrt(_tmp);                                                  \
}


#define DIST_SVV(N, s, u, v)                                            \
{                                                                       \
    real _tmp;                                                          \
    int _i;                                                             \
    _tmp = 0.0;                                                         \
    for (_i = 0; _i < (N); _i++)                                        \
        _tmp += ((u)[_i]-(v)[_i]) * ((u)[_i]-(v)[_i]);                  \
    (s) = rsqrt(_tmp);                                                  \
}


#define CROSSP_SV2V2(s,v,u)                                             \
{                                                                       \
    (s) = (v)[0]*(u)[1] - (v)[1]*(u)[0];                                \
}


#define CROSSP_SV3V3(v,u,w)                                             \
{                                                                       \
    (v)[0] = (u)[1]*(w)[2] - (u)[2]*(w)[1];                             \
    (v)[1] = (u)[2]*(w)[0] - (u)[0]*(w)[2];                             \
    (v)[2] = (u)[0]*(w)[1] - (u)[1]*(w)[0];                             \
}


#define IDENTITY_M(n1, n2, m)                                           \
{                                                                       \
    for (size_t _i = 0; _i < (n1); _i++)                                \
    for (size_t _j = 0; _j < (n2); _j++)                                \
        (m)[_i*n1 + _j] = (_i == _j ? 1.0 : 0.0);                       \
}


#define VN_HADAMARD_PRODUCT(n, r, a, b)\
{\
    for (size_t _i = 0; _i < (n); _i++)\
        (r)[_i] = (a)[_i] * (b)[_i];\
}

//y := y + M v
#define MN_MAC(w, h, y, m, v)\
{\
	for (size_t _r = 0; _r < h; _r = _r + 1)\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
	{\
		(y) [_r] += (m) [MAJX (w, _r, _c)] * (v) [_c];\
	}\
}

//y := y + M^T v
#define MN_MAC_TRANSPOSE(w, h, y, mt, v)\
{\
	for (size_t _r = 0; _r < h; _r = _r + 1)\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
	{\
		(y) [_r] += (mt) [MAJX (w, _c, _r)] * (v) [_c];\
	}\
}


#define MM_MUL(w, m, a, b)\
{\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
	for (size_t _r = 0; _r < w; _r = _r + 1)\
	{\
		(m) [MAJX (w, _r, _c)] = 0.0f;\
		for(size_t _k = 0; _k < w; _k = _k + 1)\
			(m) [MAJX (w, _r, _c)] += (a) [MAJX (w, _r, _k)] * (b) [MAJX (w, _k, _c)];\
	}\
}



















//Matrix can be stored in column major or row major.
//Column major means that a continuous array is a column vector.
//Row major means that a continuous array is a row vector.
#define MAJR(w, r, c) (((w)*(r)) + (c))
#define MAJC(w, r, c) (((w)*(c)) + (r))
#define MAJX MAJC





#define V4_SUM(v, s)\
{\
	(s) = 0;\
    for (size_t _i = 0; _i < 4; _i++)\
        (s) += (v) [_i]\
}


#define V4_DOT(s, u, v)\
{\
    (s) = 0;\
    for (size_t _i = 0; _i < 4; _i++)\
        (s) += (u)[_i] * (v)[_i];\
}


#define V4_MUL_SCALAR(r, v, s)\
{\
    for (size_t _i = 0; _i < 4; _i++)\
        (r)[_i] = (v)[_i] * (s);\
}


//Translation vector
#define M4_TX 12
#define M4_T0 12
#define M4_TY 13
#define M4_T1 13
#define M4_TZ 14
#define M4_T2 14
#define M4_TW 15
#define M4_T3 15

//Column vectors
#define M4_V0 0
#define M4_VX 0
#define M4_V1 4
#define M4_VY 4
#define M4_V2 8
#define M4_VZ 8
#define M4_V3 12
#define M4_VT 12

#define M4_S0 0
#define M4_S1 5
#define M4_S2 10
#define M4_S3 15




// Clear vector
// (v := {x | x = 0})
#define M4_CLR(m)\
{\
	for (size_t _i = 0; _i < 16; _i++)\
		(m)[_i] = 0.0;\
}


#define M4_IDENTITY(m)\
{\
    for (size_t _i = 0; _i < 4; _i++)\
    for (size_t _j = 0; _j < 4; _j++)\
        (m)[_i*4 + _j] = (_i == _j ? 1.0 : 0.0);\
}


#define M4_PRINT(m, fmt)\
{\
	for (size_t _r = 0; _r < 4; _r++)\
	{\
		for (size_t _c = 0; _c < 4; _c++)\
			printf (fmt, (m) [MAJX(4, _r, _c)]);\
		printf ("\n");\
	}\
}


//M = A*B
#define M4_MUL_CPY(m, a, b)\
{\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
	{\
		(m) [MAJX(4, _r, _c)] = 0.0f;\
		for(size_t _k = 0; _k < 4; _k = _k + 1)\
			(m) [MAJX(4, _r, _c)] += (a) [MAJX(4, _r, _k)] * (b) [MAJX(4, _k, _c)];\
	}\
}


#define M4_MAC(m, a, b)\
{\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
	for (size_t _k = 0; _k < 4; _k = _k + 1)\
		(m) [MAJX(4, _r, _c)] += (a) [MAJX(4, _r, _k)] * (b) [MAJX(4, _k, _c)];\
}


//y := y + M^T v
#define M4_MAC_TRANSPOSE(y, mt, v)\
{\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
	{\
		(y) [_r] += (mt) [MAJX (4, _c, _r)] * (v) [_c];\
	}\
}


#define M4_ROTX(m, a)\
(m)[5] = cos (a);\
(m)[6] = sin (a);\
(m)[9] = -sin (a);\
(m)[10] = cos (a);\

#define M4_ROTY(m, a)\
(m)[0] = cos (a);\
(m)[2] = -sin (a);\
(m)[8] = sin (a);\
(m)[10] = cos (a);\

#define M4_ROTZ(m, a)\
(m)[0] = cos (a);\
(m)[1] = sin (a);\
(m)[4] = -sin (a);\
(m)[5] = cos (a);\


#define V4_SET(v,x,y,z,w)\
{\
(v)[0] = x;\
(v)[1] = y;\
(v)[2] = z;\
(v)[3] = w;\
}\


#define M4_FRUSTUM_UPDATE(m, l, r, b, t, n, f)\
(m)[0] = (2*(n))/((r)-(l));\
(m)[5] = (2*(n))/((t)-(b));\
(m)[8] = ((r)+(l))/((r)-(l));\
(m)[9] = ((t)+(b))/((t)-(b));\
(m)[10] = (-(f)-(n))/((f)-(n));\
(m)[14] = (-2*(f)*(n))/((f)-(n));\


#define M4_FRUSTUM_INIT(m)\
M4_CLR (m);\
(m)[11] = -1;


static void m4f_perspective (float m [4*4], float fov, float aspect, float near, float far)
{
	float tangent = tan ((M_PI/180.0f) * (fov / 2.0f));
	float height = near * tangent;
	float width = height * aspect;
	M4_FRUSTUM_UPDATE (m, -width, width, -height, height, near, far);
}


static void m4f_mul (float m [4*4], float a [4*4], float b [4*4])
{
	float t [4*4];
	M4_MUL_CPY (t, a, b);
	memcpy (m, t, sizeof (t));
}


void v4f_normalize (float r [4], float v [4])
{
	float sum;
	V4_DOT (sum, v, v);
	if (sum == 0) return;
	sum = 1.0f / sqrtf (sum);
	//printf ("s %f\n", sum);
	V4_MUL_SCALAR (r, v, sum);
}





























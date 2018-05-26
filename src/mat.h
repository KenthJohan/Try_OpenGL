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




//Matrix can be stored in column major or row major.
//Column major means that a continuous array is a column vector.
//Row major means that a continuous array is a row vector.
#define MAJR(w, r, c) (((w)*(r)) + (c))
#define MAJC(w, r, c) (((w)*(c)) + (r))
#define MAJX MAJC

#define TX_M4X4 MAJX(4, 0, 3)
#define TY_M4X4 MAJX(4, 1, 3)
#define TZ_M4X4 MAJX(4, 2, 3)


#define PRINT_M4X4(m)\
{\
	for (size_t _r = 0; _r < 4; _r++)\
	{\
		for (size_t _c = 0; _c < 4; _c++)\
			printf ("%2.2f ", m [MAJX(4, _r, _c)]);\
		printf ("\n");\
	}\
}


#define MUL_M(w, m, a, b)\
{\
	for (size_t _c = 0; _c < w; _c = _c + 1)\
	for (size_t _r = 0; _r < w; _r = _r + 1)\
	{\
		m[MAJX (w, _r, _c)] = 0.0f;\
		for(size_t _k = 0; _k < w; _k = _k + 1)\
			m [MAJX (w, _r, _c)] += a [MAJX (w, _r, _k)] * b [MAJX (w, _k, _c)];\
	}\
}


#define MUL_M4(m, a, b)\
{\
	for (size_t _c = 0; _c < 4; _c = _c + 1)\
	for (size_t _r = 0; _r < 4; _r = _r + 1)\
	{\
		m[MAJX(4, _r, _c)] = 0.0f;\
		for(size_t _k = 0; _k < 4; _k = _k + 1)\
			m [MAJX(4, _r, _c)] += a [MAJX(4, _r, _k)] * b [MAJX(4, _k, _c)];\
	}\
}



#define ROTX_M4X4(m, a)\
(m)[5] = cos (a);\
(m)[6] = sin (a);\
(m)[9] = -sin (a);\
(m)[10] = cos (a);\

#define ROTY_M4X4(m, a)\
(m)[0] = cos (a);\
(m)[2] = -sin (a);\
(m)[8] = sin (a);\
(m)[10] = cos (a);\

#define ROTZ_M4X4(m, a)\
(m)[0] = cos (a);\
(m)[1] = sin (a);\
(m)[4] = -sin (a);\
(m)[5] = cos (a);\


#define SET_V4(v,x,y,z,w)\
{\
(v)[0] = x;\
(v)[1] = y;\
(v)[2] = z;\
(v)[3] = w;\
}\


#define FRUSTUM_M4X4(m, l, r, b, t, n, f)\
(m)[0] = (2*(n))/((r)-(l));\
(m)[5] = (2*(n))/((t)-(b));\
(m)[8] = ((r)+(l))/((r)-(l));\
(m)[9] = ((t)+(b))/((t)-(b));\
(m)[10] = (-(f)-(n))/((f)-(n));\
(m)[11] = -1;\
(m)[14] = (-2*(f)*(n))/((f)-(n));\


static void perspective_m4x4 (float m [4*4], float fov, float aspect, float near, float far)
{
	float tangent = tan ((M_PI/180.0f) * (fov / 2.0f));
	float height = near * tangent;
	float width = height * aspect;
	FRUSTUM_M4X4 (m, -width, width, -height, height, near, far);
}


static void mul_m4 (float m [4*4], float a [4*4], float b [4*4])
{
	float t [4*4];
	MUL_M4 (t, a, b);
	memcpy (m, t, sizeof (t));
}





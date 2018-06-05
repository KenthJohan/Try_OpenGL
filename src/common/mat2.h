#pragma once
#include <math.h>

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define COUNT_ELEMENT(x) (sizeof(x) / sizeof(*(x)))


#define MAT_2D(rp, cp, a, r, c) (a)[((r)*(rp) + (c)*(cp))]
#define MAT_4X4_ROWM 4, 1, 4, 4
#define MAT_4X4_COLM 1, 4, 4, 4

//#define MAJR(w, r, c) (((w)*(r)) + (1)*(c))
//#define MAJC(w, r, c) (((1)*(r)) + (w)*(c))
//#define MAJX MAJC

#define MAT_MUL7(rp, cp, w, h, y, a, b)\
{\
	for (size_t c = 0; c < w; c = c + 1)\
	for (size_t r = 0; r < w; r = r + 1)\
	{\
		MAT_2D (rp, cp, y, r, c) = 0;\
		for(size_t k = 0; k < w; k = k + 1)\
			MAT_2D (rp, cp, y, r, c) += MAT_2D (rp, cp, a, r, k) * MAT_2D (rp, cp, b, k, c);\
	}\
}

#define MAT_MUL4(rpcpwh, y, a, b) MAT_MUL7 (rpcpwh, y, a, b);

#define MAT_PRINT6(rp, cp, w, h, m, fmt)\
{\
	for (size_t r = 0; r < h; r = r + 1)\
	{\
		for (size_t c = 0; c < w; c = c + 1)\
			printf (fmt, MAT_2D (rp, cp, m, r, c));\
		printf ("\n");\
	}\
}

#define MAT_PRINT3(rpcpwh, m, fmt) MAT_PRINT6 (rpcpwh, m, fmt);

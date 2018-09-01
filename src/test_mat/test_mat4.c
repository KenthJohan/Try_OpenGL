#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "tcol.h"
#include "debug.h"


//Matrix can be stored in column major or row major.
//Column major means that a continuous array is a column vector.
//Row major means that a continuous array is a row vector.
#define MAT_RMAJ 1 << 0
#define MAT_CMAJ 1 << 1
#define M_TRANSPOSE_LEFT 1 << 2
#define M_TRANSPOSE_RIGHT 1 << 3
size_t m_index (size_t r, size_t c, size_t rn, size_t cn, uint8_t flags)
{
	if (flags & MAT_RMAJ) {return (r * cn) + (1  * c);}
	if (flags & MAT_CMAJ) {return (r * 1 ) + (rn * c);}
	return 0;
}


void sum (int * A, int * B, int * C, size_t r, size_t c, size_t rn, size_t kn, size_t cn, size_t flags)
{
	size_t ci = m_index (r, c, rn, cn, flags);
	C [ci] = 0;
	for (size_t k = 0; k < kn; k = k + 1)
	{
		size_t ai;
		size_t bi;
		if (flags & M_TRANSPOSE_LEFT)  {ai = m_index (k, r, rn, cn, flags);}
		else                           {ai = m_index (r, k, rn, cn, flags);}
		if (flags & M_TRANSPOSE_RIGHT) {bi = m_index (c, k, rn, cn, flags);}
		else                           {bi = m_index (k, c, rn, cn, flags);}
		//TRACE_F ("%i %i %i", k, A [ai], B [bi]);
		C [ci] += A [ai] * B [bi];
	}
}


void mul (int * A, int * B, int * C, size_t rn, size_t kn, size_t cn, size_t flags)
{
	for (size_t r = 0; r < rn; r = r + 1)
	for (size_t c = 0; c < cn; c = c + 1)
	{
		sum (A, B, C, r, c, rn, kn, cn, flags);
	}
}


void mat_print (int * A, size_t rn, size_t cn, uint8_t flags)
{
	for (size_t r = 0; r < rn; r = r + 1)
	{
		for (size_t c = 0; c < cn; c = c + 1)
		{
			size_t i = m_index (r, c, rn, cn, flags); 
			printf ("%2i ", A [i]);
		}
		printf ("\n");
	}
	printf ("\n");
}


int main (int argc, char *argv[])
{
	/*
	int A [2 * 2] = 
	{
		1, 2, 
		3, 4
	};
	int B [2 * 4] = 
	{
		1, 1, 
		2, 2, 
		3, 3, 
		3, 3
	};
	//mat_print (A, 2, 2, MAT_RMAJ);
	mat_print (A, 2, 2, MAT_CMAJ);
	
	//mat_print (B, 4, 2, MAT_RMAJ);
	mat_print (B, 2, 4, MAT_CMAJ);
	
	
	int C [2 * 4] = {0};
	//sum (A, B, C, 0, 3, 2, 2, 4, MAT_CMAJ);
	//sum (A, B, C, 1, 3, 2, 2, 4, MAT_CMAJ);
	mul (A, B, C, 2, 2, 4, MAT_CMAJ);
	mat_print (C, 2, 4, MAT_CMAJ);
	*/
	
	int A [4 * 4] = 
	{
		1, 0, 0, 0,
		0, 0, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 1
	};
	
	int B [4] = {1,2,3,4};
	
	int R [4] = {0,0,0,0};
	
	mul (A, B, R, 4, 4, 1, MAT_CMAJ | M_TRANSPOSE_LEFT);
	
	mat_print (A, 4, 4, MAT_CMAJ);
	mat_print (B, 4, 1, MAT_CMAJ);
	mat_print (R, 4, 1, MAT_CMAJ);

	
	
	return 0;
}

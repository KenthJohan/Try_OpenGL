/*
https://math.tutorvista.com/algebra/matrix-multiplication.html
*/

#include <stdio.h>
#include <unistd.h>

#include "debug.h"
#include "mat.h"
#include "ANSIC.h"

#define FMT_INT ANSIC (ANSIC_NORMAL, ANSIC_YELLOW, ANSIC_DEFAULT) "%02i " ANSIC_RESET
#define FMT_FLT ANSIC (ANSIC_NORMAL, ANSIC_CYAN, ANSIC_DEFAULT) "%04.1f " ANSIC_RESET

int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	printf ("\n\n");
	
	{
		printf ("Integer matrix multiplication\n");
		int m1 [4*4];
		int m2 [4*4];
		int r [4*4];
		memcpy (m1 + 0, (int []){4, 0, 3, 2}, sizeof (int) * 4);
		memcpy (m1 + 4, (int []){5, 3, 5, 4}, sizeof (int) * 4);
		memcpy (m1 + 8, (int []){1, 6, 0, 6}, sizeof (int) * 4);
		memcpy (m1 + 12, (int []){8, 1, 9, 1}, sizeof (int) * 4);
		PRINT_M4 (m1, FMT_INT);
		printf ("*\n");
		memcpy (m2 + 0, (int []){1, 0, 3, 2}, sizeof (int) * 4);
		memcpy (m2 + 4, (int []){5, 3, 5, 0}, sizeof (int) * 4);
		memcpy (m2 + 8, (int []){1, 6, 7, 6}, sizeof (int) * 4);
		memcpy (m2 + 12, (int []){0, 1, 2, 1}, sizeof (int) * 4);
		PRINT_M4 (m2, FMT_INT);
		printf ("=\n");
		MUL_M (4, r, m1, m2);
		PRINT_M4 (r, FMT_INT);
		printf ("\n");
	}
	
	{
		printf ("Real matrix multiplication\n");
		float m1 [4*4];
		float m2 [4*4];
		float r [4*4];
		memcpy (m1 + 0, (float []){4.0f, 0.0f, 3.0f, 2.0f}, sizeof (float) * 4);
		memcpy (m1 + 4, (float []){5.0f, 3.0f, 5.0f, 4.0f}, sizeof (float) * 4);
		memcpy (m1 + 8, (float []){1.0f, 6.0f, 0.0f, 6.0f}, sizeof (float) * 4);
		memcpy (m1 + 12, (float []){8.0f, 1.0f, 9.0f, 1.0f}, sizeof (float) * 4);
		PRINT_M4 (m1, FMT_FLT);
		printf ("*\n");
		memcpy (m2 + 0, (float []){1.0f, 0.0f, 3.0f, 2.0f}, sizeof (float) * 4);
		memcpy (m2 + 4, (float []){5.0f, 3.0f, 5.0f, 0.0f}, sizeof (float) * 4);
		memcpy (m2 + 8, (float []){1.0f, 6.0f, 7.0f, 6.0f}, sizeof (float) * 4);
		memcpy (m2 + 12, (float []){0.0f, 1.0f, 2.0f, 1.0f}, sizeof (float) * 4);
		PRINT_M4 (m2, FMT_FLT);
		printf ("=\n");
		MUL_M (4, r, m1, m2);
		PRINT_M4 (r, FMT_FLT);
		printf ("\n");
	}
	
	

	
	return 0;
}
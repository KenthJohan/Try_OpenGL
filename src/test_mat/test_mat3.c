#include <stdio.h>
#include <unistd.h>

#include "ANSIC.h"
#include "debug.h"
#include "mat2.h"

#define FMT_INT ANSIC (ANSIC_NORMAL, ANSIC_YELLOW, ANSIC_DEFAULT) "%02i " ANSIC_RESET
#define FMT_FLT ANSIC (ANSIC_NORMAL, ANSIC_CYAN, ANSIC_DEFAULT) "%04.1f " ANSIC_RESET
 
int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	
	{
		printf ("Integer matrix multiplication\n");
		int a [4*4];
		int b [4*4];
		int y [4*4];
		memcpy (a + 0, (int []){4, 0, 3, 2}, sizeof (int) * 4);
		memcpy (a + 4, (int []){5, 3, 5, 4}, sizeof (int) * 4);
		memcpy (a + 8, (int []){1, 6, 0, 6}, sizeof (int) * 4);
		memcpy (a + 12, (int []){8, 1, 9, 1}, sizeof (int) * 4);
		memcpy (b + 0, (int []){1, 0, 3, 2}, sizeof (int) * 4);
		memcpy (b + 4, (int []){5, 3, 5, 0}, sizeof (int) * 4);
		memcpy (b + 8, (int []){1, 6, 7, 6}, sizeof (int) * 4);
		memcpy (b + 12, (int []){0, 1, 2, 1}, sizeof (int) * 4);
		MAT_PRINT3 (MAT_4X4_COLM, a, FMT_INT);
		printf ("*\n");
		MAT_PRINT3 (MAT_4X4_COLM, b, FMT_INT);
		printf ("=\n");
		MAT_MUL4 (MAT_4X4_COLM, y, a, b);
		MAT_PRINT3 (MAT_4X4_COLM, y, FMT_INT);
		printf ("\n");
	}
	
	{
		printf ("Real matrix multiplication\n");
		float a [4*4];
		float b [4*4];
		float y [4*4];
		memcpy (a + 0, (float []){4.0f, 0.0f, 3.0f, 2.0f}, sizeof (float) * 4);
		memcpy (a + 4, (float []){5.0f, 3.0f, 5.0f, 4.0f}, sizeof (float) * 4);
		memcpy (a + 8, (float []){1.0f, 6.0f, 0.0f, 6.0f}, sizeof (float) * 4);
		memcpy (a + 12, (float []){8.0f, 1.0f, 9.0f, 1.0f}, sizeof (float) * 4);
		memcpy (b + 0, (float []){1.0f, 0.0f, 3.0f, 2.0f}, sizeof (float) * 4);
		memcpy (b + 4, (float []){5.0f, 3.0f, 5.0f, 0.0f}, sizeof (float) * 4);
		memcpy (b + 8, (float []){1.0f, 6.0f, 7.0f, 6.0f}, sizeof (float) * 4);
		memcpy (b + 12, (float []){0.0f, 1.0f, 2.0f, 1.0f}, sizeof (float) * 4);
		MAT_PRINT3 (MAT_4X4_COLM, a, FMT_FLT);
		printf ("*\n");
		MAT_PRINT3 (MAT_4X4_COLM, b, FMT_FLT);
		printf ("=\n");
		MAT_MUL4 (MAT_4X4_COLM, y, a, b);
		MAT_PRINT3 (MAT_4X4_COLM, y, FMT_FLT);
		printf ("\n");
	}
	
	
	return 0;
}

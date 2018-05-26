
#include <stdio.h>
#include <unistd.h>

#include "debug.h"
#include "mat.h"

 
int main (int argc, char *argv[])
{
	fprintf (stderr, "main\n");
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	
	{
	//https://math.tutorvista.com/algebra/matrix-multiplication.html
	float m1 [4*4];
	float m2 [4*4];
	float r [4*4];
	//IDENTITY_M (4, 4, m1);
	//IDENTITY_M (4, 4, m2);
	memcpy (m1 + 0, (float []){4.0f, 0.0f, 3.0f, 2.0f}, sizeof (float) * 4);
	memcpy (m1 + 4, (float []){5.0f, 3.0f, 5.0f, 4.0f}, sizeof (float) * 4);
	memcpy (m1 + 8, (float []){1.0f, 6.0f, 0.0f, 6.0f}, sizeof (float) * 4);
	memcpy (m1 + 12, (float []){8.0f, 1.0f, 9.0f, 1.0f}, sizeof (float) * 4);
	PRINT_M4X4 (m1);
	printf ("\n");
	memcpy (m2 + 0, (float []){1.0f, 0.0f, 3.0f, 2.0f}, sizeof (float) * 4);
	memcpy (m2 + 4, (float []){5.0f, 3.0f, 5.0f, 0.0f}, sizeof (float) * 4);
	memcpy (m2 + 8, (float []){1.0f, 6.0f, 7.0f, 6.0f}, sizeof (float) * 4);
	memcpy (m2 + 12, (float []){0.0f, 1.0f, 2.0f, 1.0f}, sizeof (float) * 4);
	PRINT_M4X4 (m2);
	printf ("\n");
	MUL_M (4, r, m1, m2);
	PRINT_M4X4 (r);
	printf ("\n#2");
	}
	
	return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "misc.h"
#include "debug.h"


void vec_print (FILE * f, int * x, size_t n)
{
	for (size_t i = 0; i < n; i = i + 1)
	{
		fprintf (f, "%02i ", x [i]);
	}
}

void sort (int * x, int * y, size_t n)
{
	for (size_t i = 0; i < n; i = i + 1)
	{
		y [x [i]] = x [i];
	}
}


int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	
	
	int x [] = {4, 3, 6, 1, 9, 2};
	int y [100] = {0};
	vec_print (stdout, x, COUNTOF (x));
	printf ("\n");
	sort (x, y, COUNTOF (x));
	vec_print (stdout, y, COUNTOF (y));
	printf ("\n");
	
	
	
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"
#include "mat.h"
#include "ANSIC.h"
#include "vertex.h"


void scantron (FILE * file, struct Vertex * v, int * len)
{
	int i = 0;
	while (1)
	{
		if (i == *len) {break;}
		int r;
		r = fscanf 
		(
			file, 
			"%f%f%f%f%f%f%f%f", 
			&(v[i].pos [0]), &(v[i].pos [1]), &(v[i].pos [2]), &(v[i].pos [3]),
			&(v[i].col [0]), &(v[i].col [1]), &(v[i].col [2]), &(v[i].col [3])
		);
		if (r != 8) {break;}
		printf ("%i: %f %f %f %f %f %f %f %f\n", r, v[i].pos [0], v[i].pos [1], v[i].pos [2], v[i].pos [3], v[i].col [0], v[i].col [1], v[i].col [2], v[i].col [3]);
		i ++;
	}
	*len = i;
}

int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	printf ("\n\n");

	//FILE * file = fopen ("verts.txt", "r");
	FILE * file = stdin;
	ASSERT_F (file != NULL, "%s", "");
	
	
	struct Vertex v [10];
	int len = COUNTE (v);
	scantron (file, v, &len);
	printf ("len : %i\n", len);
	
	fclose (file);
	

	
	return 0;
}

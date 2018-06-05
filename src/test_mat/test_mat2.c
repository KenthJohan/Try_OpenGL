#include <stdio.h>
#include <unistd.h>

#include "debug.h"
#include "mat.h"

 
int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	return 0;
}
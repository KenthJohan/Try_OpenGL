#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"
#include "ANSIC.h"
#include "misc.h"



void test (int expect, char const * haystack, size_t len, char const * needles)
{
	printf ("%10s %3i %10s : ", haystack, (int)len, needles);
	int r = str_contain (haystack, len, needles);
	char const * c [2];
	char const * t [2];
	c [0] = ANSIC (ANSIC_BOLD, ANSIC_RED, ANSIC_DEFAULT);
	c [1] = ANSIC (ANSIC_BOLD, ANSIC_GREEN, ANSIC_DEFAULT);
	t [0] = "FAIL";
	t [1] = "PASS";
	printf ("%s[%s]%s\n", c [r == expect], t [r == expect], ANSIC_RESET);
}

int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");

	test (0, "", 0, "");
	test (0, "", 1, "");
	test (1, "a", 1, "a");
	test (0, "a", 1, "b");
	
	test (0, "abc", 0, "b");
	test (0, "abc", 1, "b");
	test (1, "abc", 2, "b");
	
	test (0, "abc:", 0, ":x");
	test (0, "abc:", 1, ":y");
	test (0, "abc:", 2, ":z");
	test (0, "abc:", 3, ":z");
	test (1, "abc:", 4, ":z");

	test (0, "abc:", 0, "QQQQQQQ:x");
	test (0, "abc:", 1, "QQQQQQQ:y");
	test (0, "abc:", 2, "QQQQQQQ:z");
	test (0, "abc:", 3, "QQQQQQQ:z");
	test (1, "abc:", 4, "QQQQQQQ:z");

	return 0;
}


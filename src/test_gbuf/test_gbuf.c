#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "global.h"
#include "debug.h"
#include "gbuf.h"


struct Vec2
{
	float x;
	float y;
};

void vec2_fill (struct Vec2 * v0)
{
	int i = 0;
	while (1)
	{
		struct Vec2 * v = gbuf_add (v0);
		if (v == NULL) {break;}
		v->x = i;
		v->y = i * 2;
		i ++;
	}
}

void vec2_print (struct Vec2 * v0)
{
	GBUF_LOOP (size_t, i, v0)
	{
		printf ("(%f %f)\n", v0 [i].x, v0 [i].y);
	}
}

int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");

	struct Vec2 * v = gbuf_init (10, sizeof (struct Vec2), GBUF_MALLOC, NULL);
	vec2_fill (v);
	vec2_print (v);
	gbuf_free (v);


	return 0;
}

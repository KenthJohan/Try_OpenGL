#pragma once

#include "gbuf.h"


struct Grid_Layout
{
	float r;
};

void gen1_grid 
(
	struct Vertex * v0,
	float z, 
	float x1, 
	float x2, 
	float y1, 
	float y2, 
	float r
)
{
	float c [4] = {0.2, 0.2, 0.2, 1.0};
	float x = x1;
	float y = y1;
	while (1)
	{
		struct Vertex * v;
		
		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x, z, y1, 1.0f);
		V4_SET (v->col, c [0], c [1], c [2], c [3]);

		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x, z, y2, 1.0f);
		V4_SET (v->col, c [0], c [1], c [2], c [3]);
		
		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x1, z, y, 1.0f);
		V4_SET (v->col, c [0], c [1], c [2], c [3]);

		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x2, z, y, 1.0f);
		V4_SET (v->col, c [0], c [1], c [2], c [3]);
		
		x += r;
		y += r;
	}
}

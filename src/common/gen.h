#pragma once

#include "gbuf.h"
#include "v.h"


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
	float cc [4] = {0.2, 0.2, 0.2, 1.0};
	float x = x1;
	float y = y1;
	while (1)
	{
		struct Vertex * v;
		float * p;
		float * c;
		v = gbuf_add (v0);
		p = v->pos;
		c = v->col;
		if (v == NULL) {break;}
		p [0] = x;
		p [1] = z;
		p [2] = y1;
		p [3] = 1.0f;
		memcpy (c, cc, 4 * sizeof (float));

		v = gbuf_add (v0);
		p = v->pos;
		c = v->col;
		if (v == NULL) {break;}
		p [0] = x;
		p [1] = z;
		p [2] = y2;
		p [3] = 1.0f;
		memcpy (c, cc, 4 * sizeof (float));
		
		v = gbuf_add (v0);
		p = v->pos;
		c = v->col;
		if (v == NULL) {break;}
		p [0] = x1;
		p [1] = z;
		p [2] = y;
		p [3] = 1.0f;
		memcpy (c, cc, 4 * sizeof (float));
		
		v = gbuf_add (v0);
		p = v->pos;
		c = v->col;
		if (v == NULL) {break;}
		p [0] = x2;
		p [1] = z;
		p [2] = y;
		p [3] = 1.0f;
		memcpy (c, cc, 4 * sizeof (float));
		
		x += r;
		y += r;
	}
}




void gen_circle (struct Vertex * v, size_t n, float r, float x, float y, float z)
{
	for (size_t i = 0; i < n; i = i + 1)
	{
		//V4_SET (v [i].col, 0.0f, 0.0f, 1.0f, 1.0f);
		float a = (2 * M_PI) * ((float)i / (float)n);
		v [i].pos [0] = x + r * cos (a);
		v [i].pos [1] = y + r * sin (a);
		v [i].pos [2] = z;
		v [i].pos [3] = 1.0f;
		//printf ("%f %f %f %f : %f %f %f %f\n", v [i].pos [0], v [i].pos [1], v [i].pos [2], v [i].pos [3], v [i].col [0], v [i].col [1], v [i].col [2], v [i].col [3]);
	}
}


void gen_circle1 (struct Vertex * v, size_t n, float r, float x, float y, float z)
{
	size_t i = 0;
	float a;
	if (i >= n) {return;}
	//V4_SET (v [i].col, 0.0f, 0.0f, 1.0f, 1.0f);
	v [i].pos [0] = x;
	v [i].pos [1] = y;
	v [i].pos [2] = z;
	v [i].pos [3] = 1.0f;
	i ++;
	
	while (1)
	{
		if (i >= n) {break;}
		a = (2 * M_PI) * ((float)i / (float)(n));
		v [i].pos [0] = x + r * cos (a);
		v [i].pos [1] = y + r * sin (a);
		v [i].pos [2] = z;
		v [i].pos [3] = 1.0f;
		i ++;
	}
}

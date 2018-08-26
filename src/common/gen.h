#pragma once



void gen1_grid (struct Vertex * v0, float x1, float x2, float y1, float y2, float r)
{
	float x = x1;
	float y = y1;
	
	while (1)
	{
		struct Vertex * v;
		
		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x, -0.5f, y1, 1.0f);
		V4_SET (v->col, 1.0f, 0.0f, 0.0f, 1.0f);
		
		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x, -0.5f, y2, 1.0f);
		V4_SET (v->col, 1.0f, 0.0f, 0.0f, 1.0f);
		x += r;
		
		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x1, -0.5f, y, 1.0f);
		V4_SET (v->col, 1.0f, 0.0f, 0.0f, 1.0f);
		
		v = gbuf_add (v0);
		if (v == NULL) {break;}
		V4_SET (v->pos, x2, -0.5f, y, 1.0f);
		V4_SET (v->col, 1.0f, 0.0f, 0.0f, 1.0f);
		y += r;
	}
	

}

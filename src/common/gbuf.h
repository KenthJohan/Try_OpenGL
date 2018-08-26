#pragma once

#include "debug.h"
#include <stdint.h>



#define GBUF_SIGNATURE 0
#define GBUF_MALLOC (1 << 0)

#define GBUF_LOOP(t, i, x) for (t (i) = 0; (i) < gbuf ((x))->n; (i) = (i) + 1)

struct GBuffer
{
	uint16_t flags;
	uint32_t cap;
	uint32_t n;
	uint16_t esize8;
};


void * gbuf_init (uint32_t cap, uint16_t esize8, uint16_t flags, void * data)
{
	struct GBuffer * g = data;
	if ((data == NULL) && (flags & GBUF_MALLOC))
	{
		g = malloc (esize8 * cap + sizeof (struct GBuffer));
	}
	ASSERT (g != NULL);
	g->cap = cap;
	g->n = 0;
	g->esize8 = esize8;
	g->flags = flags;
	return g + 1;
}


struct GBuffer * gbuf (void * data)
{
	struct GBuffer * g = (struct GBuffer *) data;
	return g - 1;
}


void * gbuf_add (void * data)
{
	struct GBuffer * g = gbuf (data);
	if (g->n >= g->cap) {return NULL;}
	g->n ++;
	uintptr_t d = (uintptr_t) data;
	d += (g->n - 1) * g->esize8;
	return (void *) d;
}


void gbuf_free (void * data)
{
	struct GBuffer * g = gbuf (data);
	free (g);
}





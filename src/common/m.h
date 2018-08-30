#pragma once

#include <stdio.h>

#define M_ROWMAJ 1 << 0
#define M_COLMAJ 1 << 1



size_t m_index (size_t r, size_t c, size_t rn, size_t cn, uint8_t flags)
{
	if (flags & M_ROWMAJ) {return (r * cn) + (1  * c);}
	if (flags & M_COLMAJ) {return (r * 1 ) + (rn * c);}
	return 0;
}



void m_print (float * m, size_t rn, size_t cn, uint8_t flags, FILE * f)
{
	for (size_t r = 0; r < rn; ++ r)
	{
		for (size_t c = 0; c < cn; ++ c)
		{
			fprintf (f, "%f ", m [m_index (r, c, rn, cn, flags)]);
		}
		fprintf (f, "\n");
	}
	fprintf (f, "\n");
	fflush (f);
}



void m4_print (float * m, uint8_t flags, FILE * f)
{
	m_print (m, 4, 4, flags, f);
}

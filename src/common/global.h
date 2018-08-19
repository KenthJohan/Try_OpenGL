#pragma once

#define COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define LOOP(t, i, n) for (t (i) = 0; (i) < (n); (i) = (i) + 1)
#define LOOP_AB(t, i, a, b) for (t (i) = (a); (i) < (b); (i) = (i) + 1)

#pragma once


// Set vector scalar
// (v := {x | x = s})
#define SET_VS(N, v, s)                                                 \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (s);                                                  \
}

// Set vector vector
// (v := u)
#define SET_VV(N, v, u)                                                 \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i];                                              \
}

// Add vector vector scalar
// (v := u + s)
#define ADD_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] + (s);                                        \
}

// Add vector vector vector
// (v := u + s)
#define ADD_VVV(N, v, u, w)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] + (w)[_i];                                    \
}

// Sub vector vector vector
// (v := u - w)
#define SUB_VVV(N, v, u, w)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] - (w)[_i];                                    \
}

// Sub vector vector scalar
// (v := u - s)
#define SUB_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] - (s);                                        \
}


// Clear vector
// (v := {x | x = 0})
#define CLR_V(N, v)                                                     \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = 0.0;                                                  \
}


#define UNIT_V(N, v, j)                                                 \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (_i == (j) ? 1.0 : 0.0);                              \
}


#define MUL_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] * (s);                                        \
}


#define DIV_VVS(N, v, u, s)                                             \
{                                                                       \
    int _i;                                                             \
    for (_i = 0; _i < (N); _i++)                                        \
        (v)[_i] = (u)[_i] / (s);                                        \
}


#define DOTP_SVV(N, s, v, u)                                            \
{                                                                       \
    int _i;                                                             \
    (s) = 0.0;                                                          \
    for (_i = 0; _i < N; _i++)                                          \
        (s) += (v)[_i] * (u)[_i];                                       \
}


#define ABS_SV(N, s, v)                                                 \
{                                                                       \
    real _tmp;                                                          \
    int _i;                                                             \
    _tmp = 0.0;                                                         \
    for (_i = 0; _i < (N); _i++)                                        \
        _tmp += (v)[_i] * (v)[_i];                                      \
    (s) = rsqrt(_tmp);                                                  \
}


#define DIST_SVV(N, s, u, v)                                            \
{                                                                       \
    real _tmp;                                                          \
    int _i;                                                             \
    _tmp = 0.0;                                                         \
    for (_i = 0; _i < (N); _i++)                                        \
        _tmp += ((u)[_i]-(v)[_i]) * ((u)[_i]-(v)[_i]);                  \
    (s) = rsqrt(_tmp);                                                  \
}


#define CROSSP_SV2V2(s,v,u)                                             \
{                                                                       \
    (s) = (v)[0]*(u)[1] - (v)[1]*(u)[0];                                \
}


#define CROSSP_SV3V3(v,u,w)                                             \
{                                                                       \
    (v)[0] = (u)[1]*(w)[2] - (u)[2]*(w)[1];                             \
    (v)[1] = (u)[2]*(w)[0] - (u)[0]*(w)[2];                             \
    (v)[2] = (u)[0]*(w)[1] - (u)[1]*(w)[0];                             \
}


#define IDENTITY_M(n1, n2, m)                                           \
{                                                                       \
    for (size_t _i = 0; _i < (n1); _i++)                                \
    for (size_t _j = 0; _j < (n2); _j++)                                \
        (m)[_i*n1 + _j] = (_i == _j ? 1.0 : 0.0);                       \
}


#define TRANSLATE_X(m) (m) [12]
#define TRANSLATE_Y(m) (m) [13]
#define TRANSLATE_Z(m) (m) [14]

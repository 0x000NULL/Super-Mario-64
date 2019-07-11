.balign 4
glabel lll_seg7_arr_0702874C # 0x0702874C - 0x070287A8
.hword    1,  8191,     0,  8192,     0,     0,     0,     0
.hword 8192,     0,     2, -8191,     0,  8192,     0,     4
.hword 8191,     0,     0,     2,     0,     0,     0,     0
.hword    2,     2, -8191,     0,     0,     2,     4,  8191
.hword    0, -8192,     4,     0,     0,     0, -8192,     4
.hword    2, -8191,     0, -8192,     4,     4

glabel lll_seg7_dl_070287A8 # 0x070287A8 - 0x070287F0
gsSP2Triangles  0,  3,  1, 0x0,  1,  3,  4, 0x0
gsSP2Triangles  1,  4,  2, 0x0,  2,  4,  5, 0x0
gsSP2Triangles  3,  6,  4, 0x0,  4,  6,  7, 0x0
gsSP2Triangles  4,  7,  5, 0x0,  5,  7,  8, 0x0
gsSPEndDisplayList

lll_seg7_moving_texture_data_070287F0:
.hword     1,     0,     8,     5, -3071, -3071, -3071,  3072
.hword  3072,  3072,  3072, -3071,     1,   200,     4,     0

glabel lll_seg7_moving_texture_07028810
.hword 10, 0
.word lll_seg7_moving_texture_data_070287F0
.hword -1, 0
.word 0x0

glabel lll_seg7_arr_07028820 # 0x07028820 - 0x070288C8
.hword    50,   655,   -86, -2934,     0,     0,   527,   256
.hword -2934,     2,     0,   436,  1042, -2934,     4,     0
.hword   430,  2555, -2934,     7,     0,   430,  5248, -2934
.hword    11,     0,   148,   -86, -2669,     0,     1,   256
.hword   281, -2823,     2,     1,   138,   946, -2870,     4
.hword     1,     1,  2555, -2917,     7,     1,     0,  5248
.hword -2928,    11,     1,    36,    63, -2766,     1,     1
.hword  -574,   -86, -2934,     0,     2,  -471,   247, -2934
.hword     2,     2,  -389,   775, -2934,     4,     2,  -390
.hword  2555, -2934,     7,     2,  -430,  5248, -2934,    11
.hword     2,     0,     0,     0

glabel lll_seg7_dl_070288C8 # 0x070288C8 - 0x07028960
gsSP2Triangles 15, 14,  9, 0x0,  4,  9,  3, 0x0
gsSP2Triangles  9, 14,  8, 0x0,  9,  8,  3, 0x0
gsSP2Triangles 14, 13,  8, 0x0,  8, 13,  7, 0x0
gsSP2Triangles  8,  7,  2, 0x0,  8,  2,  3, 0x0
gsSP2Triangles 10,  6,  7, 0x0, 13, 10,  7, 0x0
gsSP2Triangles 13, 12, 10, 0x0, 12, 11, 10, 0x0
gsSP2Triangles 10, 11,  5, 0x0,  6, 10,  5, 0x0
gsSP2Triangles  6,  5,  0, 0x0,  6,  0,  1, 0x0
gsSP2Triangles  2,  6,  1, 0x0,  7,  6,  2, 0x0
gsSPEndDisplayList

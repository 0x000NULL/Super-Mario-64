.balign 4

glabel bitfs_seg7_arr_07015AF0 # 0x07015AF0 - 0x07015B1C
.hword     2, -7450,   205,  1050,     0,     0,  4838,   205
.hword  1050,     6,     0,  4838,   205, -1108,     6,     2
.hword -7450,   205, -1108,     0,     2,     0

glabel bitfs_seg7_arr_07015B1C # 0x07015B1C - 0x07015B48
.hword    -3, -4531,  3487,  1050,     0,     0,  5658,  3487
.hword  1050,     6,     0,  5658,  3487, -1108,     6,     2
.hword -4531,  3487, -1108,     0,     2,     0

glabel bitfs_seg7_arr_07015B48 # 0x07015B48 - 0x07015BA8
.hword    -2,  8191, -3067,  8192,     0,     0,     0, -3067
.hword  8192,     0,     2, -8191, -3067,  8192,     0,     4
.hword  8191, -3067,     0,     2,     0,     0, -3067,     0
.hword     2,     2, -8191, -3067,     0,     2,     4,  8191
.hword -3067, -8192,     4,     0,     0, -3067, -8192,     4
.hword     2, -8191, -3067, -8192,     4,     4,     0,     0

glabel bitfs_seg7_dl_07015BA8 # 0x07015BA8 - 0x07015BC0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSPEndDisplayList

glabel bitfs_seg7_dl_07015BC0 # 0x07015BC0 - 0x07015C08
gsSP2Triangles  0,  3,  1, 0x0,  1,  3,  4, 0x0
gsSP2Triangles  1,  4,  2, 0x0,  2,  4,  5, 0x0
gsSP2Triangles  3,  6,  4, 0x0,  4,  6,  7, 0x0
gsSP2Triangles  4,  7,  5, 0x0,  5,  7,  8, 0x0
gsSPEndDisplayList

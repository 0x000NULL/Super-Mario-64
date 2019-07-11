ssl_seg7_moving_texture_data_07012758: # 0x07012758 - 0x07012778
.hword     1,     0,    10,     3, -6911, -7167, -6911, -4607
.hword -4223, -4607, -4223, -7167,     1,   150,     0,     0

glabel ssl_seg7_moving_texture_07012778 # 0x07012778
.hword  0, 0
.word ssl_seg7_moving_texture_data_07012758
.hword -1, 0
.word 0x0

ssl_seg7_moving_texture_data_07012788: # 0x07012788 - 0x070127A8
.hword     1,     0,    40,     2,  1024, -7065,  1024,  -716
.hword  7578,  -716,  7578, -7065,     1,   150,     0,     0
ssl_seg7_moving_texture_data_070127A8: # 0x070127A8 - 0x070127C8
.hword     1,     0,    40,     2, -3993, -7065, -3993, -4197
.hword  1024, -4197,  1024, -7065,     1,   150,     0,     0 

glabel ssl_seg7_moving_texture_070127C8 # 0x070127C8 - 0x070127E0
.hword 51, 0
.word ssl_seg7_moving_texture_data_07012788
.hword 52, 0
.word ssl_seg7_moving_texture_data_070127A8
.hword -1, 0
.word 0x0

glabel ssl_seg7_dl_070127E0 # 0x070127E0
gsSPBranchList ssl_seg7_dl_07004818

glabel ssl_seg7_dl_070127E8 # 0x070127E8
gsSPBranchList ssl_seg7_dl_07004860

glabel ssl_seg7_arr_070127F0 # 0x070127F0 - 0x070128B8
.hword    20, -4096,  -256,  1024,    23,   123,   -15,     0
.hword     0,     0,  -256,  1024,   -15,   123,   -23,     0
.hword     2,     0,  -256, -3072,   -23,   123,    15,     0
.hword     4, -4096,  -256, -3072,    17,   123,    23,     0
.hword     2, -4096,     0,  2048,    17,   123,   -21,     1
.hword     0,     0,     0,  2048,    -6,   126,   -12,     1
.hword     2,  1024,     0,  1024,   -14,   126,     2,     1
.hword     2,  1024,     0, -3072,    -6,   125,   -16,     1
.hword     4,     0,     0, -4096,     7,   125,   -15,     1
.hword     4, -4096,     0, -4096,    10,   125,   -12,     1
.hword     2, -5120,     0, -3072,    14,   126,     4,     1
.hword     2, -5120,     0,  1024,    12,   126,    -6,     1
.hword     0,     0,     0,     0

glabel ssl_seg7_dl_070128B8 # 0x070128B8 - 0x07012900
gsSP2Triangles  0,  4,  1, 0x0,  1,  4,  5, 0x0
gsSP2Triangles  1,  6,  2, 0x0,  2,  6,  7, 0x0
gsSP2Triangles  2,  8,  3, 0x0,  3,  8,  9, 0x0
gsSP2Triangles  0,  3, 10, 0x0,  0, 10, 11, 0x0
gsSPEndDisplayList

glabel ssl_seg7_arr_07012900 # 0x07012900 - 0x07012A08
.hword    20, -4096,  -256,  1024,    23,   123,   -15,     0
.hword     1,     0,  -256,  1024,   -15,   123,   -23,     0
.hword     3,     0,  -256, -3072,   -23,   123,    15,     0
.hword     5, -4096,  -256, -3072,    17,   123,    23,     0
.hword     3, -4096,     0,  2048,    17,   123,   -21,     1
.hword     0,     0,     0,  2048,    -6,   126,   -12,     1
.hword     2,  1024,     0,  1024,   -14,   126,     2,     1
.hword     2,  1024,     0, -3072,    -6,   125,   -16,     1
.hword     4,     0,     0, -4096,     7,   125,   -15,     1
.hword     4, -4096,     0, -4096,    10,   125,   -12,     1
.hword     2, -5120,     0, -3072,    14,   126,     4,     1
.hword     2, -5120,     0,  1024,    12,   126,    -6,     1
.hword     0, -4608,     0,  1536,    15,   125,   -14,     1
.hword     1,   512,     0,  1536,   -10,   126,    -5,     1
.hword     3,   512,     0, -3584,     1,   125,   -15,     1
.hword     5, -4608,     0, -3584,    12,   126,    -4,     1
.hword     3,     0,     0,     0

glabel ssl_seg7_dl_07012A08 # 0x07012A08 - 0x07012A50
gsSP2Triangles  0, 11, 12, 0x0,  0, 12,  4, 0x0
gsSP2Triangles  1,  5, 13, 0x0,  1, 13,  6, 0x0
gsSP2Triangles  2,  7, 14, 0x0,  2, 14,  8, 0x0
gsSP2Triangles  3,  9, 15, 0x0,  3, 15, 10, 0x0
gsSPEndDisplayList

glabel ssl_seg7_arr_07012A50 # 0x07012A50 - 0x07012B48
.hword    20, -8192,  -512,  8704,     0,   113,    56,     0
.hword     0, -8192,  -256,  8192,     0,   113,    56,     1
.hword     0, -8192,     0,  7680,     0,   106,    69,     2
.hword     0,     0,  -512,  8704,     0,   113,    56,     0
.hword     3,     0,  -256,  8192,     0,   113,    56,     1
.hword     3,     0,     0,  7680,     2,   122,    34,     2
.hword     3,  8704,  -512,  8704,    29,   119,    29,     0
.hword     6,  8192,  -256,  8192,    29,   119,    29,     1
.hword     6,  7680,     0,  7680,     6,   126,     6,     2
.hword     6,  8704,  -512,     0,    56,   113,     0,     0
.hword     9,  8192,  -256,     0,    56,   113,     0,     1
.hword     9,  7680,     0,     0,    20,   125,     0,     2
.hword     9,  8704,  -512, -8192,    56,   113,     0,     0
.hword    12,  8192,  -256, -8192,    56,   113,     0,     1
.hword    12,  7680,     0, -8192,    43,   119,     0,     2
.hword    12,     0,     0,     0

glabel ssl_seg7_dl_07012B48 # 0x07012B48 - 0x07012BD0
gsSP2Triangles  2,  1,  5, 0x0,  1,  4,  5, 0x0
gsSP2Triangles  0,  3,  1, 0x0,  1,  3,  4, 0x0
gsSP2Triangles  4,  7,  5, 0x0,  5,  7,  8, 0x0
gsSP2Triangles  4,  3,  7, 0x0,  3,  6,  7, 0x0
gsSP2Triangles  8,  7, 11, 0x0, 11,  7, 10, 0x0
gsSP2Triangles  7,  9, 10, 0x0,  7,  6,  9, 0x0
gsSP2Triangles 11, 10, 13, 0x0, 11, 13, 14, 0x0
gsSP2Triangles 10,  9, 13, 0x0, 13,  9, 12, 0x0
gsSPEndDisplayList

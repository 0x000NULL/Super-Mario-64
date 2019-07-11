.balign 4
ttm_seg7_moving_texture_data_07017104: # 0x07017104 - 0x07017124
.hword     1,     0,   100,     3, -2047,  2765, -2047,  5069
.hword  1946,  5069,  1946,  2765,     0,   150,     0,     0

glabel ttm_seg7_moving_texture_07017124 # 0x07017124 - 0x07017134
.hword  0, 0
.word ttm_seg7_moving_texture_data_07017104
.hword -1, 0
.word 0x0

glabel ttm_seg7_arr_07017134 # 0x07017134 - 0x07017174
.hword 35
.hword  2191, -1638,  1188, 0, 0
.hword  2165,  1638,  1145, 3, 0
.hword  1625,  2286,   757, 4, 0
.hword  1756, -1638,  1457, 0, 1
.hword  1729,  1638,  1413, 3, 1
.hword  1221,  2286,  1077, 4, 1
.hword 0

glabel ttm_seg7_arr_07017174 # 0x07017174 - 0x070171A0
.hword 30
.hword  3583, -1638,  2751, 0, 0
.hword  3583, -1638,   898, 5, 0
.hword  1586, -1638,  2751, 0, 2
.hword  1586, -1638,   898, 5, 2
.hword 0

glabel ttm_seg7_arr_070171A0 # 0x070171A0 - 0x070171E0
.hword 25
.hword  3583, -3840,  2913, 0, 0
.hword  3583, -1889,  2913, 2, 0
.hword  3583, -1638,  2751, 3, 0
.hword  2559, -3840,  2913, 0, 1
.hword  2559, -1889,  2913, 2, 1
.hword  2457, -1638,  2751, 3, 1
.hword 0

glabel ttm_seg7_arr_070171E0 # 0x070171E0 - 0x0701720C
.hword 20
.hword  3640, -3840,  3422, 0, 0
.hword  3792, -3840,  2849, 1, 0
.hword  2358, -3840,  3422, 0, 1
.hword  2358, -3840,  2849, 1, 1
.hword 0

glabel ttm_seg7_arr_0701720C # 0x0701720C - 0x07017260
.hword 15
.hword  1946, -4403,  4520, 0, 0
.hword  1992, -4096,  4520, 1, 0
.hword  3640, -4096,  3496, 3, 0
.hword  3640, -3840,  3422, 4, 0
.hword  1946, -4403,  4008, 0, 1
.hword  1992, -4096,  4008, 1, 1
.hword  2358, -4096,  3496, 3, 1
.hword  2358, -3840,  3422, 4, 1
.hword 0

glabel ttm_seg7_dl_07017260 # 0x07017260 - 0x07017288
gsSP2Triangles  0,  1,  3, 0x0,  3,  1,  4, 0x0
gsSP2Triangles  1,  2,  4, 0x0,  4,  2,  5, 0x0
gsSPEndDisplayList

glabel ttm_seg7_dl_07017288 # 0x07017288 - 0x070172A0
gsSP2Triangles  0,  1,  2, 0x0,  2,  1,  3, 0x0
gsSPEndDisplayList

glabel ttm_seg7_dl_070172A0 # 0x070172A0 - 0x070172D8
gsSP2Triangles  0,  1,  4, 0x0,  4,  1,  5, 0x0
gsSP2Triangles  1,  2,  5, 0x0,  5,  2,  6, 0x0
gsSP2Triangles  2,  3,  6, 0x0,  6,  3,  7, 0x0
gsSPEndDisplayList

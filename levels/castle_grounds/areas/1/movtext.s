castle_grounds_seg7_moving_texture_data_07010E40: # 0x07010E40 - 0x07010E60
.hword     1,     0,    20,    15, -7129, -7222, -7129,   -58
.hword  8253,   -58,  8253, -7222,     1,   150,     0,     0

castle_grounds_seg7_moving_texture_data_07010E60: # 0x07010E60 - 0x07010E80
.hword     1,     0,    15,    10,  1024,   -58,  1024,  8137
.hword  8230,  8137,  8230,   -58,     0,   150,     0,     0

glabel castle_grounds_seg7_moving_texture # 0x07010E80
.hword 0, 0
.word castle_grounds_seg7_moving_texture_data_07010E40
.hword 1, 0
.word castle_grounds_seg7_moving_texture_data_07010E60
.hword -1, 0

# TODO: alignment?
.hword  0, 0

glabel castle_grounds_seg7_arr_07010E98
.hword    70, -4469,  -800, -6413,     0,     0, -5525,  1171
.hword -7026,     2,     0, -6292,  2028, -7463,     4,     0
.hword -7302,  2955, -7461,     6,     0, -4883,  -800, -5690
.hword     0,     3, -5547,  1110, -6097,     2,     3, -6732
.hword  2587, -6770,     4,     3, -7603,  3004, -7160,     6
.hword     3, -5580,  -800, -4740,     0,     6, -6205,  1068
.hword -5347,     2,     6, -7249,  2566, -6192,     4,     6
.hword -6895,  -800, -4714,     0,     9, -7201,  1083, -5071
.hword     2,     9, -7578,  2042, -5766,     4,     9, -8132
.hword  2961, -6761,     6,     9

glabel castle_grounds_seg7_dl_07010F30 # 0x07010F30 - 0x07010FC0
gsSP2Triangles  0,  1,  5, 0x0,  0,  5,  4, 0x0
gsSP2Triangles  1,  2,  6, 0x0,  1,  6,  5, 0x0
gsSP2Triangles  2,  3,  6, 0x0,  3,  7,  6, 0x0
gsSP2Triangles  4,  5,  9, 0x0,  4,  9,  8, 0x0
gsSP2Triangles  5,  6,  9, 0x0,  6, 10,  9, 0x0
gsSP2Triangles  6,  7, 10, 0x0,  8,  9, 12, 0x0
gsSP2Triangles  8, 12, 11, 0x0,  9, 10, 13, 0x0
gsSP2Triangles  9, 13, 12, 0x0, 10,  7, 14, 0x0
gsSP1Triangle 10, 14, 13, 0x0
gsSPEndDisplayList

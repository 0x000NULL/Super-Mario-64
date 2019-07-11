inside_castle_seg7_moving_texture_data_07079090: # 0x07079090 - 0x070790F0
.hword     1,     0,    10,    10, -3225, -4146, -3225,  -255
.hword   870,  -255,   870, -4146,     1,   150,     2,     0

inside_castle_seg7_moving_texture_data_070790B0: # 0x070790B0 - 0x070790D0
.hword     1,     0,     5,     7,   973, -1279,   973,   256
.hword  5786,   256,  5786, -1279,     1,   150,     0,     0

inside_castle_seg7_moving_texture_data_070790D0: # 0x070790D0 - 0x07079100
.hword     1,     0,     3,     5,  5786, -1330,  5786,   410
.hword  8038,   410,  8038, -1330,     1,   150,     0,     0

glabel inside_castle_seg7_moving_texture_070790F0 # 0x070790F0 - 0x07079100
.hword  0, 0
.word inside_castle_seg7_moving_texture_data_07079090
.hword -1, 0
.word 0x0

glabel inside_castle_seg7_moving_texture_07079100 # 0x07079100 - 0x07079118
.hword  1, 0
.word inside_castle_seg7_moving_texture_data_070790B0
.hword  2, 0
.word inside_castle_seg7_moving_texture_data_070790D0
.hword -1, 0
.word 0x0

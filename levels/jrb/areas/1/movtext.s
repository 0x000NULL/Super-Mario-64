.balign 4
jrb_seg7_moving_texture_data_0700D28C: # 0x0700D28C - 0x0700D2AC
.hword     1,     0,    20,     6, -6304,  -669, -6304,  7814
.hword  7992,  7814,  7992,  -669,     1,   180,     2,     0
jrb_seg7_moving_texture_data_0700D2AC: # 0x0700D2AC - 0x0700D2CC
.hword     1,     0,    10,     2,  4433, -4253,  4433,  -669
.hword  5969,  -669,  5969, -4253,     1,   180,     2,     0

glabel jrb_seg7_moving_texture_0700D2CC # 0x0700D2CC
.hword  0, 0
.word jrb_seg7_moving_texture_data_0700D28C
.hword  1, 0
.word jrb_seg7_moving_texture_data_0700D2AC
.hword -1, 0
.word 0x0

jrb_seg7_moving_texture_data_0700D2E4: # 0x0700D2E4
.hword     1,     0,    20,    10, -7818, -1125, -7818,  7814
.hword  9055,  7814,  9055, -1125,     1,    50,     1,     0

glabel jrb_seg7_moving_texture_0700D304 # 0x0700D304
.hword 51, 0 
.word jrb_seg7_moving_texture_data_0700D2E4
.hword -1, 0
.word 0x0

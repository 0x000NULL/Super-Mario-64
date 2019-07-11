.balign 4 # 0x0700FA2E - 0x0700FA30

sl_seg7_moving_texture_data_0700FA30: # 0x0700FA30 - 0x0700FA50
.hword     1,     0,     8,    10, -6194,  -409, -6194,  4198
.hword   154,  4198,   154,  -409,     1,   150,     0,     0

sl_seg7_moving_texture_data_0700FA50: # 0x0700FA50 - 0x0700FA70
.hword     1,     0,     8,    10, -1279, -6143, -1279, -3071
.hword  1792, -3071,  1792, -6143,     1,   150,     0,     0

glabel sl_seg7_moving_texture_0700FA70 # 0x0700FA70 - 0x0700FA88
.hword  0, 0
.word sl_seg7_moving_texture_data_0700FA30
.hword  1, 0
.word sl_seg7_moving_texture_data_0700FA50
.hword -1, 0
.word 0x0

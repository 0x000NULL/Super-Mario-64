bbh_seg7_moving_texture_data_07026DE4: # 0x07026DE4
.hword     1,     0,     0,    20, -4812,  1485, -4812,  7270
.hword   640,  7270,   640,  1485,     0,   150,     0,     0

bbh_seg7_moving_texture_data_07026E04: # 0x07026E14
.hword     1,     0,     0,    20,  1536, -1637,  1536,  2662
.hword  3789,  2662,  3789, -1637,     0,   150,     0,     0

glabel bbh_seg7_moving_texture_07026E24 # 0x07026E24
.hword  0, 0
.word bbh_seg7_moving_texture_data_07026DE4
.hword -1, 0
.word 0x0

glabel bbh_seg7_moving_texture_07026E34 # 0x07026E34
.hword  1, 0
.word bbh_seg7_moving_texture_data_07026E04
.hword -1, 0
.word 0x0

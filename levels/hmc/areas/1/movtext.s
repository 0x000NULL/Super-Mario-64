glabel hmc_seg7_arr_702B86C # 0x0702B86C - 0x0702B8E0
.hword  0,  1272,  2000,  5120
.hword  1,  1272,  2000,  3500
.hword  2,  1800,  2100,  3000
.hword  3,  4100,  2100,  3000
.hword  4,  4640,  2160,  3980
.hword  5,  5416,  2100,  3642
.hword  6,  6720,  1950,  4440
.hword  7,  6700,  1880,  5040
.hword  8,  6060,  1880,  5640
.hword  9,  5440,  1850,  6200
.hword 10,  3820,  1990,  7130
.hword 11,  1270,  1990,  7130
.hword 12,  1272,  2000,  6000
.hword 13,  1272,  2000,  5400
.hword -1
.hword  0

hmc_seg7_moving_texture_data_0702B8E0: # 0x0702B8E0
.hword     1,     0,    20,    12, -7628, -2559, -7628,  7654
.hword   563,  7654,   563, -2559,     1,   150,     0,     0
glabel hmc_seg7_moving_texture_0702B900
.hword  0, 0
.word hmc_seg7_moving_texture_data_0702B8E0
.hword -1, 0
.word 0x0

hmc_seg7_moving_texture_data_0702B910: # 0x0702B910
.hword     1,     0,    15,     3,  1690, -6348,  1690,   819
.hword  6298,   819,  6298, -6348,     1,   120,     1,     0
hmc_seg7_moving_texture_data_0702B930: # 0x0702B930
.hword     1,     0,     8,     3,  1690, -6348,  1690,   819
.hword  6298,   819,  6298, -6348,     0,   180,     1,     0

glabel hmc_seg7_moving_texture_0702B950
.hword 50, 0
.word hmc_seg7_moving_texture_data_0702B910
.hword 51, 0
.word hmc_seg7_moving_texture_data_0702B930
.hword -1, 0
.word 0x0

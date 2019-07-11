.balign 4
thi_seg7_moving_texture_data_0700E2BC: # 0x0700E2BC
.hword     1,     0,    10,    10, -8191, -8191, -8191,  6246
.hword -5119,  6246, -5119, -8191,     1,   150,     0,     0
thi_seg7_moving_texture_data_0700E2DC: # 0x0700E2DC
.hword     1,     0,    10,    10, -4607,  4506, -4607,  8192
.hword  6963,  8192,  6963,  4506,     1,   150,     0,     0
thi_seg7_moving_texture_data_0700E2FC: # 0x0700E2FC
.hword     1,     0,    10,    10, -1023, -2555, -1023,  -507
.hword  1024,  -507,  1024, -2555,     1,   150,     0,     0

glabel thi_seg7_moving_texture_0700E31C # 0x0700E31C
.hword 0, 0
.word thi_seg7_moving_texture_data_0700E2BC
.hword 1, 0
.word thi_seg7_moving_texture_data_0700E2DC
.hword 2, 0
.word thi_seg7_moving_texture_data_0700E2FC
.hword -1, 0
.word 0x0

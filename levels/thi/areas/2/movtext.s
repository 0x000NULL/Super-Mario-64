thi_seg7_moving_texture_data_0700E33C: # 0x0700E33C
.hword     1,     0,     3,     3, -2457, -2457, -2457,  1874
.hword -1535,  1874, -1535, -2457,     1,   150,     0,     0
thi_seg7_moving_texture_data_0700E35C: # 0x0700E35C
.hword      1,     0,    3,     3, -1381,  1352, -1381,  2458
.hword   2089,  2458, 2089,  1352,     1,   150,     0,     0
thi_seg7_moving_texture_data_0700E37C: # 0x0700E37C
.hword      1,     0,    3,     3,  -306,  -766,  -306,  -152
.hword    307,  -152,  307,  -766,     1,   150,     0,     0

glabel thi_seg7_moving_texture_0700E39C # 0x0700E39C
.hword 0, 0
.word thi_seg7_moving_texture_data_0700E33C 
.hword 1, 0
.word thi_seg7_moving_texture_data_0700E35C 
.hword 2, 0
.word thi_seg7_moving_texture_data_0700E37C 
.hword -1, 0
.word 0x0

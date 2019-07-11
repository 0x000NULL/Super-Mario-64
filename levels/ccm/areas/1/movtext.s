.balign 4
ccm_seg7_moving_texture_data: # 0x070166E8
.hword     1,     0,     5,     2, 3137, 4228, 3137, 4945
.hword  3925,  4945,  3925,  4228,    1,  150,    0,    0

glabel ccm_seg7_moving_texture # 0x07016708 - 0x07016718
.hword 0x0000, 0x0000
.word ccm_seg7_moving_texture_data
.hword -1, 0, 0, 0

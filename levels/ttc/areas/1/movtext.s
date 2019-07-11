.balign 8 # 0x07015F72 - 0x07015F78

ttc_seg7_light_07015F78: # 0x07015F78 - 0x07015F80
.byte 0x3F, 0x3F, 0x3F, 0x00, 0x3F, 0x3F, 0x3F, 0x00

ttc_seg7_light_07015F80: # 0x07015F80 - 0x07015F90
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

glabel ttc_seg7_texture_07015F90 # 0x07015F90 - 0x07016790
.incbin "levels/ttc/2.rgba16"

glabel ttc_seg7_dl_07016790 # 0x07016790 - 0x07016808
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 200, 255, 255, 255
gsSPFogPosition 900, 1000
gsSPSetGeometryMode G_FOG
gsDPSetCombineMode G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPLight ttc_seg7_light_07015F80, 1
gsSPLight ttc_seg7_light_07015F78, 2
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPEndDisplayList

glabel ttc_seg7_dl_07016808 # 0x07016808 - 0x07016840
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsDPSetRenderMode G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2
gsSPClearGeometryMode G_FOG
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPEndDisplayList


glabel ttc_seg7_arr_07016840 # 0x07016840 - 0x07016904
.hword 120
.hword  230, -86,  549,  0,   0,  127,  0, 0
.hword  230, -35,  549,  0,  33,  122,  1, 0
.hword  230,   0,  512,  0, 110,   62,  2, 0
.hword  230,   0, -511,  0, 123,  -31, 14, 0
.hword  230, -35, -547,  0,  65, -108, 15, 0
.hword  230, -86, -547,  0,   0, -127, 16, 0
.hword -229, -86,  549,  0,   0,  127,  0, 1
.hword -229, -35,  549,  0,  66,  108,  1, 1
.hword -229,   0,  512,  0, 123,   31,  2, 1
.hword -229,   0, -511,  0, 110,  -63, 14, 1
.hword -229, -35, -547,  0,  32, -122, 15, 1
.hword -229, -86, -547,  0,   0, -127, 16, 1
.hword 0

glabel ttc_seg7_arr_07016904 # 0x07016904 - 0x070169C8
.hword 120
.hword  230, -86,  344,  0,   0,  127,  0, 0
.hword  230, -35,  344,  0,  32,  122,  1, 0
.hword  230,   0,  308,  0, 110,   63,  2, 0
.hword  230,   0, -306,  0, 123,  -31,  9, 0
.hword  230, -35, -342,  0,  65, -108, 10, 0
.hword  230, -86, -342,  0,   0, -127, 11, 0
.hword -229, -86,  344,  0,   0,  127,  0, 1
.hword -229, -35,  344,  0,  65,  108,  1, 1
.hword -229,   0,  308,  0, 123,   31,  2, 1
.hword -229,   0, -306,  0, 110,  -63,  9, 1
.hword -229, -35, -342,  0,  32, -122, 10, 1
.hword -229, -86, -342,  0,   0, -127, 11, 1
.hword 0

glabel ttc_seg7_dl_070169C8 # 0x070169C8 - 0x07016A20
gsSP2Triangles  0,  1,  6, 0x0,  1,  7,  6, 0x0
gsSP2Triangles  1,  2,  7, 0x0,  2,  8,  7, 0x0
gsSP2Triangles  2,  3,  8, 0x0,  3,  9,  8, 0x0
gsSP2Triangles  3,  4,  9, 0x0,  4, 10,  9, 0x0
gsSP2Triangles  4,  5, 10, 0x0,  5, 11, 10, 0x0
gsSPEndDisplayList

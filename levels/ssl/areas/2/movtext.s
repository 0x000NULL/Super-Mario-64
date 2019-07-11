.balign 4 # 0x070285EE - 0x070285F0

glabel ssl_seg7_dl_070285F0 # 0x070285F0 - 0x07028660
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_INTER2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 0, 0, 0, 255
gsSPFogFactor 0x0E49, 0xF2B7 # This isn't gsSPFogPosition since there is no valid min/max pair that corresponds to 0x0E49F2B7
gsSPSetGeometryMode G_FOG
gsDPSetCombineMode G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPEndDisplayList

glabel ssl_seg7_dl_07028660 # 0x07028660 - 0x070286A0
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsSPGeometryMode G_FOG, G_LIGHTING | G_CULL_BACK, G_ORDER_SFIRST
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsDPSetRenderMode G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2
gsSPEndDisplayList

glabel ssl_seg7_dl_070286A0 # 0x070286A0 - 0x07028718
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_INTER2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 0, 0, 0, 255
gsSPFogFactor 0x0E49, 0xF2B7 # This isn't gsSPFogPosition since there is no valid min/max pair that corresponds to 0x0E49F2B7
gsSPSetGeometryMode G_FOG
gsDPSetEnvColor 255, 255, 255, 180
gsDPSetCombineMode G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_ENVIRONMENT, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPEndDisplayList

glabel ssl_seg7_dl_07028718 # 0x07028718 - 0x07028760
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsSPGeometryMode G_FOG, G_LIGHTING | G_CULL_BACK, G_ORDER_SFIRST
gsDPSetEnvColor 255, 255, 255, 255
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsDPSetRenderMode G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2
gsSPEndDisplayList

glabel ssl_seg7_arr_07028760 # 0x07028760 - 0x070287B8
.hword    50,   102,  1229,  -742,     0,     0,   102,  4275
.hword  -742,     5,     0,   102,  4300,  -768,     6,     0
.hword   102,  4300,  -870,     8,     0,  -102,  1229,  -742
.hword     0,     1,  -102,  4275,  -742,     5,     1,  -102
.hword  4300,  -768,     6,     1,  -102,  4300,  -870,     8
.hword     1,     0,     0,     0

glabel ssl_seg7_dl_070287B8 # 0x070287B8 - 0x070287F0
gsSP2Triangles  0,  1,  4, 0x0,  4,  1,  5, 0x0
gsSP2Triangles  1,  2,  5, 0x0,  5,  2,  6, 0x0
gsSP2Triangles  2,  3,  6, 0x0,  6,  3,  7, 0x0
gsSPEndDisplayList

glabel ssl_seg7_arr_070287F0 # 0x070287F0 - 0x07028844
.hword     8,  1178,  1229,  2150,     0,     0, -1741,  1229
.hword  2150,     2,     0, -1741,  1229,  -589,     4,     0
.hword   154,  1229,  -589,     5,     0,  1178,  1229,  2560
.hword     0,     1, -2150,  1229,  2560,     2,     1, -2150
.hword  1229,  -794,     4,     1,   154,  1229,  -794,     5
.hword     1,     0

glabel ssl_seg7_arr_07028844 # 0x07028844 - 0x07028888
.hword    50,  1229,  -307,  2150,     0,     0,  1229,  1168
.hword  2150,     1,     0,  1178,  1229,  2150,     2,     0
.hword  1229,  -307,  2560,     0,     1,  1229,  1168,  2560
.hword     1,     1,  1178,  1229,  2560,     2,     1,     0
.hword     0,     0

glabel ssl_seg7_dl_07028888 # 0x07028888 - 0x070288B0
gsSP2Triangles  0,  1,  3, 0x0,  1,  4,  3, 0x0
gsSP2Triangles  1,  2,  4, 0x0,  2,  5,  4, 0x0
gsSPEndDisplayList

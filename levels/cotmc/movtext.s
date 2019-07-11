.balign 8

glabel cotmc_seg7_dl_0700BE10 # 0x0700BE10 - 0x0700BE88
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_INTER2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 0, 0, 0, 255
gsSPFogPosition 980, 1000
gsSPSetGeometryMode G_FOG
gsDPSetEnvColor 255, 255, 255, 140
gsDPSetCombineMode G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_ENVIRONMENT, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPEndDisplayList

glabel cotmc_seg7_dl_0700BE88 # 0x0700BE88 - 0x0700BED0
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsSPGeometryMode G_FOG, G_LIGHTING | G_CULL_BACK, G_ORDER_SFIRST
gsDPSetEnvColor 255, 255, 255, 255
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsDPSetRenderMode G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2
gsSPEndDisplayList

glabel cotmc_seg7_arr_0700BED0 # 0x0700BED0 - 0x0700BF60
.hword    30,   256,     0, -7373,     0,     0,   256,  5120
.hword -7373,     4,     0,  -256,     0, -7373,     0,     1
.hword  -256,  5120, -7373,     4,     1,  1536,  -204,  3584
.hword     0,     0,  1536,     0,  3430,     1,     0,  1536
.hword     0, -7680,     5,     0, -1536,  -204,  3584,     0
.hword     2, -1536,     0,  3430,     1,     2, -1536,     0
.hword -7680,     5,     2, -1024,  -614,  3584,     0,     0
.hword -1024,  1434,  3584,     1,     0,  1024,  -614,  3584
.hword     0,     1,  1024,  1434,  3584,     1,     1,     0 

glabel cotmc_seg7_dl_0700BF60 # 0x0700BF60 - 0x0700BFA8
gsSP2Triangles  0,  1,  2, 0x0,  2,  1,  3, 0x0
gsSP2Triangles  4,  5,  7, 0x0, 10, 11, 12, 0x0
gsSP2Triangles 12, 11, 13, 0x0,  7,  5,  8, 0x0
gsSP2Triangles  5,  6,  8, 0x0,  8,  6,  9, 0x0
gsSPEndDisplayList

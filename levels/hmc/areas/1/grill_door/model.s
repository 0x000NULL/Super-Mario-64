hmc_seg7_vertex_0701FDC8: # 0x0701FDC8 - 0x0701FEB8
vertex    410,      0,     51,      0,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,    717,    -50,    990,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,    717,     51,      0,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,    717,     51,   3034,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,    717,    -50,   3034,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,    717,    -50,  -1052,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,    717,     51,  -1052,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,    717,     51,      0,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,    717,    -50,    990,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,      0,    -50,    990,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,      0,     51,      0,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,      0,     51,  -1052,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,      0,    -50,  -1052,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,      0,    -50,   3034,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,      0,     51,   3034,      0,  0xFF, 0xFF, 0xFF, 0xFF

hmc_seg7_vertex_0701FEB8: # 0x0701FEB8 - 0x0701FF68
vertex    410,    717,     51,   2012,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,      0,     51,  -2074,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,      0,     51,   2012,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,      0,     51,      0,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,      0,    -50,    990,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,    717,    -50,    990,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,      0,    -50,   2012,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,      0,    -50,  -2074,   4054,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,    717,    -50,  -2074,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex    410,    717,    -50,   2012,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,    717,     51,  -2074,  -3098,  0xFF, 0xFF, 0xFF, 0xFF

hmc_seg7_dl_0701FF68: # 0x0701FF68 - 0x0701FFF8
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, cave_09001000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex hmc_seg7_vertex_0701FDC8, 15, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  3,  5,  6, 0x0,  7,  8,  9, 0x0
gsSP2Triangles  7,  9, 10, 0x0, 11, 12, 13, 0x0
gsSP1Triangle 11, 13, 14, 0x0
gsSPVertex hmc_seg7_vertex_0701FEB8, 11, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  6,  7,  8, 0x0,  6,  8,  9, 0x0
gsSP1Triangle  0, 10,  1, 0x0
gsSPEndDisplayList

glabel hmc_seg7_dl_0701FFF8 # 0x0701FFF8 - 0x070200B0
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 0, 0, 0, 255
gsSPFogPosition 960, 1000
gsSPSetGeometryMode G_FOG
gsDPSetCombineMode G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_TEXEL0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList hmc_seg7_dl_0701FF68
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsDPSetRenderMode G_RM_AA_ZB_TEX_EDGE, G_RM_NOOP2
gsSPClearGeometryMode G_FOG
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING | G_CULL_BACK
gsSPEndDisplayList

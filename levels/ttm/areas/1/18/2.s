ttm_seg7_vertex_070109F0: # 0x070109F0 - 0x07010A30
vertex    750,   -767,   -340,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -174,   -767,   1212,   5980,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -104,   -562,   1194,   5810,    288,  0xFF, 0xFF, 0xFF, 0xFF
vertex    794,   -562,   -314,      0,    288,  0xFF, 0xFF, 0xFF, 0xFF

ttm_seg7_dl_07010A30: # 0x07010A30 - 0x07010A68
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, ttm_seg7_texture_07001000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex ttm_seg7_vertex_070109F0, 4, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSPEndDisplayList

glabel ttm_seg7_dl_07010A68 # 0x07010A68 - 0x07010AD8
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_TEXEL0
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList ttm_seg7_dl_07010A30
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING | G_CULL_BACK
gsSPEndDisplayList

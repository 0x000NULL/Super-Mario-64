bbh_seg7_vertex_0700D178: # 0x0700D178 - 0x0700D278
vertex  -2037,   1024,   2316,  -1470,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1863,   1126,   2142,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -2037,   1126,   2316,  -1470,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1310,    819,   1126,   4980,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1310,    922,   1126,   4980,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1310,    922,   1629,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1310,    819,   1629,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1310,    922,   1629,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -805,    922,   2032,  -5454,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -805,    819,   2032,  -5454,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1310,    819,   1629,    990,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1863,   1024,   1126,  10104,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1863,   1126,   2142,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1863,   1024,   2142,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1863,   1126,   1126,  10104,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -1863,   1024,   2142,    990,    990,  0xFF, 0xFF, 0xFF, 0xFF

bbh_seg7_dl_0700D278: # 0x0700D278 - 0x0700D2E0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, spooky_09008000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex bbh_seg7_vertex_0700D178, 16, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  3,  5,  6, 0x0,  7,  8,  9, 0x0
gsSP2Triangles  7,  9, 10, 0x0, 11, 12, 13, 0x0
gsSP2Triangles 11, 14, 12, 0x0,  0, 15,  1, 0x0
gsSPEndDisplayList

glabel bbh_seg7_dl_0700D2E0 # 0x0700D2E0 - 0x0700D350
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_TEXEL0
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList bbh_seg7_dl_0700D278
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING | G_CULL_BACK
gsSPEndDisplayList

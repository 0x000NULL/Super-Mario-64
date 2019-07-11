bbh_seg7_light_0701F0E0: # 0x0701F0E0 - 0x0701F0E8
.byte 0x66, 0x66, 0x66, 0x00, 0x66, 0x66, 0x66, 0x00

bbh_seg7_light_0701F0E8: # 0x0701F0E8 - 0x0701F0F8
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

bbh_seg7_vertex_0701F0F8: # 0x0701F0F8 - 0x0701F138
vertex    154,    614,   -101,   3034,   2010,  0x00, 0x7F, 0x00, 0xFF
vertex   -153,    614,   -101,  -3096,   2010,  0x00, 0x7F, 0x00, 0xFF
vertex   -153,    614,    102,  -3096,   6098,  0x00, 0x7F, 0x00, 0xFF
vertex    154,    614,    102,   3034,   6098,  0x00, 0x7F, 0x00, 0xFF

bbh_seg7_vertex_0701F138: # 0x0701F138 - 0x0701F238
vertex    154,      0,   -101,    990,   2012,  0x00, 0x00, 0x81, 0xFF
vertex   -153,      0,   -101,      0,   2012,  0x00, 0x00, 0x81, 0xFF
vertex    154,    614,   -101,    990,      0,  0x00, 0x00, 0x81, 0xFF
vertex    154,    614,    102,    990,      0,  0x00, 0x00, 0x7F, 0xFF
vertex   -153,      0,    102,      0,   2012,  0x00, 0x00, 0x7F, 0xFF
vertex    154,      0,    102,    990,   2012,  0x00, 0x00, 0x7F, 0xFF
vertex   -153,    614,    102,      0,      0,  0x00, 0x00, 0x7F, 0xFF
vertex   -153,      0,   -101,    308,   2012,  0x81, 0x00, 0x00, 0xFF
vertex   -153,    614,    102,    990,      0,  0x81, 0x00, 0x00, 0xFF
vertex   -153,    614,   -101,    308,      0,  0x81, 0x00, 0x00, 0xFF
vertex   -153,      0,    102,    990,   2012,  0x81, 0x00, 0x00, 0xFF
vertex    154,      0,   -101,    308,   2012,  0x7F, 0x00, 0x00, 0xFF
vertex    154,    614,   -101,    308,      0,  0x7F, 0x00, 0x00, 0xFF
vertex    154,    614,    102,    990,      0,  0x7F, 0x00, 0x00, 0xFF
vertex    154,      0,    102,    990,   2012,  0x7F, 0x00, 0x00, 0xFF
vertex   -153,    614,   -101,      0,      0,  0x00, 0x00, 0x81, 0xFF

bbh_seg7_dl_0701F238: # 0x0701F238 - 0x0701F280
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, spooky_0900A000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPLight bbh_seg7_light_0701F0E8, 1
gsSPLight bbh_seg7_light_0701F0E0, 2
gsSPVertex bbh_seg7_vertex_0701F0F8, 4, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSPEndDisplayList

bbh_seg7_dl_0701F280: # 0x0701F280 - 0x0701F2E8
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, spooky_09005000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x100
gsSPVertex bbh_seg7_vertex_0701F138, 16, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  3,  6,  4, 0x0,  7,  8,  9, 0x0
gsSP2Triangles  7, 10,  8, 0x0, 11, 12, 13, 0x0
gsSP2Triangles 11, 13, 14, 0x0,  1, 15,  2, 0x0
gsSPEndDisplayList

glabel bbh_seg7_dl_0701F2E8 # 0x0701F2E8 - 0x0701F378
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPClearGeometryMode G_SHADING_SMOOTH
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList bbh_seg7_dl_0701F238
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 252
gsSPDisplayList bbh_seg7_dl_0701F280
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_SHADING_SMOOTH
gsSPEndDisplayList

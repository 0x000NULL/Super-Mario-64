thi_seg7_vertex_07009DF0: # 0x07009DF0 - 0x07009EB0
vertex  -2047,   2150,   2048,   4056,   5076,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -716,   1843,    717,   1398,   2418,  0xFF, 0xFF, 0xFF, 0xFF
vertex    717,   1843,    717,  -1460,   2418,  0xFF, 0xFF, 0xFF, 0xFF
vertex   2048,   2150,   2048,  -4118,   5076,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -716,   1843,   -716,   1398,   -440,  0xFF, 0xFF, 0xFF, 0xFF
vertex  -2047,   2150,  -2047,   4056,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -101,   1843,      0,    172,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,   1843,    102,      0,   1192,  0xFF, 0xFF, 0xFF, 0xFF
vertex    717,   1843,   -716,  -1460,   -440,  0xFF, 0xFF, 0xFF, 0xFF
vertex    102,   1843,      0,   -234,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   2048,   2150,  -2047,  -4118,  -3098,  0xFF, 0xFF, 0xFF, 0xFF
vertex      0,   1843,   -101,      0,    786,  0xFF, 0xFF, 0xFF, 0xFF

thi_seg7_dl_07009EB0: # 0x07009EB0 - 0x07009F58
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, grass_09000000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex thi_seg7_vertex_07009DF0, 12, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSP2Triangles  4,  0,  5, 0x0,  4,  1,  0, 0x0
gsSP2Triangles  4,  6,  1, 0x0,  6,  7,  1, 0x0
gsSP2Triangles  1,  7,  2, 0x0,  8,  2,  9, 0x0
gsSP2Triangles  7,  9,  2, 0x0, 10,  3,  2, 0x0
gsSP2Triangles 10,  2,  8, 0x0, 10,  4,  5, 0x0
gsSP2Triangles 10,  8,  4, 0x0,  4,  8, 11, 0x0
gsSP2Triangles  8,  9, 11, 0x0,  4, 11,  6, 0x0
gsSPEndDisplayList

glabel thi_seg7_dl_07009F58 # 0x07009F58 - 0x07009FC8
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_TEXEL0
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList thi_seg7_dl_07009EB0
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING | G_CULL_BACK
gsSPEndDisplayList

inside_castle_seg7_vertex_07034E50: # 0x07034E50 - 0x07034F40
vertex     88,   -101,  -2692,  -1048,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    -14,      0,  -2539,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex     88,      0,  -2692,  -1048,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,      0,  -2661,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,   -101,  -2539,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,   -101,  -2661,    990,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,      0,  -2539,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,   -204,  -3685,  10084,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,   -101,  -2661,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,   -306,  -3685,  10080,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,      0,  -2661,    -28,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,      0,  -2539,   6612,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    -14,      0,  -2539,   4568,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    -14,   -101,  -2539,   4568,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -250,   -101,  -2539,   6612,    990,  0xFF, 0xFF, 0xFF, 0xFF

inside_castle_seg7_vertex_07034F40: # 0x07034F40 - 0x07035030
vertex    763,   -101,  -2661,    990,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    762,      0,  -2539,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    763,      0,  -2661,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex     88,   -101,  -2692,  -1048,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    -14,   -101,  -2539,    990,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    -14,      0,  -2539,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex     88,      0,  -2692,   3546,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    424,   -101,  -2692,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex     88,   -101,  -2692,   3546,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    424,      0,  -2692,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    424,      0,  -2692,    990,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    526,      0,  -2539,  -1048,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    526,   -101,  -2539,  -1048,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    424,   -101,  -2692,    990,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    762,   -101,  -2539,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF

inside_castle_seg7_vertex_07035030: # 0x07035030 - 0x070350B0
vertex    526,      0,  -2539,  -1052,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    762,   -101,  -2539,  -3096,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    526,   -101,  -2539,  -1052,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    762,      0,  -2539,  -3096,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    763,   -306,  -3685,  10080,    990,  0xFF, 0xFF, 0xFF, 0xFF
vertex    763,      0,  -2661,    -28,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    763,   -204,  -3685,  10084,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex    763,   -101,  -2661,      0,    990,  0xFF, 0xFF, 0xFF, 0xFF

inside_castle_seg7_dl_070350B0: # 0x070350B0 - 0x07035178
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, inside_09008800
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex inside_castle_seg7_vertex_07034E50, 15, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  3,  6,  4, 0x0,  7,  8,  9, 0x0
gsSP2Triangles  7, 10,  8, 0x0, 11, 12, 13, 0x0
gsSP1Triangle 11, 13, 14, 0x0
gsSPVertex inside_castle_seg7_vertex_07034F40, 15, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  6,  7,  8, 0x0,  6,  9,  7, 0x0
gsSP2Triangles 10, 11, 12, 0x0, 10, 12, 13, 0x0
gsSP1Triangle  0, 14,  1, 0x0
gsSPVertex inside_castle_seg7_vertex_07035030, 8, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  3,  1, 0x0
gsSP2Triangles  4,  5,  6, 0x0,  4,  7,  5, 0x0
gsSPEndDisplayList

glabel inside_castle_seg7_dl_07035178 # 0x07035178 - 0x070351E8
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_TEXEL0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_TEXEL0
gsSPClearGeometryMode G_LIGHTING | G_CULL_BACK
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList inside_castle_seg7_dl_070350B0
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING | G_CULL_BACK
gsSPEndDisplayList

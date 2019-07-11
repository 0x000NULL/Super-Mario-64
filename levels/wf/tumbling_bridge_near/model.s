wf_seg7_light_0700DE98: # 0x0700DE98 - 0x0700DEA0
.byte 0x66, 0x66, 0x66, 0x00, 0x66, 0x66, 0x66, 0x00

wf_seg7_light_0700DEA0: # 0x0700DEA0 - 0x0700DEB0
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

wf_seg7_vertex_0700DEB0: # 0x0700DEB0 - 0x0700DFA0
vertex   -127,    -63,     64,    990,   -542,  0x00, 0x81, 0x00, 0xFF
vertex    215,    -63,    -63,      0,      0,  0x00, 0x81, 0x00, 0xFF
vertex    215,    -63,     64,      0,   -542,  0x00, 0x81, 0x00, 0xFF
vertex    215,    -63,     64,   5078,    990,  0x7F, 0x00, 0x00, 0xFF
vertex    215,    -63,    -63,   6100,    990,  0x7F, 0x00, 0x00, 0xFF
vertex    215,     64,    -63,   6100,    480,  0x7F, 0x00, 0x00, 0xFF
vertex    215,     64,     64,   5078,    480,  0x7F, 0x00, 0x00, 0xFF
vertex    215,     64,    -63,    880,    990,  0x00, 0x00, 0x81, 0xFF
vertex    215,    -63,    -63,    880,      0,  0x00, 0x00, 0x81, 0xFF
vertex   -127,    -63,    -63,      0,      0,  0x00, 0x00, 0x81, 0xFF
vertex   -127,     64,    -63,      0,    990,  0x00, 0x00, 0x81, 0xFF
vertex   -127,     64,    -63,    990,      0,  0x00, 0x7F, 0x00, 0xFF
vertex   -127,     64,     64,    990,   -542,  0x00, 0x7F, 0x00, 0xFF
vertex    215,     64,     64,      0,   -544,  0x00, 0x7F, 0x00, 0xFF
vertex    215,     64,    -63,      0,      0,  0x00, 0x7F, 0x00, 0xFF

wf_seg7_vertex_0700DFA0: # 0x0700DFA0 - 0x0700E050
vertex   -127,     64,     64,      0,    990,  0x00, 0x00, 0x7F, 0xFF
vertex    215,    -63,     64,    880,      0,  0x00, 0x00, 0x7F, 0xFF
vertex    215,     64,     64,    880,    990,  0x00, 0x00, 0x7F, 0xFF
vertex   -127,    -63,     64,    990,   -542,  0x00, 0x81, 0x00, 0xFF
vertex   -127,    -63,    -63,    990,      0,  0x00, 0x81, 0x00, 0xFF
vertex    215,    -63,    -63,      0,      0,  0x00, 0x81, 0x00, 0xFF
vertex   -127,    -63,    -63,   3034,    990,  0x81, 0x00, 0x00, 0xFF
vertex   -127,    -63,     64,   4056,    990,  0x81, 0x00, 0x00, 0xFF
vertex   -127,     64,     64,   4056,    480,  0x81, 0x00, 0x00, 0xFF
vertex   -127,     64,    -63,   3034,    480,  0x81, 0x00, 0x00, 0xFF
vertex   -127,    -63,     64,      0,      0,  0x00, 0x00, 0x7F, 0xFF

wf_seg7_dl_0700E050: # 0x0700E050 - 0x0700E0F0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, grass_09004000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPLight wf_seg7_light_0700DEA0, 1
gsSPLight wf_seg7_light_0700DE98, 2
gsSPVertex wf_seg7_vertex_0700DEB0, 15, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  3,  5,  6, 0x0,  7,  8,  9, 0x0
gsSP2Triangles  7,  9, 10, 0x0, 11, 12, 13, 0x0
gsSP1Triangle 11, 13, 14, 0x0
gsSPVertex wf_seg7_vertex_0700DFA0, 11, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  6,  7,  8, 0x0,  6,  8,  9, 0x0
gsSP1Triangle  0, 10,  1, 0x0
gsSPEndDisplayList

glabel wf_seg7_dl_0700E0F0 # 0x0700E0F0 - 0x0700E160
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPClearGeometryMode G_SHADING_SMOOTH
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList wf_seg7_dl_0700E050
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_SHADING_SMOOTH
gsSPEndDisplayList

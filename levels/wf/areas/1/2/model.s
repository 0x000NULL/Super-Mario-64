wf_seg7_light_070051C8: # 0x070051C8 - 0x070051D0
.byte 0x66, 0x66, 0x66, 0x00, 0x66, 0x66, 0x66, 0x00

wf_seg7_light_070051D0: # 0x070051D0 - 0x070051E0
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

wf_seg7_vertex_070051E0: # 0x070051E0 - 0x07005260
vertex    781,   2560,   1728,    990,      0,  0x00, 0x7F, 0x00, 0xFF
vertex    576,   2560,   1728,      0,      0,  0x00, 0x7F, 0x00, 0xFF
vertex    576,   2560,   1933,      0,    990,  0x00, 0x7F, 0x00, 0xFF
vertex    781,   2560,   1933,    990,    990,  0x00, 0x7F, 0x00, 0xFF
vertex   1925,   2560,   -204,    990,    990,  0x00, 0x7F, 0x00, 0xFF
vertex   1720,   2560,      0,      0,      0,  0x00, 0x7F, 0x00, 0xFF
vertex   1925,   2560,      0,      0,    990,  0x00, 0x7F, 0x00, 0xFF
vertex   1720,   2560,   -204,    990,      0,  0x00, 0x7F, 0x00, 0xFF

wf_seg7_dl_07005260: # 0x07005260 - 0x070052B8
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, wf_seg7_texture_07000800
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPLight wf_seg7_light_070051D0, 1
gsSPLight wf_seg7_light_070051C8, 2
gsSPVertex wf_seg7_vertex_070051E0, 8, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSP2Triangles  4,  5,  6, 0x0,  4,  7,  5, 0x0
gsSPEndDisplayList

glabel wf_seg7_dl_070052B8 # 0x070052B8 - 0x07005328
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPClearGeometryMode G_SHADING_SMOOTH
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList wf_seg7_dl_07005260
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_SHADING_SMOOTH
gsSPEndDisplayList

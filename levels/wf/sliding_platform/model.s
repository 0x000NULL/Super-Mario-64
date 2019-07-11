wf_seg7_light_0700EA28: # 0x0700EA28 - 0x0700EA30
.byte 0x66, 0x66, 0x66, 0x00, 0x66, 0x66, 0x66, 0x00

wf_seg7_light_0700EA30: # 0x0700EA30 - 0x0700EA40
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

wf_seg7_vertex_0700EA40: # 0x0700EA40 - 0x0700EA80
vertex    256,      0,    256,      0,   -236,  0x7F, 0x00, 0x00, 0xFF
vertex    256,    -50,    256,      0,      0,  0x7F, 0x00, 0x00, 0xFF
vertex    256,    -50,   -255,   2012,      0,  0x7F, 0x00, 0x00, 0xFF
vertex    256,      0,   -255,   2012,   -236,  0x7F, 0x00, 0x00, 0xFF

wf_seg7_vertex_0700EA80: # 0x0700EA80 - 0x0700EAC0
vertex    256,      0,    256,      0,    990,  0x00, 0x7F, 0x00, 0xFF
vertex   -255,      0,   -255,   2012,  -1054,  0x00, 0x7F, 0x00, 0xFF
vertex   -255,      0,    256,      0,  -1054,  0x00, 0x7F, 0x00, 0xFF
vertex    256,      0,   -255,   2012,    990,  0x00, 0x7F, 0x00, 0xFF

wf_seg7_dl_0700EAC0: # 0x0700EAC0 - 0x0700EB08
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, grass_09000800
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPLight wf_seg7_light_0700EA30, 1
gsSPLight wf_seg7_light_0700EA28, 2
gsSPVertex wf_seg7_vertex_0700EA40, 4, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSPEndDisplayList

wf_seg7_dl_0700EB08: # 0x0700EB08 - 0x0700EB40
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, grass_09001000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex wf_seg7_vertex_0700EA80, 4, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  3,  1, 0x0
gsSPEndDisplayList

glabel wf_seg7_dl_0700EB40 # 0x0700EB40 - 0x0700EBB8
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPClearGeometryMode G_SHADING_SMOOTH
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList wf_seg7_dl_0700EAC0
gsSPDisplayList wf_seg7_dl_0700EB08
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_SHADING_SMOOTH
gsSPEndDisplayList

jrb_seg7_light_07007EA0: # 0x07007EA0 - 0x07007EA8
.byte 0x7F, 0x7F, 0x7F, 0x00, 0x7F, 0x7F, 0x7F, 0x00

jrb_seg7_light_07007EA8: # 0x07007EA8 - 0x07007EB8
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

jrb_seg7_vertex_07007EB8: # 0x07007EB8 - 0x07007FB8
vertex    102,      0,     77,    990,    990,  0x33, 0xCD, 0x67, 0xFF
vertex    102,    154,     77,    990,      0,  0x54, 0x54, 0x2A, 0xFF
vertex   -101,    154,     77,      0,      0,  0xCD, 0x33, 0x67, 0xFF
vertex    102,      0,    -76,    990,    990,  0x54, 0xAC, 0xD6, 0xFF
vertex    102,    154,     77,      0,      0,  0x54, 0x54, 0x2A, 0xFF
vertex    102,      0,     77,      0,    990,  0x33, 0xCD, 0x67, 0xFF
vertex    102,    154,    -76,    990,      0,  0x33, 0x33, 0x99, 0xFF
vertex   -101,      0,    -76,    990,    990,  0xCD, 0xCD, 0x99, 0xFF
vertex    102,    154,    -76,      0,      0,  0x33, 0x33, 0x99, 0xFF
vertex    102,      0,    -76,      0,    990,  0x54, 0xAC, 0xD6, 0xFF
vertex   -101,    154,    -76,    990,      0,  0xAC, 0x54, 0xD6, 0xFF
vertex   -101,      0,     77,    990,    990,  0xAC, 0xAC, 0x2A, 0xFF
vertex   -101,    154,     77,    990,      0,  0xCD, 0x33, 0x67, 0xFF
vertex   -101,    154,    -76,      0,      0,  0xAC, 0x54, 0xD6, 0xFF
vertex   -101,      0,    -76,      0,    990,  0xCD, 0xCD, 0x99, 0xFF
vertex   -101,      0,     77,      0,    990,  0xAC, 0xAC, 0x2A, 0xFF

jrb_seg7_vertex_07007FB8: # 0x07007FB8 - 0x07008038
vertex   -101,      0,     77,      0,      0,  0xAC, 0xAC, 0x2A, 0xFF
vertex    102,      0,    -76,    990,    990,  0x54, 0xAC, 0xD6, 0xFF
vertex    102,      0,     77,    990,      0,  0x33, 0xCD, 0x67, 0xFF
vertex   -101,      0,    -76,      0,    990,  0xCD, 0xCD, 0x99, 0xFF
vertex    102,    154,     77,    990,    990,  0x54, 0x54, 0x2A, 0xFF
vertex    102,    154,    -76,    990,      0,  0x33, 0x33, 0x99, 0xFF
vertex   -101,    154,    -76,      0,      0,  0xAC, 0x54, 0xD6, 0xFF
vertex   -101,    154,     77,      0,    990,  0xCD, 0x33, 0x67, 0xFF

jrb_seg7_dl_07008038: # 0x07008038 - 0x070080B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, jrb_seg7_texture_07001800
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPLight jrb_seg7_light_07007EA8, 1
gsSPLight jrb_seg7_light_07007EA0, 2
gsSPVertex jrb_seg7_vertex_07007EB8, 16, 0
gsSP2Triangles  0,  1,  2, 0x0,  3,  4,  5, 0x0
gsSP2Triangles  3,  6,  4, 0x0,  7,  8,  9, 0x0
gsSP2Triangles  7, 10,  8, 0x0, 11, 12, 13, 0x0
gsSP2Triangles 11, 13, 14, 0x0,  0,  2, 15, 0x0
gsSPEndDisplayList

jrb_seg7_dl_070080B0: # 0x070080B0 - 0x070080F8
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, jrb_seg7_texture_07002000
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x3FF, 0x100
gsSPVertex jrb_seg7_vertex_07007FB8, 8, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  3,  1, 0x0
gsSP2Triangles  4,  5,  6, 0x0,  4,  6,  7, 0x0
gsSPEndDisplayList

glabel jrb_seg7_dl_070080F8 # 0x070080F8 - 0x070081A8
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 5, 80, 75, 255
gsSPFogPosition 900, 1000
gsSPSetGeometryMode G_FOG
gsDPSetCombineMode G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPDisplayList jrb_seg7_dl_07008038
gsSPDisplayList jrb_seg7_dl_070080B0
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsDPSetRenderMode G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2
gsSPClearGeometryMode G_FOG
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPEndDisplayList

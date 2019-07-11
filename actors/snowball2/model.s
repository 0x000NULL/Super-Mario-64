# Snowball (copy of the other snowball actor. TODO: What is this?)

snowball_seg4_vertex_04032700: # 0x04032700
vertex     -4,      0,      0,      0,    960,  0xFF, 0xFF, 0xFF, 0xFF
vertex      4,      0,      0,    960,    960,  0xFF, 0xFF, 0xFF, 0xFF
vertex      4,      8,      0,    960,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex     -4,      8,      0,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF

snowball_seg4_vertex_04032740: # 0x04032740
vertex     -4,      0,      0,      0,    960,  0xFF, 0x00, 0x00, 0xFF
vertex      4,      0,      0,    960,    960,  0xFF, 0x00, 0x00, 0xFF
vertex      4,      8,      0,    960,      0,  0xFF, 0x00, 0x00, 0xFF
vertex     -4,      8,      0,      0,      0,  0xFF, 0x00, 0x00, 0xFF

snowball_seg4_texture_04032780: # 0x04032780
.incbin "actors/snowball2/0.rgba16"

glabel snowball_seg4_dl_04032980 # 0x04032980 - 0x040329E0
gsDPPipeSync
gsSPClearGeometryMode G_LIGHTING
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_TEXEL0, G_ACMUX_0, G_ACMUX_SHADE, G_ACMUX_0
gsSPTexture -32768, -32768, 0, 0, 1
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowball_seg4_texture_04032780
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 4, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x0FF, 0x200
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 4, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 60, 60
gsSPEndDisplayList

glabel snowball_seg4_dl_040329E0 # 0x040329E0 - 0x04032A18
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsDPPipeSync
gsSPTexture 1, 1, 0, 0, 0
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING
gsSPEndDisplayList

glabel small_snow_ball_dl # 0x04032A18 - 0x04032A30
gsSPDisplayList snowball_seg4_dl_04032980
gsSPVertex snowball_seg4_vertex_04032700, 4, 0
gsSPBranchList snowball_seg4_dl_040329E0

glabel snowball_seg4_dl_04032A30 # 0x04032A30 - 0x04032A48
gsSPDisplayList snowball_seg4_dl_04032980
gsSPVertex snowball_seg4_vertex_04032740, 4, 0
gsSPBranchList snowball_seg4_dl_040329E0

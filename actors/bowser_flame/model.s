# Bowser Flame

# TODO: Are these seperate textures or unified 64x64 ones?

.balign 8
flame_seg6_texture_06000000: # 0x06000000
.incbin "actors/bowser_flame/0.rgba16"

flame_seg6_texture_06002000: # 0x06002000
.incbin "actors/bowser_flame/1.rgba16"

flame_seg6_texture_06004000: # 0x06004000
.incbin "actors/bowser_flame/2.rgba16"

flame_seg6_texture_06006000: # 0x06006000
.incbin "actors/bowser_flame/3.rgba16"

flame_seg6_texture_06008000: # 0x06008000
.incbin "actors/bowser_flame/4.rgba16"

flame_seg6_texture_0600A000: # 0x0600A000
.incbin "actors/bowser_flame/5.rgba16"

flame_seg6_texture_0600C000: # 0x0600C000
.incbin "actors/bowser_flame/6.rgba16"

flame_seg6_texture_0600E000: # 0x0600E000
.incbin "actors/bowser_flame/7.rgba16"

flame_seg6_texture_06010000: # 0x06010000
.incbin "actors/bowser_flame/8.rgba16"

flame_seg6_texture_06012000: # 0x06012000
.incbin "actors/bowser_flame/9.rgba16"

flame_seg6_texture_06014000: # 0x06014000
.incbin "actors/bowser_flame/10.rgba16"

flame_seg6_texture_06016000: # 0x06016000
.incbin "actors/bowser_flame/11.rgba16"

flame_seg6_texture_06018000: # 0x06018000
.incbin "actors/bowser_flame/12.rgba16"

flame_seg6_texture_0601A000: # 0x0601A000
.incbin "actors/bowser_flame/13.rgba16"

flame_seg6_vertex_0601C000: # 0x0601C000
vertex   -150,    150,      0,      0,    992,  0xFF, 0xFF, 0xFF, 0xFF
vertex    150,    150,      0,   2016,    992,  0xFF, 0xFF, 0xFF, 0xFF
vertex    150,    300,      0,   2016,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -150,    300,      0,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -150,      0,      0,      0,    992,  0xFF, 0xFF, 0xFF, 0xFF
vertex    150,      0,      0,   2016,    992,  0xFF, 0xFF, 0xFF, 0xFF
vertex    150,    150,      0,   2016,      0,  0xFF, 0xFF, 0xFF, 0xFF
vertex   -150,    150,      0,      0,      0,  0xFF, 0xFF, 0xFF, 0xFF

glabel flame_seg6_dl_0601C080 # 0x0601C080 - 0x0601C0B0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_TEXEL0, G_ACMUX_0, G_ACMUX_SHADE, G_ACMUX_0
gsSPGeometryMode G_LIGHTING, G_SHADING_SMOOTH, G_ORDER_CFIRST
gsSPTexture -1, -1, 0, 0, 1
gsSPEndDisplayList

glabel flame_seg6_dl_0601C0B0 # 0x0601C0B0 - 0x0601C0C8
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C0C8 # 0x0601C0C8 - 0x0601C0E0
gsSP2Triangles  4,  5,  6, 0x0,  4,  6,  7, 0x0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C0E0 # 0x0601C0E0 - 0x0601C108
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPSetGeometryMode G_LIGHTING
gsSPEndDisplayList

glabel flame_seg6_dl_0601C108 # 0x0601C108 - 0x0601C1A8
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06000000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06000000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C1A8 # 0x0601C1A8 - 0x0601C248
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06002000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06002000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C248 # 0x0601C248 - 0x0601C2E8
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06004000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06004000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C2E8 # 0x0601C2E8 - 0x0601C388
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06006000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06006000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C388 # 0x0601C388 - 0x0601C428
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06008000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06008000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C428 # 0x0601C428 - 0x0601C4C8
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0600A000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0600A000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C4C8 # 0x0601C4C8 - 0x0601C568
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0600C000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0600C000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C568 # 0x0601C568 - 0x0601C608
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0600E000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0600E000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C608 # 0x0601C608 - 0x0601C6A8
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06010000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06010000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C6A8 # 0x0601C6A8 - 0x0601C748
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06012000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06012000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C748 # 0x0601C748 - 0x0601C7E8
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06014000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06014000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C7E8 # 0x0601C7E8 - 0x0601C888
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06016000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06016000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C888 # 0x0601C888 - 0x0601C928
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06018000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_06018000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

glabel flame_seg6_dl_0601C928 # 0x0601C928 - 0x0601C9C8
gsSPDisplayList flame_seg6_dl_0601C080
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0601A000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPVertex flame_seg6_vertex_0601C000, 8, 0
gsSPDisplayList flame_seg6_dl_0601C0B0
gsDPSetTextureImage G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, flame_seg6_texture_0601A000 + 0x1000
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPLoadSync
gsDPLoadBlock 7, 0, 0, 0x7FF, 0x080
gsDPPipeSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP | G_TX_NOMIRROR, 6, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 252, 124
gsSPDisplayList flame_seg6_dl_0601C0C8
gsSPDisplayList flame_seg6_dl_0601C0E0
gsSPEndDisplayList

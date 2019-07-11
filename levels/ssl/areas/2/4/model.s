# Quicksand Pit

ssl_seg7_light_07004000: # 0x07004000 - 0x07004008
.byte 0x3F, 0x3F, 0x3F, 0x00, 0x3F, 0x3F, 0x3F, 0x00

ssl_seg7_light_07004008: # 0x07004008 - 0x07004018
.byte 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
.byte 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00

glabel ssl_seg7_texture_07004018 # 0x07004018 - 0x07004818
.incbin "levels/ssl/7.rgba16"

glabel ssl_seg7_dl_07004818 # 0x07004818 - 0x07004860
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPLight ssl_seg7_light_07004008, 1
gsSPLight ssl_seg7_light_07004000, 2
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPEndDisplayList

glabel ssl_seg7_dl_07004860 # 0x07004860 - 0x07004880
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPEndDisplayList

glabel ssl_seg7_dl_07004880 # 0x07004880 - 0x070048F8
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 0, 0, 0, 255
gsSPFogFactor 0x0E49, 0xF2B7 # This isn't gsSPFogPosition since there is no valid min/max pair that corresponds to 0x0E49F2B7
gsSPSetGeometryMode G_FOG
gsDPSetCombineMode G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPLight ssl_seg7_light_07004008, 1
gsSPLight ssl_seg7_light_07004000, 2
gsSPTexture -1, -1, 0, 0, 1
gsDPTileSync
gsDPSetTile G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD
gsDPSetTileSize 0, 0, 0, 124, 124
gsSPEndDisplayList

glabel ssl_seg7_dl_070048F8 # 0x070048F8 - 0x07004930
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsDPSetRenderMode G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2
gsSPClearGeometryMode G_FOG
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPEndDisplayList

glabel ssl_seg7_arr_07004930 # 0x07004930 - 0x07004AB4
.hword    10,     0,  -204,     0,     0,   127,     0,     0
.hword     0,  1024,     0,     0,     0,   127,     0,     2
.hword     0,   512,     0,  -886,     0,   127,     0,     2
.hword     1,  -511,     0,  -886,     0,   127,     0,     2
.hword     2, -1023,     0,     0,     0,   127,     0,     2
.hword     3,  -511,     0,   887,     0,   127,     0,     2
.hword     2,   512,     0,   887,     0,   127,     0,     2
.hword     1,  1024,     0,     0,     0,   127,     0,     2
.hword     0,     0

glabel ssl_seg7_arr_070049B4 # 0x070049B4 - 0x07004A38
.hword     5,     0,  -204,     0,     0,   127,     0,     0
.hword     0,  1024,     0,     0,     0,   127,     0,     2
.hword     0,   512,     0,  -886,     0,   127,     0,     2
.hword     1,  -511,     0,  -886,     0,   127,     0,     2
.hword     2, -1023,     0,     0,     0,   127,     0,     2
.hword     3,  -511,     0,   887,     0,   127,     0,     2
.hword     2,   512,     0,   887,     0,   127,     0,     2
.hword     1,  1024,     0,     0,     0,   127,     0,     2
.hword     0,     0

glabel ssl_seg7_dl_07004A38 # 0x07004A38 - 0x07004A70
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSP2Triangles  0,  3,  4, 0x0,  0,  4,  5, 0x0
gsSP2Triangles  0,  5,  6, 0x0,  0,  6,  7, 0x0
gsSPEndDisplayList

ssl_seg7_vertex_07004A70: # 0x07004A70 - 0x07004AE0
vertex      0,   -204,      0,      0,      0,  0x00, 0x7F, 0x00, 0xFF
vertex   1024,      0,      0,   2016,      0,  0x00, 0x7F, 0x00, 0xFF
vertex    512,      0,   -886,   2016,    992,  0x00, 0x7F, 0x00, 0xFF
vertex   -511,      0,   -886,   2016,   2016,  0x00, 0x7F, 0x00, 0xFF
vertex  -1023,      0,      0,   2016,   3040,  0x00, 0x7F, 0x00, 0xFF
vertex   -511,      0,    887,   2016,   2016,  0x00, 0x7F, 0x00, 0xFF
vertex    512,      0,    887,   2016,    992,  0x00, 0x7F, 0x00, 0xFF

glabel ssl_seg7_dl_07004AE0 # 0x07004AE0 - 0x07004BE0
gsDPPipeSync
gsDPSetCycleType G_CYC_2CYCLE
gsDPSetRenderMode G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2
gsDPSetDepthSource G_ZS_PIXEL
gsDPSetFogColor 0, 0, 0, 255
gsSPFogFactor 0x0E49, 0xF2B7 # This isn't gsSPFogPosition since there is no valid min/max pair that corresponds to 0x0E49F2B7
gsSPSetGeometryMode G_FOG
gsDPSetCombineMode G_CCMUX_TEXEL0, G_CCMUX_0, G_CCMUX_SHADE, G_CCMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE, G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_COMBINED, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_COMBINED
gsSPLight ssl_seg7_light_07004008, 1
gsSPLight ssl_seg7_light_07004000, 2
gsSPTexture -1, -1, 0, 0, 1
gsDPLoadTextureBlock ssl_seg7_texture_07001000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD
gsSPVertex ssl_seg7_vertex_07004A70, 7, 0
gsSP2Triangles  0,  1,  2, 0x0,  0,  2,  3, 0x0
gsSP2Triangles  0,  3,  4, 0x0,  0,  4,  5, 0x0
gsSP2Triangles  0,  5,  6, 0x0,  0,  6,  1, 0x0
gsSPTexture -1, -1, 0, 0, 0
gsDPPipeSync
gsDPSetCycleType G_CYC_1CYCLE
gsDPSetRenderMode G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2
gsSPClearGeometryMode G_FOG
gsDPSetCombineMode1Cycle G_CCMUX_0, G_CCMUX_0, G_CCMUX_0, G_CCMUX_SHADE, G_ACMUX_0, G_ACMUX_0, G_ACMUX_0, G_ACMUX_SHADE
gsSPEndDisplayList

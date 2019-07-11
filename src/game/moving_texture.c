#include <ultra64.h>

#include "sm64.h"
#include "moving_texture.h"
#include "area.h"
#include "camera.h"
#include "rendering_graph_node.h"
#include "engine/math_util.h"
#include "memory.h"
#include "save_file.h"
#include "segment2.h"
#include "engine/surface_collision.h"
#include "geo_misc.h"
#include "rendering_graph_node.h"
#include "room.h"

extern Gfx castle_grounds_seg7_dl_07010F30[];
extern s16 castle_grounds_seg7_arr_07010E98[];
extern u8 ssl_seg7_texture_07004018[];
extern u8 ssl_seg7_texture_07001000[];
extern u8 ttc_seg7_texture_07015F90[];
extern s16 ssl_seg7_arr_07028760[];
extern Gfx ssl_seg7_dl_070286A0[];
extern Gfx ssl_seg7_dl_07028718[];
extern Gfx ssl_seg7_dl_070287B8[];
extern s16 ssl_seg7_arr_070287F0[];
extern Gfx ssl_seg7_dl_070285F0[];
extern Gfx ssl_seg7_dl_07028660[];
extern s16 ssl_seg7_arr_07028844[];
extern Gfx ssl_seg7_dl_07028888[];
extern s16 bitfs_seg7_arr_07015AF0[];
extern Gfx bitfs_seg7_dl_07015BA8[];
extern s16 bitfs_seg7_arr_07015B1C[];
extern s16 bitfs_seg7_arr_07015B48[];
extern Gfx bitfs_seg7_dl_07015BC0[];
extern s16 lll_seg7_arr_0702874C[];
extern Gfx lll_seg7_dl_070287A8[];
extern s16 lll_seg7_arr_07028820[];
extern Gfx lll_seg7_dl_070288C8[];
extern s16 cotmc_seg7_arr_0700BED0[];
extern Gfx cotmc_seg7_dl_0700BE10[];
extern Gfx cotmc_seg7_dl_0700BE88[];
extern Gfx cotmc_seg7_dl_0700BF60[];
extern s16 ttm_seg7_arr_07017134[];
extern Gfx ttm_seg7_dl_07017260[];
extern s16 ttm_seg7_arr_070171A0[];
extern s16 ttm_seg7_arr_07017174[];
extern Gfx ttm_seg7_dl_07017288[];
extern s16 ttm_seg7_arr_070171E0[];
extern s16 ttm_seg7_arr_0701720C[];
extern Gfx ttm_seg7_dl_070172A0[];
extern s16 ssl_seg7_arr_070127F0[];
extern Gfx ssl_seg7_dl_070127E0[];
extern Gfx ssl_seg7_dl_070127E8[];
extern Gfx ssl_seg7_dl_070128B8[];
extern s16 ssl_seg7_arr_07012900[];
extern Gfx ssl_seg7_dl_07012A08[];
extern s16 ssl_seg7_arr_07012A50[];
extern Gfx ssl_seg7_dl_07012B48[];
extern s16 ttc_seg7_arr_07016840[];
extern Gfx ttc_seg7_dl_07016790[];
extern Gfx ttc_seg7_dl_07016808[];
extern Gfx ttc_seg7_dl_070169C8[];
extern s16 ttc_seg7_arr_07016904[];
extern s16 ssl_seg7_arr_07004930[];
extern Gfx ssl_seg7_dl_07004818[];
extern Gfx ssl_seg7_dl_07004860[];
extern Gfx ssl_seg7_dl_07004A38[];
extern s16 ssl_seg7_arr_070049B4[];
extern Gfx ssl_seg7_dl_07004880[];
extern Gfx ssl_seg7_dl_070048F8[];

struct Struct802D0DD4
{
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s16 *unkC;
    Gfx *unk10;
    Gfx *unk14;
    Gfx *unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    s32 unk20;
};

s16 D_8032FFD0 = 1;
s16 D_8032FFD4 = 0;
s8 D_8032FFD8 = 0;
float D_8032FFDC = 0.0f;
s32 D_8032FFE0 = 0;
u8 *D_8032FFE4[] =
{
    texture_waterbox_water,
    texture_waterbox_mist,
    texture_waterbox_jrb_water,
    texture_waterbox_unknown_water,
    texture_waterbox_lava,
    ssl_seg7_texture_07004018,
    ssl_seg7_texture_07001000,
    ttc_seg7_texture_07015F90
};

struct Struct802D0DD4 D_80330004[] =
{
    {0x00000801, 0x00000006, 0x00000008, ssl_seg7_arr_07028760, ssl_seg7_dl_070286A0, ssl_seg7_dl_07028718, ssl_seg7_dl_070287B8, 0xff, 0xff, 0xff, 0xff, 0x00000007},
    {0x00000802, 0x00000006, 0x00000008, ssl_seg7_arr_070287F0, ssl_seg7_dl_070285F0, ssl_seg7_dl_07028660, ssl_seg7_dl_070287B8, 0xff, 0xff, 0xff, 0xff, 0x00000003},
    {0x00000803, 0x00000006, 0x00000006, ssl_seg7_arr_07028844, ssl_seg7_dl_070286A0, ssl_seg7_dl_07028718, ssl_seg7_dl_07028888, 0xff, 0xff, 0xff, 0xff, 0x00000007},
    {0x00001601, 0x00000000, 0x0000000f, castle_grounds_seg7_arr_07010E98, dl_waterbox_rgba16_begin, dl_waterbox_end, castle_grounds_seg7_dl_07010F30, 0xff, 0xff, 0xff, 0xb4, 0x00000007},
    {0x00001901, 0x00000004, 0x00000004, bitfs_seg7_arr_07015AF0, dl_waterbox_rgba16_begin, dl_waterbox_end, bitfs_seg7_dl_07015BA8, 0xff, 0xff, 0xff, 0xff, 0x00000001},
    {0x00001902, 0x00000004, 0x00000004, bitfs_seg7_arr_07015B1C, dl_waterbox_rgba16_begin, dl_waterbox_end, bitfs_seg7_dl_07015BA8, 0xff, 0xff, 0xff, 0xb4, 0x00000005},
    {0x00001903, 0x00000004, 0x00000009, bitfs_seg7_arr_07015B48, dl_waterbox_rgba16_begin, dl_waterbox_end, bitfs_seg7_dl_07015BC0, 0xff, 0xff, 0xff, 0xb4, 0x00000005},
    {0x00002201, 0x00000004, 0x00000009, lll_seg7_arr_0702874C, dl_waterbox_rgba16_begin, dl_waterbox_end, lll_seg7_dl_070287A8, 0xff, 0xff, 0xff, 0xc8, 0x00000005},
    {0x00002202, 0x00000004, 0x00000010, lll_seg7_arr_07028820, dl_waterbox_rgba16_begin, dl_waterbox_end, lll_seg7_dl_070288C8, 0xff, 0xff, 0xff, 0xb4, 0x00000007},
    {0x00002801, 0x00000000, 0x0000000e, cotmc_seg7_arr_0700BED0, cotmc_seg7_dl_0700BE10, cotmc_seg7_dl_0700BE88, cotmc_seg7_dl_0700BF60, 0xff, 0xff, 0xff, 0xb4, 0x00000007},
    {0x00003601, 0x00000000, 0x00000006, ttm_seg7_arr_07017134, dl_waterbox_rgba16_begin, dl_waterbox_end, ttm_seg7_dl_07017260, 0xff, 0xff, 0xff, 0xb4, 0x00000005},
    {0x00003602, 0x00000000, 0x00000006, ttm_seg7_arr_070171A0, dl_waterbox_rgba16_begin, dl_waterbox_end, ttm_seg7_dl_07017260, 0xff, 0xff, 0xff, 0xb4, 0x00000005},
    {0x00003603, 0x00000000, 0x00000004, ttm_seg7_arr_07017174, dl_waterbox_rgba16_begin, dl_waterbox_end, ttm_seg7_dl_07017288, 0xff, 0xff, 0xff, 0xb4, 0x00000007},
    {0x00003604, 0x00000000, 0x00000004, ttm_seg7_arr_070171E0, dl_waterbox_rgba16_begin, dl_waterbox_end, ttm_seg7_dl_07017288, 0xff, 0xff, 0xff, 0xb4, 0x00000007},
    {0x00003605, 0x00000000, 0x00000008, ttm_seg7_arr_0701720C, dl_waterbox_rgba16_begin, dl_waterbox_end, ttm_seg7_dl_070172A0, 0xff, 0xff, 0xff, 0xb4, 0x00000007},
    {0x00000000, 0x00000000, 0x00000000, NULL,       NULL,             NULL,             NULL,       0x00, 0x00, 0x00, 0x00, 0x00000000},
};

struct Struct802D0DD4 D_80330244[] =
{
    {0x00000801, 0x00000005, 0x0000000C, ssl_seg7_arr_070127F0, ssl_seg7_dl_070127E0, ssl_seg7_dl_070127E8, ssl_seg7_dl_070128B8, 0xff, 0xff, 0xff, 0xff, 0x00000001},
    {0x00000802, 0x00000005, 0x00000010, ssl_seg7_arr_07012900, ssl_seg7_dl_070127E0, ssl_seg7_dl_070127E8, ssl_seg7_dl_07012A08, 0xff, 0xff, 0xff, 0xff, 0x00000001}, 
    {0x00000803, 0x00000005, 0x0000000F, ssl_seg7_arr_07012A50, ssl_seg7_dl_070127E0, ssl_seg7_dl_070127E8, ssl_seg7_dl_07012B48, 0xff, 0xff, 0xff, 0xff, 0x00000001}, 
    {0x00001400, 0x00000007, 0x0000000C, ttc_seg7_arr_07016840, ttc_seg7_dl_07016790, ttc_seg7_dl_07016808, ttc_seg7_dl_070169C8, 0xff, 0xff, 0xff, 0xff, 0x00000001}, 
    {0x00001401, 0x00000007, 0x0000000C, ttc_seg7_arr_07016904, ttc_seg7_dl_07016790, ttc_seg7_dl_07016808, ttc_seg7_dl_070169C8, 0xff, 0xff, 0xff, 0xff, 0x00000001}, 
    {0x00000000, 0x00000000, 0x00000000, NULL,       NULL,       NULL,       NULL,       0x00, 0x00, 0x00, 0x00, 0x00000000},
};

struct Struct802D0DD4 D_8033031C[] =
{
    {0x00000801, 0x00000005, 0x00000008, ssl_seg7_arr_07004930, ssl_seg7_dl_07004818, ssl_seg7_dl_07004860, ssl_seg7_dl_07004A38, 0xff, 0xff, 0xff, 0xff, 0x00000001}, 
    {0x00000802, 0x00000006, 0x00000008, ssl_seg7_arr_070049B4, ssl_seg7_dl_07004880, ssl_seg7_dl_070048F8, ssl_seg7_dl_07004A38, 0xff, 0xff, 0xff, 0xff, 0x00000001}, 
    {0x00000000, 0x00000000, 0x00000000, NULL,       NULL,       NULL,       NULL,       0x00, 0x00, 0x00, 0x00, 0x00000000},
};

Gfx *Geo18_802D0080(s32 run, UNUSED struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    s32 spC;
    UNUSED u8 unused[] = {1, 0, 4, 0, 7, 0, 10, 0};
    s16 sp2;

    if (run != TRUE)
    {
        D_8032FFE0 = 0;
    }
    else if (run == TRUE && gWaterRegions != NULL && D_8032FFE0 == 0)
    {
        if (D_8032FFDC <= 1382.4)
            sp2 = 31;
        else if (D_8032FFDC >= 1600.0)
            sp2 = 2816;
        else
            sp2 = 1024;
        for (spC = 0; spC < *gWaterRegions; spC++)
        {
            gWaterRegions[spC * 6 + 6] = sp2;
        }
        D_8032FFE0 = 1;
    }
    return NULL;
}

Gfx *Geo18_802D01E0(s32 run, UNUSED struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    if (run != TRUE)
    {
        D_8032FFD4 = gAreaUpdateCounter - 1;
        D_8032FFD0 = gAreaUpdateCounter;
    }
    else
    {
        D_8032FFD4 = D_8032FFD0;
        D_8032FFD0 = gAreaUpdateCounter;
    }
    return NULL;
}

void func_802CF774(Vtx *verts, s32 n, s16 x, s16 y, s16 z, s16 f, s16 g,
    f32 h, u8 alpha)
{
    s16 tx = 32.0 * (32.0 * h - 1.0) * sins(f + g);
    s16 ty = 32.0 * (32.0 * h - 1.0) * coss(f + g);
    
    if (D_8032FFD8 == 1)
        make_vertex(verts, n, x, y, z, tx, ty, 255, 255, 0, alpha);
    else if (D_8032FFD8 == 2)
        make_vertex(verts, n, x, y, z, tx, ty, 255, 0, 0, alpha);
    else
        make_vertex(verts, n, x, y, z, tx, ty, 255, 255, 255, alpha);
}

struct Struct802CF9A4
{
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
};

s16 D_8035FF70;

Gfx *func_802CF9A4(s16 y, struct Struct802CF9A4 *b)
{
    s16 sp8E;
    s16 sp8C = b->unk2;
    s16 sp8A = b->unk4;
    s16 x1 = b->unk6;
    s16 z1 = b->unk8;
    s16 x2 = b->unkA;
    s16 z2 = b->unkC;
    s16 x3 = b->unkE;
    s16 z3 = b->unk10;
    s16 x4 = b->unk12;
    s16 z4 = b->unk14;
    s16 sp78 = b->unk16;
    s16 alpha = b->unk18;
    s16 sp74 = b->unk1A;
    Vtx *verts = alloc_display_list(4 * sizeof(*verts));
    Gfx *sp6C;
    Gfx *sp68;

    if (sp74 == D_8035FF70)
        sp6C = alloc_display_list(3 * sizeof(*sp6C));
    else
        sp6C = alloc_display_list(8 * sizeof(*sp6C));
    if (sp6C == NULL || verts == NULL)
        return NULL;
    sp68 = sp6C;
    if (D_8032FFD0 != D_8032FFD4)
        b->unk0 += sp8C;
    sp8E = b->unk0;
    if (sp78 == 0)
    {
        func_802CF774(verts, 0, x1, y, z1, sp8E, 0, sp8A, alpha);
        func_802CF774(verts, 1, x2, y, z2, sp8E, 16384, sp8A, alpha);
        func_802CF774(verts, 2, x3, y, z3, sp8E, -32768, sp8A, alpha);
        func_802CF774(verts, 3, x4, y, z4, sp8E, -16384, sp8A, alpha);
    }
    else
    {
        func_802CF774(verts, 0, x1, y, z1, sp8E, 0, sp8A, alpha);
        func_802CF774(verts, 1, x2, y, z2, sp8E, -16384, sp8A, alpha);
        func_802CF774(verts, 2, x3, y, z3, sp8E, -32768, sp8A, alpha);
        func_802CF774(verts, 3, x4, y, z4, sp8E, 16384, sp8A, alpha);
    }
    if (sp74 != D_8035FF70)
    {
        if (sp74 == 1)
        {
            if (0)
            {
            }
            gDPSetTextureImage(sp68++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, D_8032FFE4[sp74])
            gDPTileSync(sp68++)
            gDPSetTile(sp68++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, 7, 0,
                G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD)
            gDPLoadSync(sp68++)
            gDPLoadBlock(sp68++, 7, 0, 0, 1023, 256)
        }
        else
        {
            gDPSetTextureImage(sp68++, 0, 2, 1, D_8032FFE4[sp74])
            gDPTileSync(sp68++)
            gDPSetTile(sp68++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0,
                G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD)
            gDPLoadSync(sp68++)
            gDPLoadBlock(sp68++, 7, 0, 0, 1023, 256)
            if (0)
            {
            }
        }
        D_8035FF70 = sp74;
    }
    gSPVertex(sp68++, 0x80000000 + (u32)verts, 4, 0)
    gSPDisplayList(sp68++, dl_draw_quad_verts_0123)
    gSPEndDisplayList(sp68)
    return sp6C;
}

Gfx *func_802CFFA4(s16 a, void *b)
{
    s16 *sp34 = segmented_to_virtual(b);
    s16 sp32 = sp34[0];
    Gfx *sp2C = alloc_display_list((sp32 + 1) * sizeof(*sp2C));
    Gfx *sp28 = sp2C;
    Gfx *sp24;
    s32 sp20;

    if (sp2C == NULL)
        return NULL;
    for (sp20 = 0; sp20 < sp32; sp20++)
    {
        sp24 = func_802CF9A4(a, (struct Struct802CF9A4 *)(&sp34[sp20 * 14 + 1]));
        if (sp24 != NULL)
            gSPDisplayList(sp28++, VIRTUAL_TO_PHYSICAL(sp24))
    }
    gSPEndDisplayList(sp28);
    return sp2C;
}

struct Struct802D00D0
{
    s16 unk0;
    u8 filler2[2];
    void *unk4;
};

Gfx *func_802D00D0(s16 a, s16 b, void *c)
{
    struct Struct802D00D0 *sp1C = segmented_to_virtual(c);
    s32 sp18 = 0;
    
    while (sp1C[sp18].unk0 != -1)
    {
        if (sp1C[sp18].unk0 == a)
            return func_802CFFA4(b, sp1C[sp18].unk4);
        sp18++;
    }
    return NULL;
}

extern u8 bbh_seg7_moving_texture_07026E24[];
extern u8 bbh_seg7_moving_texture_07026E34[];
extern u8 ccm_seg7_moving_texture[];
extern u8 inside_castle_seg7_moving_texture_070790F0[];
extern u8 inside_castle_seg7_moving_texture_07079100[];
extern u8 hmc_seg7_moving_texture_0702B900[];
extern u8 hmc_seg7_moving_texture_0702B950[];
extern u8 ssl_seg7_moving_texture_07012778[];
extern u8 ssl_seg7_moving_texture_070127C8[];
extern u8 sl_seg7_moving_texture_0700FA70[];
extern u8 wdw_seg7_moving_texture_07018748[];
extern u8 wdw_seg7_moving_texture_07018778[];
extern u8 jrb_seg7_moving_texture_0700D2CC[];
extern u8 jrb_seg7_moving_texture_0700D304[];
extern u8 jrb_seg7_moving_texture_0701139C[];
extern u8 thi_seg7_moving_texture_0700E31C[];
extern u8 thi_seg7_moving_texture_0700E39C[];
extern u8 castle_grounds_seg7_moving_texture[];
extern u8 lll_seg7_moving_texture_07028810[];
extern u8 ddd_seg7_moving_texture_0700FCB4[];
extern u8 ddd_seg7_moving_texture_0700FD00[];
extern u8 wf_seg7_moving_texture[];
extern u8 castle_courtyard_seg7_moving_texture_07006E6C[];
extern u8 ttm_seg7_moving_texture_07017124[];

void *func_802D01A4(u32 a)
{
    switch (a)
    {
    case 1024: return bbh_seg7_moving_texture_07026E24;
    case 1025: return bbh_seg7_moving_texture_07026E34;
    case 1281: return ccm_seg7_moving_texture;
    case 1536: return inside_castle_seg7_moving_texture_070790F0;
    case 1554: return inside_castle_seg7_moving_texture_07079100;
    case 1793: return hmc_seg7_moving_texture_0702B900;
    case 1794: return hmc_seg7_moving_texture_0702B950;
    case 2049: return ssl_seg7_moving_texture_07012778;
    case 2129: return ssl_seg7_moving_texture_070127C8;
    case 4097: return sl_seg7_moving_texture_0700FA70;
    case 4353: return wdw_seg7_moving_texture_07018748;
    case 4354: return wdw_seg7_moving_texture_07018778;
    case 4609: return jrb_seg7_moving_texture_0700D2CC;
    case 4613: return jrb_seg7_moving_texture_0700D304;
    case 4610: return jrb_seg7_moving_texture_0701139C;
    case 4865: return thi_seg7_moving_texture_0700E31C;
    case 4866: return thi_seg7_moving_texture_0700E39C;
    case 5633: return castle_grounds_seg7_moving_texture;
    case 8706: return lll_seg7_moving_texture_07028810;
    case 8961: return ddd_seg7_moving_texture_0700FCB4;
    case 8962: return ddd_seg7_moving_texture_0700FD00;
    case 9217: return wf_seg7_moving_texture;
    case 9729: return castle_courtyard_seg7_moving_texture_07006E6C;
    case 13825: return ttm_seg7_moving_texture_07017124;
    default: return NULL;
    }
}

void func_802D0448(u32 a, Gfx **b)
{
    switch (a)
    {
    case 1794:
        gSPDisplayList((*b)++, dl_waterbox_ia16_begin)
        break;
    case 2129:
        gSPDisplayList((*b)++, dl_waterbox_ia16_begin)
        break;
    case 4613:
        gSPDisplayList((*b)++, dl_waterbox_ia16_begin)
        break;
    default:
        gSPDisplayList((*b)++, dl_waterbox_rgba16_begin)
        break;
    }
}

struct Struct802D104C
{
    u8 filler0[2];
    s16 unk2;
    u8 filler4[0x18-0x4];
    u32 unk18;
};

Gfx *Geo18_802D104C(s32 run, struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    Gfx *sp44 = NULL;
    Gfx *sp40 = NULL;
    Gfx *sp3C;
    void *sp38;
    struct GraphNode12A *sp34;
    s16 sp32;
    s16 sp30;
    s16 sp2E;
    s32 sp28;

    if (run == TRUE)
    {
        D_8032FFD8 = 0;
        if (gWaterRegions == NULL)
            return NULL;
        sp32 = gWaterRegions[0];
        sp44 = alloc_display_list((sp32 + 3) * sizeof(*sp44));
        if (sp44 == NULL)
            return NULL;
        else
            sp40 = sp44;
        sp34 = (struct GraphNode12A *)node;
        if (sp34->unk18 == 4613)
        {
            if (gCameraStatus.camFocAndPosCurrAndGoal[3][1] < 1024.0)
                return NULL;
            if (save_file_get_star_flags(gCurrSaveFileNum - 1, 2) & 1)
                return NULL;
        }
        else if (sp34->unk18 == 1794)
        {
            D_8032FFD8 = 1;
        }
        else if (sp34->unk18 == 2129)
        {
            D_8032FFD8 = 2;
        }
        sp38 = func_802D01A4(sp34->unk18);
        if (sp38 == NULL)
            return NULL;
        sp34->fnNode.node.flags = (sp34->fnNode.node.flags & 0xFF) | 0x700;
        func_802D0448(sp34->unk18, &sp40);
        D_8035FF70 = -1;
        for (sp28 = 0; sp28 < sp32; sp28++)
        {
            sp30 = gWaterRegions[sp28 * 6 + 1];
            sp2E = gWaterRegions[sp28 * 6 + 6];
            sp3C = func_802D00D0(sp30, sp2E, sp38);
            if (sp3C != NULL)
                gSPDisplayList(sp40++, VIRTUAL_TO_PHYSICAL(sp3C));
        }
        gSPDisplayList(sp40++, dl_waterbox_end);
        gSPEndDisplayList(sp40);
    }
    return sp44;
}

void func_802D0850(s16 *a, s32 b)
{
    s16 sp6 = a[0];
    s16 *sp0 = a + b;

    if (D_8032FFD0 != D_8032FFD4)
    {
        *sp0 += sp6;
        if (*sp0 >= 0x400)
            *sp0 -= 0x400;
        if (*sp0 <= -0x400)
            *sp0 += 0x400;
    }
}

void func_802D08EC(Vtx *vtx, s16 *b, struct Struct802D0DD4 *c, s8 d)
{
    s16 x = b[1];
    s16 y = b[2];
    s16 z = b[3];
    u8 alpha = c->unk1F;
    u8 r1;
    u8 g1;
    u8 b1;
    s8 r2;
    s8 g2;
    s8 b2;
    s16 tx;
    s16 ty;

    switch (d)
    {
    case 0:
        r1 = c->unk1C;
        g1 = c->unk1D;
        b1 = c->unk1E;
        tx = b[4];
        ty = b[5];
        make_vertex(vtx, 0, x, y, z, tx, ty, r1, g1, b1, alpha);
        break;
    case 1:
        r2 = b[4];
        g2 = b[5];
        b2 = b[6];
        tx = b[7];
        ty = b[8];
        make_vertex(vtx, 0, x, y, z, tx, ty, r2, g2, b2, alpha);
        break;
    }
}

void func_802D0A94(Vtx *verts, s32 n, s16 *c, struct Struct802D0DD4 *d, s8 e)
{
    u8 alpha = d->unk1F;
    s16 x;
    s16 y;
    s16 z;
    s16 sp56;
    s16 sp54;
    s16 tx;
    s16 ty;
    s16 sp4E;
    s16 sp4C;
    u8 r1;
    u8 g1;
    u8 b1;
    s8 r2;
    s8 g2;
    s8 b2;
    
    switch (e)
    {
    case 0:
        x = c[n * 5 + 1];
        y = c[n * 5 + 2];
        z = c[n * 5 + 3];
        sp56 = c[4];
        sp54 = c[5];
        sp4E = c[n * 5 + 4];
        sp4C = c[n * 5 + 5];
        tx = sp56 + sp4E * 32 * 32;
        ty = sp54 + sp4C * 32 * 32;
        r1 = d->unk1C;
        g1 = d->unk1D;
        b1 = d->unk1E;
        make_vertex(verts, n, x, y, z, tx, ty, r1, g1, b1, alpha);
        break;
    case 1:
        x = c[n * 8 + 1];
        y = c[n * 8 + 2];
        z = c[n * 8 + 3];
        sp56 = c[7];
        sp54 = c[8];
        sp4E = c[n * 8 + 7];
        sp4C = c[n * 8 + 8];
        tx = sp56 + sp4E * 32 * 32;
        ty = sp54 + sp4C * 32 * 32;
        r2 = c[n * 8 + 4];
        g2 = c[n * 8 + 5];
        b2 = c[n * 8 + 6];
        make_vertex(verts, n, x, y, z, tx, ty, r2, g2, b2, alpha);
        break;
    }
}

Gfx *func_802D0DD4(s16 *a, struct Struct802D0DD4 *b, s8 c)
{
    Vtx *verts = alloc_display_list(b->unk8 * sizeof(*verts));
    Gfx *sp50 = alloc_display_list(11 * sizeof(*sp50));
    Gfx *sp4C = sp50;
    s32 sp48;
    
    if (verts == NULL || sp50 == NULL)
        return NULL;
    func_802D08EC(verts, a, b, c);
    for (sp48 = 1; sp48 < b->unk8; sp48++)
        func_802D0A94(verts, sp48, a, b, c);
    gSPDisplayList(sp4C++, b->unk10)
    gDPSetTextureImage(sp4C++, 0, 2, 1, D_8032FFE4[b->unk4])
    gDPTileSync(sp4C++)
    gDPSetTile(sp4C++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0,
                G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD)
    gDPLoadSync(sp4C++)
    gDPLoadBlock(sp4C++, 7, 0, 0, 1023, 256)
    gSPVertex(sp4C++, 0x80000000 + (u32)verts, b->unk8, 0)
    gSPDisplayList(sp4C++, b->unk18)
    gSPDisplayList(sp4C++, b->unk14)
    gSPEndDisplayList(sp4C)
    return sp50;
}

Gfx *Geo18_802D1B70(s32 run, struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    s32 sp24;
    s16 *sp20;
    struct GraphNode12A *sp1C;
    Gfx *sp18 = NULL;
    
    if (run == TRUE)
    {
        sp24 = 0;
        sp1C = (struct GraphNode12A *)node;
        while (D_80330004[sp24].unkC != 0)
        {
            if (D_80330004[sp24].unk0 == sp1C->unk18)
            {
                sp1C->fnNode.node.flags = (sp1C->fnNode.node.flags & 0xFF) | (D_80330004[sp24].unk20 << 8);
                sp20 = segmented_to_virtual(D_80330004[sp24].unkC);
                func_802D0850(sp20, 4);
                sp18 = func_802D0DD4(sp20, &D_80330004[sp24], 0);
                break;
            }
            sp24++;
        }
    }
    return sp18;
}

Gfx *Geo18_802D1CDC(s32 run, struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    s32 sp24;
    s16 *sp20;
    struct GraphNode12A *sp1C;
    Gfx *sp18 = NULL;
    
    if (run == TRUE)
    {
        sp24 = 0;
        sp1C = (struct GraphNode12A *)node;
        while (D_80330244[sp24].unkC != 0)
        {
            if (D_80330244[sp24].unk0 == sp1C->unk18)
            {
                sp1C->fnNode.node.flags = (sp1C->fnNode.node.flags & 0xFF) | (D_80330244[sp24].unk20 << 8);
                sp20 = segmented_to_virtual(D_80330244[sp24].unkC);
                func_802D0850(sp20, 7);
                sp18 = func_802D0DD4(sp20, &D_80330244[sp24], 1);
                break;
            }
            sp24++;
        }
    }
    return sp18;
}

Gfx *Geo18_802D1E48(s32 run, struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    s32 sp24;
    s16 *sp20;
    struct GraphNode12A *sp1C;
    Gfx *sp18 = NULL;
    
    if (run == TRUE)
    {
        sp24 = 0;
        sp1C = (struct GraphNode12A *)node;
        while (D_80330244[sp24].unkC != 0)
        {
            if (D_80330244[sp24].unk0 == sp1C->unk18)
            {
                sp1C->fnNode.node.flags = (sp1C->fnNode.node.flags & 0xFF) | (D_80330244[sp24].unk20 << 8);
                sp20 = segmented_to_virtual(D_80330244[sp24].unkC);
                sp18 = func_802D0DD4(sp20, &D_80330244[sp24], 1);
                break;
            }
            sp24++;
        }
    }
    return sp18;
}

Gfx *Geo18_802D1FA8(s32 run, struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    s32 sp24;
    s16 *sp20;
    struct GraphNode12A *sp1C;
    Gfx *sp18 = NULL;
    
    if (run == TRUE)
    {
        sp24 = 0;
        sp1C = (struct GraphNode12A *)node;
        while (D_8033031C[sp24].unkC != 0)
        {
            if (D_8033031C[sp24].unk0 == sp1C->unk18)
            {
                sp1C->fnNode.node.flags = (sp1C->fnNode.node.flags & 0xFF) | (D_8033031C[sp24].unk20 << 8);
                sp20 = segmented_to_virtual(D_8033031C[sp24].unkC);
                sp18 = func_802D0DD4(sp20, &D_8033031C[sp24], 1);
                break;
            }
            sp24++;
        }
    }
    return sp18;
}

Gfx *Geo18_802D2108(s32 run, struct GraphNode *node, UNUSED f32 mtx[4][4])
{
    void *sp2C;

    if (run == TRUE)
    {
        struct GraphNode12A *sp28 = (struct GraphNode12A *)node;
        
        switch (sp28->unk18)
        {
        case 2049: sp2C = segmented_to_virtual(ssl_seg7_arr_07004930); break;
        case 2050: sp2C = segmented_to_virtual(ssl_seg7_arr_070049B4); break;
        case 5120: sp2C = segmented_to_virtual(ttc_seg7_arr_07016840); break;
        case 5121: sp2C = segmented_to_virtual(ttc_seg7_arr_07016904); break;
        }
        func_802D0850(sp2C, 7);
    }
    return NULL;
}

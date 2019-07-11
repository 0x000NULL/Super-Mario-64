#include <ultra64.h>

#include "sm64.h"
#include "game.h"
#include "mario.h"
#include "memory.h"
#include "save_file.h"
#include "engine/surface_collision.h"
#include "engine/graph_node.h"
#include "geo_misc.h"
#include "area.h"

struct TextLabel
{
    u32 x;
    u32 y;
    s16 length;
    char buffer[1];  // unknown length
};

struct Struct802D1EE0
{
    s16 unk0;
    u8 filler2[5];
    u8 unk7;
};

extern u8 dl_paintings_draw_ripples[];
extern u8 dl_paintings_rippling_begin[];
extern u8 dl_paintings_rippling_end[];
extern u8 dl_paintings_env_mapped_begin[];
extern u8 dl_paintings_env_mapped_end[];
extern u8 seg2_triangle_mesh[];
extern u8 seg2_mesh_order[];

extern struct PaintingData hmc_seg7_painting_struct[];

extern struct PaintingData inside_castle_seg7_painting_struct_07023620[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023698[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023710[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023788[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023800[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023878[];
extern struct PaintingData inside_castle_seg7_painting_struct_070238F0[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023968[];
extern struct PaintingData inside_castle_seg7_painting_struct_070239E0[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023A58[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023AD0[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023B48[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023BC0[];
extern struct PaintingData inside_castle_seg7_painting_struct_07023C38[];

extern struct PaintingData ttm_seg7_painting_struct[];

extern u16 gAreaUpdateCounter;

s16 D_8035FF90;
float D_8035FF94;
float D_8035FF98;
float D_8035FF9C;
s16 (*D_8035FFA0)[5];
float (*D_8035FFA4)[3];
struct PaintingData *D_8035FFA8;
u8 D_8035FFAC;

extern f32 D_8032FFDC;

struct PaintingData *hmcPaintings[] =
{
    hmc_seg7_painting_struct,
    NULL,
};

struct PaintingData *insideCastlePaintings[] =
{
    inside_castle_seg7_painting_struct_07023620,
    inside_castle_seg7_painting_struct_07023698,
    inside_castle_seg7_painting_struct_07023710,
    inside_castle_seg7_painting_struct_07023788,
    inside_castle_seg7_painting_struct_07023800,
    inside_castle_seg7_painting_struct_07023878,
    inside_castle_seg7_painting_struct_070238F0,
    inside_castle_seg7_painting_struct_07023968,
    inside_castle_seg7_painting_struct_070239E0,
    inside_castle_seg7_painting_struct_07023A58,
    inside_castle_seg7_painting_struct_07023AD0,
    inside_castle_seg7_painting_struct_07023B48,
    inside_castle_seg7_painting_struct_07023BC0,
    inside_castle_seg7_painting_struct_07023C38,
    NULL,
};

struct PaintingData *ttmPaintings[] =
{
    ttm_seg7_painting_struct,
    NULL,
};

struct PaintingData **D_803303EC[] =
{
    hmcPaintings,
    insideCastlePaintings,
    ttmPaintings,
};

s16 D_803303F8 = 1;
s16 D_803303FC = 0;

void func_802D1EE0(s16 *a, void **b)
{
    s16 sp1E;
    s16 sp1C = *a;

    sp1E = 0;
    while (b[sp1E] != NULL)
    {
        struct Struct802D1EE0 *sp18 = segmented_to_virtual(b[sp1E]);

        if (sp18->unk0 != sp1C)
            sp18->unk7 = 0;
        sp1E++;
    }
}

float func_802D1F94(struct PaintingData *a)
{
    //! unnecessary use of double constants
    f32 sp4 = D_8035FF98 - a->unk14 + 50.0;

    if (sp4 < 0.0)
        sp4 = 0.0;
    else if (sp4 > a->unk74)
        sp4 = a->unk74;
    return sp4;
}

float func_802D2028(struct PaintingData *a)
{
    f32 sp4 = a->unk18 - D_8035FF9C;

    if (sp4 < 0.0)
        sp4 = 0.0;
    else if (sp4 > a->unk74)
        sp4 = a->unk74;
    return sp4;
}

float func_802D20A4(struct PaintingData *a, s8 b)
{
    switch (b)
    {
    case 60:
        return func_802D1F94(a);
        break;
    case 70:
        return func_802D2028(a);
        break;
    case 80:
        return a->unk74 / 2.0;
        break;
    }
}

float func_802D2160(struct PaintingData *a)
{
    f32 spC = a->unk74 / 4.0;
    f32 sp8 = a->unk74 / 2.0;
    f32 sp4 = a->unk74 * 3.0 / 4.0;

    if (a->unk6 & 0x20)
        return spC;
    else if (a->unk6 & 0x10)
        return sp8;
    else if (a->unk6 & 0x08)
        return sp4;
    else if (a->unk6 & 0x04)
        return spC;
    else if (a->unk6 & 0x02)
        return sp8;
    else if (a->unk6 & 0x01)
        return sp4;
}

float func_802D22A0(struct PaintingData *a)
{
    f32 sp4 = D_8035FF94 - a->unk10;

    if (sp4 < 0.0)
        sp4 = 0.0;
    else if (sp4 > a->unk74)
        sp4 = a->unk74;
    return sp4;
}

float func_802D231C(struct PaintingData *a, s8 b)
{
    switch (b)
    {
    case 30:
        return func_802D2160(a);
        break;
    case 40:
        return func_802D22A0(a);
        break;
    case 50:
        return a->unk74 / 2.0;
        break;
    }
}

void func_802D23D8(s8 a, struct PaintingData *b, void **c, s8 d, s8 e, s8 f)
{
    func_802D1EE0((s16 *)b, c);
    switch (a)
    {
    case 1:
        b->unk1C = b->unk20;
        b->unk28 = b->unk2C;
        b->unk34 = b->unk38;
        b->unk40 = b->unk44;
        break;
    case 2:
        b->unk1C = b->unk24;
        b->unk28 = b->unk30;
        b->unk34 = b->unk3C;
        b->unk40 = b->unk48;
        break;
    }
    b->unk7 = a;
    b->unk50 = func_802D231C(b, d);
    b->unk54 = func_802D20A4(b, e);
    D_8032FFDC = D_8035FF98;
    if (f == 100)
        b->unk4C = 0.0f;
    D_8035FFA8 = b;
}

void func_802D251C(struct PaintingData *a, void **b)
{
    if (a->unk6 & 0x20)
        func_802D23D8(1, a, b, 30, 60, 100);
    else if (a->unk6 & 0x10)
        func_802D23D8(1, a, b, 30, 60, 100);
    else if (a->unk6 & 0x08)
        func_802D23D8(1, a, b, 30, 60, 100);
    else if (a->unk6 & 0x04)
        func_802D23D8(2, a, b, 30, 60, 100);
    else if (a->unk6 & 0x02)
        func_802D23D8(2, a, b, 30, 60, 100);
    else if (a->unk6 & 0x01)
        func_802D23D8(2, a, b, 30, 60, 100);
}

void func_802D26BC(struct PaintingData *a, void **b)
{
    if (a->unk6 & 0x04)
        func_802D23D8(2, a, b, 30, 60, 100);
    else if (a->unk6 & 0x02)
        func_802D23D8(2, a, b, 30, 60, 100);
    else if (a->unk6 & 0x01)
        func_802D23D8(2, a, b, 30, 60, 100);
}

void func_802D279C(struct PaintingData *a, void **b)
{
    if (a->unk6 & 0x20)
        func_802D23D8(1, a, b, 50, 80, 100);
    else if (a->unk6 & 0x10)
        func_802D23D8(1, a, b, 50, 80, 100);
    else if (a->unk6 & 0x08)
        func_802D23D8(1, a, b, 50, 80, 100);
    else if (a->unk6 & 0x04)
        func_802D23D8(2, a, b, 30, 60, 100);
    else if (a->unk6 & 0x02)
        func_802D23D8(2, a, b, 30, 60, 100);
    else if (a->unk6 & 0x01)
        func_802D23D8(2, a, b, 30, 60, 100);
}

void func_802D293C(struct PaintingData *a, void **b)
{
    if (a->unk6 & 0x04)
        func_802D23D8(2, a, b, 30, 60, -56);
    else if (a->unk6 & 0x02)
        func_802D23D8(2, a, b, 30, 60, -56);
    else if (a->unk6 & 0x01)
        func_802D23D8(2, a, b, 30, 60, -56);
}

void func_802D2A1C(struct PaintingData *a, void **b)
{
    if (a->unk6 & 0x20)
        func_802D23D8(1, a, b, 40, 70, 100);
    else if (a->unk6 & 0x10)
        func_802D23D8(1, a, b, 40, 70, 100);
    else if (a->unk6 & 0x08)
        func_802D23D8(1, a, b, 40, 70, 100);
    else if (a->unk70 != 0)
    {
        if (a->unk5 & 0x04)
            func_802D23D8(2, a, b, 40, 70, 100);
        else if (a->unk5 & 0x02)
            func_802D23D8(2, a, b, 40, 70, 100);
        else if (a->unk5 & 0x01)
            func_802D23D8(2, a, b, 40, 70, 100);
    }
}

void func_802D2BCC(struct PaintingData *a, void **b)
{
    if (a->unk70 != 0)
    {
        if (a->unk5 & 0x04)
            func_802D23D8(2, a, b, 40, 70, 100);
        else if (a->unk5 & 0x02)
            func_802D23D8(2, a, b, 40, 70, 100);
        else if (a->unk5 & 0x01)
            func_802D23D8(2, a, b, 40, 70, 100);
    }
}

void func_802D2CBC(struct PaintingData *a, void **b)
{
    if (a->unk6 & 0x20)
        func_802D23D8(1, a, b, 50, 80, 100);
    else if (a->unk6 & 0x10)
        func_802D23D8(1, a, b, 50, 80, 100);
    else if (a->unk6 & 0x08)
        func_802D23D8(1, a, b, 50, 80, 100);
    else if (a->unk5 & 0x04)
        func_802D23D8(2, a, b, 40, 70, 100);
    else if (a->unk5 & 0x02)
        func_802D23D8(2, a, b, 40, 70, 100);
    else if (a->unk5 & 0x01)
        func_802D23D8(2, a, b, 40, 70, 100);
}

void func_802D2E5C(struct PaintingData *a, void **b)
{
    if (a->unk70 != 0)
    {
        if (a->unk5 & 0x04)
            func_802D23D8(2, a, b, 40, 70, -56);
        else if (a->unk5 & 0x02)
            func_802D23D8(2, a, b, 40, 70, -56);
        else if (a->unk5 & 0x01)
            func_802D23D8(2, a, b, 40, 70, -56);
    }
}

void func_802D2F4C(struct PaintingData *a)
{
    s16 sp6 = a->unk0;
    s8 sp5 = 0;
    s8 sp4 = 0;
    s8 sp3 = 0;
    s8 sp2 = 0;
    s8 sp1 = 0;
    s8 sp0 = 0;

    if (D_8035FF90 == sp6 * 3 + 0xA6)
        sp5 = 32;
    if (D_8035FF90 == sp6 * 3 + 0xA7)
        sp4 = 16;
    if (D_8035FF90 == sp6 * 3 + 0xA8)
        sp3 = 8;
    if (D_8035FF90 == sp6 * 3 + 0xD3)
        sp2 = 4;
    if (D_8035FF90 == sp6 * 3 + 0xD4)
        sp1 = 2;
    if (D_8035FF90 == sp6 * 3 + 0xD5)
        sp0 = 1;
    a->unk4 = a->unk5;
    a->unk5 = sp5 + sp4 + sp3 + sp2 + sp1 + sp0;
    a->unk6 = (a->unk4 ^ a->unk5) & a->unk5;
    a->unk6E = a->unk6F;
    if (D_8035FF98 < a->unk14)
        a->unk6F = 1;
    else
        a->unk6F = 0;
    a->unk70 = (a->unk6E ^ a->unk6F) & a->unk6F;
}

void func_802D310C(struct PaintingData *a)
{
    if (D_803303F8 != D_803303FC)
    {
        a->unk1C *= a->unk28;
        a->unk4C += 1.0;
    }
    if (a->unk6C == 10)
    {
        if (a->unk1C <= 1.0)
        {
            a->unk7 = 0;
            D_8035FFA8 = NULL;
        }
    }
    //L802D319C
    else if (a->unk6C == 20)
    {
        if (a->unk7 == 2 && a->unk1C <= a->unk20)
        {
            a->unk7 = 1;
            a->unk1C = a->unk20;
            a->unk28 = a->unk2C;
            a->unk34 = a->unk38;
            a->unk40 = a->unk44;
        }
    }
}

int func_802D320C(struct PaintingData *a, f32 b, f32 c)
{
    f32 sp3C = a->unk1C;
    f32 sp38 = a->unk34;
    f32 sp34 = a->unk40;
    f32 sp30 = a->unk4C;
    f32 sp2C = a->unk50;
    f32 sp28 = a->unk54;
    f32 sp24;
    f32 sp20;

    b *= a->unk74 / 614.0;
    c *= a->unk74 / 614.0;
    sp24 = sqrtf((b - sp2C) * (b - sp2C) + (c - sp28) * (c - sp28));
    sp20 = sp24 / sp34;
    if (sp30 < sp20)
    {
        return 0;
    }
    else
    {
        f32 sp1C = sp3C * cosf(sp38 * (M_PI * 2) * (sp30 - sp20));

        return round_float(sp1C);
    }
}

int func_802D338C(struct PaintingData *a, s16 b, s16 c, s16 d)
{
    s16 sp1E = 0;

    if (b != 0)
        sp1E = func_802D320C(a, c, d);
    return sp1E;
}

void Print1(struct PaintingData *a, s16 *b, s16 c)
{
    s16 sp1E;

    D_8035FFA0 = mem_pool_alloc(D_8033A124, c * 10);
    if (D_8035FFA0 == NULL)
    {
    }
    for (sp1E = 0; sp1E < c; sp1E++)
    {
        D_8035FFA0[sp1E][0] = b[sp1E * 3 + 1];
        D_8035FFA0[sp1E][1] = b[sp1E * 3 + 2];
        D_8035FFA0[sp1E][2] = func_802D338C(a, b[(sp1E + 1) * 3 + 0], D_8035FFA0[sp1E][0], D_8035FFA0[sp1E][1]);
    }
}

void Print2(s16 *a, s16 b, s16 c)
{
    s16 sp46;

    D_8035FFA4 = mem_pool_alloc(D_8033A124, c * 12);
    if (D_8035FFA4 == NULL)
    {
    }
    for (sp46 = 0; sp46 < c; sp46++)
    {
        s16 sp44 = b * 3 + sp46 * 3 + 2;
        s16 sp42 = a[sp44];
        s16 sp40 = a[sp44 + 1];
        s16 sp3E = a[sp44 + 2];
        f32 sp38 = D_8035FFA0[sp42][0];
        f32 sp34 = D_8035FFA0[sp42][1];
        f32 sp30 = D_8035FFA0[sp42][2];
        f32 sp2C = D_8035FFA0[sp40][0];
        f32 sp28 = D_8035FFA0[sp40][1];
        f32 sp24 = D_8035FFA0[sp40][2];
        f32 sp20 = D_8035FFA0[sp3E][0];
        f32 sp1C = D_8035FFA0[sp3E][1];
        f32 sp18 = D_8035FFA0[sp3E][2];

        D_8035FFA4[sp46][0] = (sp28 - sp34) * (sp18 - sp24) - (sp24 - sp30) * (sp1C - sp28);
        D_8035FFA4[sp46][1] = (sp24 - sp30) * (sp20 - sp2C) - (sp2C - sp38) * (sp18 - sp24);
        D_8035FFA4[sp46][2] = (sp2C - sp38) * (sp1C - sp28) - (sp28 - sp34) * (sp20 - sp2C);
    }
}

int func_802D3918(f32 a)
{
    s8 sp7;

    if (a > 0.0)
        sp7 = a * 127.0 + 0.5;
    else if (a < 0.0)
        sp7 = a * 128.0 - 0.5;
    else
        sp7 = 0;
    return sp7;
}

void func_802D39DC(s16 *a, s16 b)
{
    UNUSED s16 sp38;
    s16 sp34;
    s16 sp32;
    s16 sp30;
    s16 sp2E;
    s16 sp2C = 0;

    for (sp32 = 0; sp32 < b; sp32++)
    {
        f32 sp28 = 0.0f;
        f32 sp24 = 0.0f;
        f32 sp20 = 0.0f;
        f32 sp1C;

        sp2E = a[sp2C];
        for (sp30 = 0; sp30 < sp2E; sp30++)
        {
            sp34 = a[sp2C + sp30 + 1];
            sp28 += D_8035FFA4[sp34][0];
            sp24 += D_8035FFA4[sp34][1];
            sp20 += D_8035FFA4[sp34][2];
        }
        sp2C += sp2E + 1;
        sp28 /= sp2E;
        sp24 /= sp2E;
        sp20 /= sp2E;
        sp1C = sqrtf(sp28 * sp28 + sp24 * sp24 + sp20 * sp20);
        if (sp1C == 0.0)
        {
            ((u8 (*)[10])D_8035FFA0)[sp32][6] = 0;
            ((u8 (*)[10])D_8035FFA0)[sp32][7] = 0;
            ((u8 (*)[10])D_8035FFA0)[sp32][8] = 0;
        }
        else
        {
            ((u8 (*)[10])D_8035FFA0)[sp32][6] = func_802D3918(sp28 / sp1C);
            ((u8 (*)[10])D_8035FFA0)[sp32][7] = func_802D3918(sp24 / sp1C);
            ((u8 (*)[10])D_8035FFA0)[sp32][8] = func_802D3918(sp20 / sp1C);
        }
    }
}

void *func_802D3CF0(u8 *img, s16 b, s16 c, s16 *d, s16 e, s16 f, u8 g)
{
    s16 sp9E;
    s16 sp9C;
    s16 sp9A;
    s16 sp98;
    s16 sp96;
    s16 tx;
    s16 ty;
    s16 sp90 = f / 5;
    s16 sp8E = f % 5;
    s16 sp8C = f * 3;
    s16 sp8A = sp90 * 2 + sp8E + 7;
    Vtx *verts = alloc_display_list(sp8C * sizeof(*verts));
    Gfx *sp80 = alloc_display_list(sp8A * sizeof(*sp80));
    Gfx *sp7C = sp80;

    if (verts == NULL || sp80 == NULL)
    {
    }

    gDPSetTextureImage(sp7C++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, img);
    gDPTileSync(sp7C++);
    gDPSetTile(sp7C++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(sp7C++);
    gDPLoadBlock(sp7C++, 7, 0, 0, b * c - 1, (MAX(1, b * 2 / 8) + 2047) / MAX(1, b * 2 / 8));

    for (sp9E = 0; sp9E < sp90; sp9E++)
    {
        sp9A = e * 3 + sp9E * 15 + 2;
        for (sp9C = 0; sp9C < 15; sp9C++)
        {
            sp98 = d[sp9A + sp9C];
            sp96 = d[sp98 * 3 + 1];
            tx = d[sp98 * 3 + 2];
            ty = d[sp98 * 3 + 3];
            make_vertex(verts, sp9E * 15 + sp9C,
                D_8035FFA0[sp96][0], D_8035FFA0[sp96][1], D_8035FFA0[sp96][2],
                tx, ty,
                ((s8 (*)[10])D_8035FFA0)[sp96][6],
                ((s8 (*)[10])D_8035FFA0)[sp96][7],
                ((s8 (*)[10])D_8035FFA0)[sp96][8],
                g);
        }
        gSPVertex(sp7C++, VIRTUAL_TO_PHYSICAL(verts + sp9E * 15), 15, 0);
        gSPDisplayList(sp7C++, dl_paintings_draw_ripples);
    }

    sp9A = e * 3 + sp90 * 15 + 2;
    for (sp9C = 0; sp9C < sp8E * 3; sp9C++)
    {
        sp98 = d[sp9A + sp9C];
        sp96 = d[sp98 * 3 + 1];
        tx = d[sp98 * 3 + 2];
        ty = d[sp98 * 3 + 3];
        make_vertex(verts, sp90 * 15 + sp9C,
            D_8035FFA0[sp96][0], D_8035FFA0[sp96][1], D_8035FFA0[sp96][2],
            tx, ty,
            ((s8 (*)[10])D_8035FFA0)[sp96][6],
            ((s8 (*)[10])D_8035FFA0)[sp96][7],
            ((s8 (*)[10])D_8035FFA0)[sp96][8],
            g);
    }
    gSPVertex(sp7C++, VIRTUAL_TO_PHYSICAL(verts + sp90 * 15), sp8E * 3, 0);
    for (sp9E = 0; sp9E < sp8E; sp9E++)
        gSP1Triangle(sp7C++, sp9E * 3, sp9E * 3 + 1, sp9E * 3 + 2, 0);
    gSPEndDisplayList(sp7C);
    return sp80;
}

Gfx *func_802D43FC(struct PaintingData *a)
{
    f32 sp4C = a->unk74 / 614.0;
    void *sp48 = alloc_display_list(64);
    void *sp44 = alloc_display_list(64);
    void *sp40 = alloc_display_list(64);
    void *sp3C = alloc_display_list(64);
    void *sp38 = alloc_display_list(40);
    Gfx *sp34 = sp38;

    if (sp48 == NULL || sp44 == NULL || sp40 == NULL || sp38 == NULL)
    {
    }

    guTranslate(sp40, a->unk10, a->unk14, a->unk18);
    guRotate(sp48, a->unk8, 1.0f, 0.0f, 0.0f);
    guRotate(sp44, a->unkC, 0.0f, 1.0f, 0.0f);
    guScale(sp3C, sp4C, sp4C, sp4C);

    gSPMatrix(sp34++, sp40, G_MTX_MODELVIEW | G_MTX_PUSH);
    gSPMatrix(sp34++, sp48, G_MTX_MODELVIEW | G_MTX_MUL);
    gSPMatrix(sp34++, sp44, G_MTX_MODELVIEW | G_MTX_MUL);
    gSPMatrix(sp34++, sp3C, G_MTX_MODELVIEW | G_MTX_MUL);
    gSPEndDisplayList(sp34);

    return sp38;
}

Gfx *func_802D45FC(struct PaintingData *a)
{
    s16 sp66;
    s16 sp64;
    s16 sp62;
    s16 *sp5C;
    s16 faceCount = a->faceCount;
    s16 tWidth = a->textureWidth;
    s16 tHeight = a->textureHeight;
    s16 **meshArray = segmented_to_virtual(a->meshData);
    u8 **tArray = segmented_to_virtual(a->textureArray);
    void *sp48 = alloc_display_list(faceCount * 8 + 48);
    Gfx *sp44 = sp48;

    if (sp48 == NULL)
        return sp48;

    gSPDisplayList(sp44++, func_802D43FC(a));
    gSPDisplayList(sp44++, dl_paintings_rippling_begin);
    gSPDisplayList(sp44++, a->displayList68);

    for (sp62 = 0; sp62 < faceCount; sp62++)
    {
        sp5C = segmented_to_virtual(meshArray[sp62]);
        sp66 = sp5C[0];
        sp64 = sp5C[sp66 * 3 + 1];
        gSPDisplayList(sp44++, func_802D3CF0(tArray[sp62], tWidth, tHeight, sp5C, sp66, sp64, a->unk6D));
    }
    func_802D310C(a);
    gSPPopMatrix(sp44++, G_MTX_MODELVIEW);
    gSPDisplayList(sp44++, dl_paintings_rippling_end);
    gSPEndDisplayList(sp44);
    return sp48;
}

Gfx *func_802D4874(struct PaintingData *a)
{
    s16 sp5E;
    s16 sp5C;
    s16 *sp58;
    s16 tWidth = a->textureWidth;
    s16 tHeight = a->textureHeight;
    s16 **meshArray = segmented_to_virtual(a->meshData);
    u8 **tArray = segmented_to_virtual(a->textureArray);
    Gfx *sp48 = alloc_display_list(56);
    Gfx *sp44 = sp48;

    if (sp48 == NULL)
        return sp48;
    gSPDisplayList(sp44++, func_802D43FC(a));
    gSPDisplayList(sp44++, dl_paintings_env_mapped_begin);
    gSPDisplayList(sp44++, a->displayList68);
    sp58 = segmented_to_virtual(meshArray[0]);
    sp5E = sp58[0];
    sp5C = sp58[sp5E * 3 + 1];
    gSPDisplayList(sp44++, func_802D3CF0(tArray[0], tWidth, tHeight, sp58, sp5E, sp5C, a->unk6D));
    func_802D310C(a);
    gSPPopMatrix(sp44++, G_MTX_MODELVIEW);
    gSPDisplayList(sp44++, dl_paintings_env_mapped_end);
    gSPEndDisplayList(sp44);
    return sp48;
}

Gfx *func_802D4A8C(struct PaintingData *a)
{
    s16 *sp34 = segmented_to_virtual(seg2_triangle_mesh);
    s16 *sp30 = segmented_to_virtual(seg2_mesh_order);
    s16 sp2E = sp34[0];
    s16 sp2C = sp34[sp2E * 3 + 1];
    Gfx *sp28;

    Print1(a, sp34, sp2E);
    Print2(sp34, sp2E, sp2C);
    func_802D39DC(sp30, sp2E);
    switch (a->unk3)
    {
    case 0:
        sp28 = func_802D45FC(a);
        break;
    case 1:
        sp28 = func_802D4874(a);
        break;
    }
    mem_pool_free(D_8033A124, D_8035FFA0);
    mem_pool_free(D_8033A124, D_8035FFA4);
    return sp28;
}

Gfx *func_802D4BAC(struct PaintingData *a)
{
    Gfx *sp2C = alloc_display_list(32);
    Gfx *sp28 = sp2C;

    if (sp2C == NULL)
        return sp2C;
    gSPDisplayList(sp28++, func_802D43FC(a));
    gSPDisplayList(sp28++, a->displayList58);
    gSPPopMatrix(sp28++, G_MTX_MODELVIEW);
    gSPEndDisplayList(sp28);
    return sp2C;
}

void func_802D4C98(struct PaintingData *a)
{
    a->unk4 = 0;
    a->unk5 = 0;
    a->unk6 = 0;
    a->unk6E = 0;
    a->unk6F = 0;
    a->unk70 = 0;
    D_8035FFA8 = NULL;
}

void func_802D4CC8(struct PaintingData *a, f32 b, f32 c, f32 d)
{
    u32 sp24 = save_file_get_star_flags(gCurrSaveFileNum - 1, 8);
    u32 sp20 = save_file_get_flags();
    u32 sp1C = sp24 & 1;
    u32 sp18 = sp20 & 0x100;

    if (sp1C == 0 && sp18 == 0)
    {
        a->unk10 = b;
        D_8035FFAC = 0;
    }
    else if (sp1C != 0 && sp18 == 0)
    {
        a->unk10 += d;
        D_8035FFAC = 2;
        if (a->unk10 >= c)
        {
            a->unk10 = c;
            save_file_set_flags(0x100);
        }
    }
    else if (sp1C != 0 && sp18 != 0)
    {
        a->unk10 = c;
        D_8035FFAC = 3;
    }
}

struct Struct802D4E04
{
    u8 filler0[2];
    s16 unk2;
    u8 filler4[0x18-0x4];
    u32 unk18;
};

void func_802D4E04(struct GraphNode12A *a, struct PaintingData *b)
{
    switch (b->unk6D)
    {
    case 0xFF:
        a->fnNode.node.flags = (a->fnNode.node.flags & 0xFF) | 0x100;
        break;
    default:
        a->fnNode.node.flags = (a->fnNode.node.flags & 0xFF) | 0x500;
        break;
    }
}

Gfx *func_802D4E5C(struct PaintingData *a)
{
    switch (a->unk7)
    {
    case 0:
        return func_802D4BAC(a);
        break;
    default:
        return func_802D4A8C(a);
        break;
    }
}

void func_802D4EC8(struct PaintingData *a, void **b)
{
    if (a->unk6C == 10)
    {
        switch (a->unk7)
        {
        case 0:
            func_802D251C(a, b);
            break;
        case 1:
            func_802D26BC(a, b);
            break;
        }
    }
    else if (a->unk6C == 20)
    {
        switch (a->unk7)
        {
        case 0:
            func_802D279C(a, b);
            break;
        case 1:
            func_802D293C(a, b);
            break;
        }
    }
}

void func_802D4FC0(struct PaintingData *a, void **b)
{
    if (a->unk6C == 10)
    {
        switch (a->unk7)
        {
        case 0:
            func_802D2A1C(a, b);
            break;
        case 1:
            func_802D2BCC(a, b);
            break;
        }
    }
    else if (a->unk6C == 20)
    {
        switch (a->unk7)
        {
        case 0:
            func_802D2CBC(a, b);
            break;
        case 1:
            func_802D2E5C(a, b);
            break;
        }
    }
}

Gfx *Geo18_802D5B98(s32 run, struct GraphNode *node, UNUSED s32 c)
{
    struct GraphNode12A *sp2C = (struct GraphNode12A *)node;
    s32 sp28 = (sp2C->unk18 >> 8) & 0xFF;
    s32 sp24 = sp2C->unk18 & 0xFF;
    Gfx *sp20 = NULL;
    struct PaintingData **sp1C = D_803303EC[sp28];
    struct PaintingData *sp18 = segmented_to_virtual(sp1C[sp24]);

    if (run != TRUE)
    {
        func_802D4C98(sp18);
    }
    else if (run == TRUE) // because the extra comparison was really necessary...
    {
        if (sp28 == 1 && sp24 == 7)
            func_802D4CC8(sp18, 3456.0f, 5529.6f, 20.0f);
        func_802D4E04(sp2C, sp18);
        sp20 = func_802D4E5C(sp18);
        func_802D2F4C(sp18);
        switch ((s16)sp18->unk8)
        {
        case 0:
            func_802D4EC8(sp18, (void *)sp1C);
            break;
        default:
            func_802D4FC0(sp18, (void *)sp1C);
            break;
        }
    }
    return sp20;
}

Gfx *Geo18_802D5D0C(s32 run, UNUSED struct GraphNode *node, UNUSED f32 c[4][4])
{
    struct Surface *surface;

    if (run != TRUE)
    {
        D_803303FC = gAreaUpdateCounter - 1;
        D_803303F8 = gAreaUpdateCounter;
    }
    else
    {
        D_803303FC = D_803303F8;
        D_803303F8 = gAreaUpdateCounter;
        find_floor(gMarioObject->oPosX, gMarioObject->oPosY, gMarioObject->oPosZ, &surface);
        D_8035FF90 = surface->type;
        D_8035FF94 = gMarioObject->oPosX;
        D_8035FF98 = gMarioObject->oPosY;
        D_8035FF9C = gMarioObject->oPosZ;
    }
    return NULL;
}

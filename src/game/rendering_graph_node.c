#include <ultra64.h>

#include "sm64.h"
#include "main.h"
#include "display.h"
#include "print.h"
#include "engine/math_util.h"
#include "area.h"
#include "shadow.h"
#include "memory.h"
#include "game.h"
#include "rendering_graph_node.h"

s16 D_8033A770;
Mat4 D_8033A778[32];
void *D_8033AF78[32];
u8 D_8033AFF8;
u8 D_8033AFF9;
s16 D_8033AFFA;
f32 D_8033AFFC;
u16 *D_8033B000;
s16 *D_8033B004;
u8 D_8033B008;
u8 D_8033B009;
s16 D_8033B00A;
float D_8033B00C;
u16 *D_8033B010;
s16 *D_8033B014;
struct AllocOnlyPool *D_8033B018;

struct Struct8032CF10
{
    u32 unk0[8];
};

struct Struct8032CF10 renderModeTable[2] = {
    {{
        G_RM_OPA_SURF,    G_RM_AA_OPA_SURF,
        G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF, 
        G_RM_AA_TEX_EDGE, G_RM_AA_XLU_SURF,
        G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF,
    }},
    {{
        G_RM_ZB_OPA_SURF,     G_RM_AA_ZB_OPA_SURF,
        G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_INTER,
        G_RM_AA_ZB_TEX_EDGE,  G_RM_AA_ZB_XLU_SURF,
        G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_INTER,
    }}
};

struct Struct8032CF10 D_8032CF50[2] = {
    {{
        0x03024000, 0x00112048,
        0x00112048, 0x00112048, 
        0x00113048, 0x001041C8,
        0x001041C8, 0x001041C8,
    }},
    {{
        0x00112230, 0x00112078,
        0x00112D58, 0x00112478, 
        0x00113078, 0x001049D8,
        0x00104DD8, 0x001045D8,
    }}
};

struct GraphNodeScreenArea *D_8032CF90 = NULL;
struct GraphNodeToggleZBuffer *D_8032CF94 = NULL;
struct GraphNodeCamFrustum *D_8032CF98 = NULL;
struct GraphNode114 *D_8032CF9C = NULL;
struct GraphNodeObject *D_8032CFA0 = NULL;
struct GraphNode12E *D_8032CFA4 = NULL;
u16 gAreaUpdateCounter = 0;

extern Vec3s D_80385FDC;

#ifdef F3DEX_GBI_2
LookAt lookAt;
#endif


void func_8027B110(struct GraphNodeToggleZBuffer *a)
{
    struct GraphNodeToggleZBuffer_sub *sp2C;
    s32 i;
    s32 sp24 = (a->node.flags & GRAPH_RENDER_Z_BUFFER) != 0;
    struct Struct8032CF10 *sp20 = &renderModeTable[sp24];
    struct Struct8032CF10 *sp1C = &D_8032CF50[sp24];

    /** @bug This is where the LookAt values should be calculated but aren't.
     * As a result, environment mapping is broken on Fast3DEX2 without the
     * changes below.
     */

#ifdef F3DEX_GBI_2
    Mtx lMtx;
    guLookAtReflect(&lMtx, &lookAt, 0, 0, 0, /* eye */ 0, 0, 1, /* at */ 1, 0, 0 /* up */ );
#endif
    if (sp24 != 0)
    {
        gDPPipeSync(gDisplayListHead++);
        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }
    //L8027B1BC
    for (i = 0; i < 8; i++)
    {
        if ((sp2C = a->unk14[i]) != NULL)
        {
            gDPSetRenderMode(gDisplayListHead++, sp20->unk0[i], sp1C->unk0[i]);
            while (sp2C != NULL)
            {
                gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(sp2C->unk0), G_MTX_MODELVIEW | G_MTX_LOAD);
                gSPDisplayList(gDisplayListHead++, sp2C->unk4);
                sp2C = sp2C->unk8;
            }
        }
        //L8027B2C8
    }
    if (sp24 != 0)
    {
        gDPPipeSync(gDisplayListHead++);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }
}

void func_8027B354(void *displayList, s16 b)
{
#ifdef F3DEX_GBI_2
        gSPLookAt(gDisplayListHead++, &lookAt);
#endif
    if (D_8032CF94 != 0)
    {
        struct GraphNodeToggleZBuffer_sub *sp1C = alloc_only_pool_alloc(D_8033B018, sizeof(struct GraphNodeToggleZBuffer_sub));

        sp1C->unk0 = D_8033AF78[D_8033A770];
        sp1C->unk4 = displayList;
        sp1C->unk8 = 0;
        if (D_8032CF94->unk14[b] == 0)
            D_8032CF94->unk14[b] = sp1C;
        else
            D_8032CF94->unk34[b]->unk8 = sp1C;
        D_8032CF94->unk34[b] = sp1C;
    }
    //L8027B438
}

void func_8027B450(struct GraphNodeToggleZBuffer *a) // 004
{
    s32 sp1C;

    if (D_8032CF94 == NULL && a->node.children != NULL)
    {
        D_8032CF94 = a;
        for (sp1C = 0; sp1C < 8; sp1C++)
            a->unk14[sp1C] = NULL;
        func_8027D8F8(a->node.children);
        func_8027B110(a);
        D_8032CF94 = NULL;
    }
}

void func_8027B4E8(struct GraphNode002 *a)
{
    if (a->node.children != NULL)
    {
        Mtx *mtx = alloc_display_list(sizeof(*mtx));
        f32 left   = (D_8032CF90->x - D_8032CF90->width) / 2.0f * a->unk14;
        f32 right  = (D_8032CF90->x + D_8032CF90->width) / 2.0f * a->unk14;
        f32 top    = (D_8032CF90->y - D_8032CF90->height) / 2.0f * a->unk14;
        f32 bottom = (D_8032CF90->y + D_8032CF90->height) / 2.0f * a->unk14;

        guOrtho(mtx, left, right, bottom, top, -2.0f, 2.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_PROJECTION | G_MTX_LOAD);

        func_8027D8F8(a->node.children);
    }
}

void func_8027B6C4(struct GraphNodeCamFrustum *a)
{
    if (a->fnNode.func != NULL)
        a->fnNode.func(1, &a->fnNode.node, D_8033A778[D_8033A770]);
    if (a->fnNode.node.children != NULL)
    {
        u16 perspNorm;
        Mtx *mtx = alloc_display_list(sizeof(*mtx));
        f32 aspect = (f32)D_8032CF90->width / (f32)D_8032CF90->height;

        guPerspective(mtx, &perspNorm, a->fov, aspect, a->near, a->far, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, perspNorm);

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_PROJECTION | G_MTX_LOAD);

        D_8032CF98 = a;
        func_8027D8F8(a->fnNode.node.children);
        D_8032CF98 = NULL;
    }
}

void func_8027B840(struct GraphNodeRenderRange *a)
{
    // Dunno what type this is
    struct GraphNodeScreenArea *sp1C = D_8033AF78[D_8033A770];
    s16 sp1A = -sp1C->height;

    if (a->minDistance <= sp1A && sp1A < a->maxDistance)
    {
        if (a->node.children != 0)
            func_8027D8F8(a->node.children);
    }
}

void func_8027B8D4(struct GraphNodeSwitchCase *a)
{
    struct GraphNode *sp1C = a->fnNode.node.children;
    s32 sp18;

    if (a->fnNode.func != NULL)
        a->fnNode.func(1, &a->fnNode.node, D_8033A778[D_8033A770]);
    for (sp18 = 0; sp1C != NULL && a->unk1E > sp18; sp18++)
        sp1C = sp1C->next;
    if (sp1C != NULL)
        func_8027D8F8(sp1C);
}

void func_8027B9A8(struct GraphNode114 *a) // 114
{
    f32 sp28[4][4];
    Mtx *sp24 = alloc_display_list(sizeof(*sp24));
    Mtx *sp20 = alloc_display_list(sizeof(*sp20));

    if (a->fnNode.func != NULL)
        a->fnNode.func(1, &a->fnNode.node, D_8033A778[D_8033A770]);
    mtxf_rotate_xy(sp24, a->unk3A);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(sp24), G_MTX_PROJECTION);

    func_80378F84(sp28, a->unk1C, a->unk28, a->unk38);
    mtxf_mul(D_8033A778[D_8033A770 + 1], sp28, D_8033A778[D_8033A770]);
    D_8033A770++;
    mtxf_to_mtx(sp20, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp20;
    if (a->fnNode.node.children != 0)
    {
        D_8032CF9C = a;
        a->unk34 = D_8033A778[D_8033A770];
        func_8027D8F8(a->fnNode.node.children);
        D_8032CF9C = NULL;
    }
    D_8033A770--;
}

void func_8027BB64(struct GraphNodeTranslationRotationOptionalDisplayList *a)
{
    f32 sp28[4][4];
    Vec3f sp1C;
    Mtx *sp18 = alloc_display_list(sizeof(*sp18));

    vec3s_to_vec3f(sp1C, a->translation);
    mtxf_rotate_zxy_and_translate(sp28, sp1C, a->rotation);
    mtxf_mul(D_8033A778[D_8033A770 + 1], sp28, D_8033A778[D_8033A770]);
    D_8033A770++;
    mtxf_to_mtx(sp18, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp18;
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
    D_8033A770--;
}

void func_8027BC88(struct GraphNode016 *a)
{
    f32 sp28[4][4];
    Vec3f sp1C;
    Mtx *sp18 = alloc_display_list(sizeof(*sp18));

    vec3s_to_vec3f(sp1C, a->unk18);
    mtxf_rotate_zxy_and_translate(sp28, sp1C, D_80385FDC);
    mtxf_mul(D_8033A778[D_8033A770 + 1], sp28, D_8033A778[D_8033A770]);
    D_8033A770++;
    mtxf_to_mtx(sp18, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp18;
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
    D_8033A770--;
}

void func_8027BDAC(struct GraphNode017 *a)
{
    f32 sp20[4][4];
    Mtx *sp1C = alloc_display_list(sizeof(*sp1C));

    mtxf_rotate_zxy_and_translate(sp20, D_80385FD0, a->unk18);
    mtxf_mul(D_8033A778[D_8033A770 + 1], sp20, D_8033A778[D_8033A770]);
    D_8033A770++;
    mtxf_to_mtx(sp1C, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp1C;
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
    D_8033A770--;
}

void func_8027BEC4(struct GraphNodeScaleOptionalDisplayList *a)
{
    UNUSED f32 sp28[4][4];
    Vec3f sp1C;
    Mtx *sp18 = alloc_display_list(sizeof(*sp18));

    vec3f_set(sp1C, a->scale, a->scale, a->scale);
    mtxf_scale_vec3f(D_8033A778[D_8033A770 + 1], D_8033A778[D_8033A770], sp1C);
    D_8033A770++;
    mtxf_to_mtx(sp18, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp18;
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
    D_8033A770--;
}

void func_8027BFE4(struct GraphNodeBillboardOptionalDisplayList *a)
{
    Vec3f sp1C;
    Mtx *sp18 = alloc_display_list(sizeof(*sp18));

    D_8033A770++;
    vec3s_to_vec3f(sp1C, a->translation);
    func_80379798(D_8033A778[D_8033A770], D_8033A778[D_8033A770 - 1], sp1C, D_8032CF9C->unk38);
    if (D_8032CFA4 != NULL)
        mtxf_scale_vec3f(D_8033A778[D_8033A770], D_8033A778[D_8033A770], D_8032CFA4->unk1C->scale);
    else if (D_8032CFA0 != NULL)
        mtxf_scale_vec3f(D_8033A778[D_8033A770], D_8033A778[D_8033A770], D_8032CFA0->scale);

    mtxf_to_mtx(sp18, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp18;
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
    D_8033A770--;
}

void func_8027C18C(struct GraphNodeDisplayList *a)
{
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
}

void func_8027C1F4(struct GraphNode12A *a)
{
    if (a->fnNode.func != NULL)
    {
        s32 sp1C = a->fnNode.func(1, &a->fnNode.node, (struct AllocOnlyPool *) D_8033A778[D_8033A770]);

        if (sp1C != 0)
        {
            func_8027B354((void *)VIRTUAL_TO_PHYSICAL(sp1C), a->fnNode.node.flags >> 8);
        }
    }
    //L8027C274
    if (a->fnNode.node.children != NULL)
        func_8027D8F8(a->fnNode.node.children);
}

void func_8027C2A8(struct GraphNodeBackground *a)
{
    s32 sp3C = 0;

    if (a->fnNode.func != NULL)
        sp3C = a->fnNode.func(1, &a->fnNode.node, (struct AllocOnlyPool *) D_8033A778[D_8033A770]);
    if (sp3C != 0)
    {
        func_8027B354((void *)VIRTUAL_TO_PHYSICAL(sp3C), a->fnNode.node.flags >> 8);
    }
    else if (D_8032CF94 != NULL)
    {
        Gfx *sp38 = alloc_display_list(56);
        Gfx *sp34 = sp38;

        gDPPipeSync(sp34++);
        gDPSetCycleType(sp34++, G_CYC_FILL);
        gDPSetFillColor(sp34++, a->background);
        gDPFillRectangle(sp34++, 0, BORDER_HEIGHT, 319, 239-BORDER_HEIGHT);
        gDPPipeSync(sp34++);
        gDPSetCycleType(sp34++, G_CYC_1CYCLE);
        gSPEndDisplayList(sp34++);

        func_8027B354((void *)VIRTUAL_TO_PHYSICAL(sp38), 0);
    }
    //L8027C48C
    if (a->fnNode.node.children != NULL)
        func_8027D8F8(a->fnNode.node.children);
}

void func_8027C4C0(struct GraphNodeDisplayListTranslated *a)
{
    f32 sp40[4][4];
    Vec3s sp38;
    Vec3f sp2C;
    Mtx *sp28 = alloc_display_list(sizeof(*sp28));

    vec3s_copy(sp38, D_80385FDC);
    vec3f_set(sp2C, a->translation[0], a->translation[1], a->translation[2]);
    if (D_8033B008 == 1)
    {
        sp2C[0] += D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C;
        sp2C[1] += D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C;
        sp2C[2] += D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C;
        D_8033B008 = 5;
    }
    //L8027C640
    else
    {
        if (D_8033B008 == 3)
        {
            sp2C[0] += D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C;
            D_8033B010 += 2;
            sp2C[2] += D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C;
            D_8033B008 = 5;
        }
        //L8027C718
        else
        {
            if (D_8033B008 == 2)
            {
                D_8033B010 += 2;
                sp2C[1] += D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C;
                D_8033B010 += 2;
                D_8033B008 = 5;
            }
            //L8027C7B4
            else if (D_8033B008 == 4)
            {
                D_8033B010 += 6;
                D_8033B008 = 5;
            }
        }
        //L8027C7E8
    }
    //L8027C7E8
    if (D_8033B008 == 5)
    {
        sp38[0] = D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)];
        sp38[1] = D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)];
        sp38[2] = D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)];
    }
    //L8027C88C
    mtxf_rotate_xyz_and_translate(sp40, sp2C, sp38);
    mtxf_mul(D_8033A778[D_8033A770 + 1], sp40, D_8033A778[D_8033A770]);
    D_8033A770++;
    mtxf_to_mtx(sp28, D_8033A778[D_8033A770]);
    D_8033AF78[D_8033A770] = sp28;
    if (a->displayList != NULL)
        func_8027B354(a->displayList, a->node.flags >> 8);
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
    D_8033A770--;
}

void func_8027C988(struct GraphNodeObject_sub *a, s32 b)
{
    struct Animation *sp1C = a->curAnim;

    if (b != 0)
        a->animFrame = func_8037C844(a, &a->animFrameAccelAssist);
    a->animTimer = gAreaUpdateCounter;
    if (sp1C->flags & ANIM_FLAG_3)
        D_8033B008 = 2;
    else if (sp1C->flags & ANIM_FLAG_4)
        D_8033B008 = 3;
    else if (sp1C->flags & ANIM_FLAG_6)
        D_8033B008 = 4;
    else
        D_8033B008 = 1;
    //L8027CA50
    D_8033B00A = a->animFrame;
    D_8033B009 = (sp1C->flags & ANIM_FLAG_5) == 0;
    D_8033B010 = segmented_to_virtual(sp1C->index);
    D_8033B014 = segmented_to_virtual(sp1C->values);

    if (sp1C->unk02 == 0)
        D_8033B00C = 1.0f;
    else
        D_8033B00C = (f32)a->animYTrans / (f32)sp1C->unk02;
}

void func_8027CB08(struct GraphNodeShadow *a)
{
    Gfx *sp94;
    f32 sp54[4][4];
    Vec3f sp48;
    Vec3f sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    struct GraphNode *sp28;
    Mtx *sp24;

    if (D_8032CF9C != NULL && D_8032CFA0 != NULL)
    {
        if (D_8032CFA4 != NULL)
        {
            func_8037A550(sp48, D_8033A778[D_8033A770], D_8032CF9C->unk34);
            sp34 = a->shadowScale;
        }
        else
        {
            vec3f_copy(sp48, D_8032CFA0->pos);
            sp34 = a->shadowScale * D_8032CFA0->scale[0];
        }
        //L8027CBC8
        sp38 = 1.0f;
        if (D_8033B009 != 0)
        {
            if (D_8033B008 == 1 || D_8033B008 == 3)
            {
                sp28 = a->node.children;
                if (sp28 != NULL && sp28->type == GRAPH_NODE_TYPE_SCALE_OPTIONAL_DISPLAY_LIST)
                    sp38 = ((struct GraphNodeScaleOptionalDisplayList *)sp28)->scale;
                //L8027CC3C
                sp3C[0] = D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C * sp38;
                sp3C[1] = 0.0f;
                D_8033B010 += 2;
                sp3C[2] = D_8033B014[func_8037C7D8(D_8033B00A, &D_8033B010)] * D_8033B00C * sp38;
                D_8033B010 -= 6;
                sp30 = sins(D_8032CFA0->angle[1]);
                sp2C = coss(D_8032CFA0->angle[1]);

                sp48[0] += sp3C[0] * sp2C + sp3C[2] * sp30;
                sp48[2] += -sp3C[0] * sp30 + sp3C[2] * sp2C;
            }
        }
        //L8027CDBC
        sp94 = create_shadow_below_xyz(sp48[0], sp48[1], sp48[2], sp34, a->shadowSolidity, a->shadowType);
        if (sp94 != NULL)
        {
            sp24 = alloc_display_list(sizeof(*sp24));
            D_8033A770++;
            mtxf_translate(sp54, sp48);
            mtxf_mul(D_8033A778[D_8033A770], sp54, D_8032CF9C->unk34);
            mtxf_to_mtx(sp24, D_8033A778[D_8033A770]);
            D_8033AF78[D_8033A770] = sp24;
            if (gShadowAboveWaterOrLava == 1)
                func_8027B354((void *)VIRTUAL_TO_PHYSICAL(sp94), 4);
            else if (gMarioOnIceOrCarpet == 1)
                func_8027B354((void *)VIRTUAL_TO_PHYSICAL(sp94), 5);
            else
                func_8027B354((void *)VIRTUAL_TO_PHYSICAL(sp94), 6);
            D_8033A770--;
        }
        //L8027CF34
    }
    //L8027CF34
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
}

extern f32 D_80335DF8;

int func_8027CF68(struct GraphNodeObject *a, Mat4 b)
{
    s16 spE;
    s16 spC;
    struct GraphNode *sp8;
    f32 sp4;

    if (a->node.flags & 0x10)
        return 0;
    sp8 = a->asGraphNode;
    spC = (D_8032CF98->fov / 2.0f + 1.0f) * 32768.0f / 180.0f + 0.5f;
    sp4 = -b[3][2] * sins(spC) / coss(spC);
    if (sp8 != NULL && sp8->type == GRAPH_NODE_TYPE_02F)
        spE = (f32)((struct GraphNode02F *)sp8)->unk14;  //! Why is there a f32 cast?
    else
        spE = 300;
    if (b[3][2] > -100.0f + spE)
        return 0;
    if (b[3][2] < -20000.0f - spE)
        return 0;
    if (b[3][0] > sp4 + spE)
        return 0;
    if (b[3][0] < -sp4 - spE)
        return 0;
    return 1;
}

void func_8027D14C(struct Object *a)
{
    Mat4 sp30;
    s32 sp2C = (a->header.gfx.node.flags & GRAPH_RENDER_20) != 0;

    if (a->header.gfx.unk18 == D_8032CF90->unk14)
    {
        if (a->header.gfx.throwMatrix != NULL)
        {
            mtxf_mul(D_8033A778[D_8033A770 + 1], (void *)a->header.gfx.throwMatrix, D_8033A778[D_8033A770]);
        }
        else if (a->header.gfx.node.flags & 4)
        {
            func_80379798(D_8033A778[D_8033A770 + 1], D_8033A778[D_8033A770], a->header.gfx.pos, D_8032CF9C->unk38);
        }
        else
        {
            mtxf_rotate_zxy_and_translate(sp30, a->header.gfx.pos, a->header.gfx.angle);
            mtxf_mul(D_8033A778[D_8033A770 + 1], sp30, D_8033A778[D_8033A770]);
        }

        mtxf_scale_vec3f(D_8033A778[D_8033A770 + 1], D_8033A778[D_8033A770 + 1], a->header.gfx.scale);
        a->header.gfx.throwMatrix = D_8033A778[++D_8033A770];
        a->header.gfx.cameraToObject[0] = D_8033A778[D_8033A770][3][0];
        a->header.gfx.cameraToObject[1] = D_8033A778[D_8033A770][3][1];
        a->header.gfx.cameraToObject[2] = D_8033A778[D_8033A770][3][2];
        // FIXME: correct types
        if (a->header.gfx.unk38.curAnim != NULL)
            func_8027C988(&a->header.gfx.unk38, sp2C);
        if (func_8027CF68(&a->header.gfx, D_8033A778[D_8033A770]))
        {
            Mtx *sp28 = alloc_display_list(sizeof(*sp28));

            mtxf_to_mtx(sp28, D_8033A778[D_8033A770]);
            D_8033AF78[D_8033A770] = sp28;
            if (a->header.gfx.asGraphNode != NULL)
            {
                D_8032CFA0 = (struct GraphNodeObject *)a;
                a->header.gfx.asGraphNode->parent = &a->header.gfx.node;
                func_8027D8F8(a->header.gfx.asGraphNode);
                a->header.gfx.asGraphNode->parent = NULL;
                D_8032CFA0 = NULL;
            }
            if (a->header.gfx.node.children != NULL)
                func_8027D8F8(a->header.gfx.node.children);
        }
        //L8027D420
        D_8033A770--;
        D_8033B008 = 0;
        a->header.gfx.throwMatrix = NULL;
    }
}

void func_8027D460(struct GraphNode029 *a)
{
    if (a->unk14 != NULL)
    {
        a->unk14->parent = (struct GraphNode *) a;
        func_8027D8F8(a->unk14);
        a->unk14->parent = NULL;
    }
    if (a->node.children != NULL)
        func_8027D8F8(a->node.children);
}

void func_8027D4D4(struct GraphNode12E *a)
{
    Mat4 sp30;
    Vec3f sp24;
    Mtx *sp20 = alloc_display_list(sizeof(*sp20));

#ifdef F3DEX_GBI_2
        gSPLookAt(gDisplayListHead++, &lookAt);
#endif

    if (a->fnNode.func != NULL)
        a->fnNode.func(1, &a->fnNode.node, D_8033A778[D_8033A770]);
    if (a->unk1C != NULL && a->unk1C->asGraphNode != NULL)
    {
        UNUSED s32 sp1C = (a->unk1C->node.flags & GRAPH_RENDER_20) != 0;

        sp24[0] = a->unk20[0] / 4.0f;
        sp24[1] = a->unk20[1] / 4.0f;
        sp24[2] = a->unk20[2] / 4.0f;
        /*
        sp24[0] = a->unk20 / 4.0f;
        sp24[1] = a->unk22 / 4.0f;
        sp24[2] = a->unk24 / 4.0f;
        */
        mtxf_translate(sp30, sp24);
        mtxf_copy(D_8033A778[D_8033A770 + 1], (void *)D_8032CFA0->throwMatrix);
        D_8033A778[D_8033A770 + 1][3][0] = D_8033A778[D_8033A770][3][0];
        D_8033A778[D_8033A770 + 1][3][1] = D_8033A778[D_8033A770][3][1];
        D_8033A778[D_8033A770 + 1][3][2] = D_8033A778[D_8033A770][3][2];
        mtxf_mul(D_8033A778[D_8033A770 + 1], sp30, D_8033A778[D_8033A770 + 1]);
        mtxf_scale_vec3f(D_8033A778[D_8033A770 + 1], D_8033A778[D_8033A770 + 1], a->unk1C->scale);
        if (a->fnNode.func != NULL)
            a->fnNode.func(5, &a->fnNode.node, (struct AllocOnlyPool *) D_8033A778[D_8033A770 + 1]);
        D_8033A770++;
        mtxf_to_mtx(sp20, D_8033A778[D_8033A770]);
        D_8033AF78[D_8033A770] = sp20;
        D_8033AFF8 = D_8033B008;
        D_8033AFF9 = D_8033B009;
        D_8033AFFA = D_8033B00A;
        D_8033AFFC = D_8033B00C;
        D_8033B000 = D_8033B010;
        D_8033B004 = D_8033B014;
        D_8033B008 = 0;
        D_8032CFA4 = (void *)a;
        if (a->unk1C->unk38.curAnim != NULL)
            func_8027C988(&a->unk1C->unk38, sp1C);
        //L8027D7F8
        func_8027D8F8(a->unk1C->asGraphNode);
        D_8032CFA4 = NULL;
        D_8033B008 = D_8033AFF8;
        D_8033B009 = D_8033AFF9;
        D_8033B00A = D_8033AFFA;
        D_8033B00C = D_8033AFFC;
        D_8033B010 = D_8033B000;
        D_8033B014 = D_8033B004;
        D_8033A770--;
    }
    //L8027D884
    if (a->fnNode.node.children != NULL)
        func_8027D8F8(a->fnNode.node.children);
}

void func_8027D8B8(struct GraphNode *a)
{
    if (a->children != NULL)
        func_8027D8F8(a->children);
}

void func_8027D8F8(struct GraphNode *rootGraphNode)
{
    s16 sp2E = TRUE;
    struct GraphNode *curGraphNode = rootGraphNode;
    struct GraphNode *sp24 = curGraphNode->parent;

    if (sp24 != NULL)
        sp2E = (sp24->type != GRAPH_NODE_TYPE_SWITCH_CASE);
    //L8027D944
    do
    {
        if (curGraphNode->flags & GRAPH_RENDER_01)
        {
            if (curGraphNode->flags & GRAPH_RENDER_02)
            {
                func_8027D8B8(curGraphNode);
            }
            //L8027D97C
            else
            {
                switch (curGraphNode->type)
                {
                case GRAPH_NODE_TYPE_002:
                    func_8027B4E8((struct GraphNode002 *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_103:
                    func_8027B6C4((struct GraphNodeCamFrustum *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_TOGGLE_Z_BUFFER:
                    func_8027B450((struct GraphNodeToggleZBuffer *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_RENDER_RANGE:
                    func_8027B840((struct GraphNodeRenderRange *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_SWITCH_CASE:
                    func_8027B8D4((struct GraphNodeSwitchCase *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_114:
                    func_8027B9A8((struct GraphNode114 *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_TRANSLATION_ROTATION_OPTIONAL_DISPLAY_LIST:
                    func_8027BB64((struct GraphNodeTranslationRotationOptionalDisplayList *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_016:
                    func_8027BC88((struct GraphNode016 *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_017:
                    func_8027BDAC((struct GraphNode017 *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_OBJECT:
                    func_8027D14C((struct Object *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_DISPLAY_LIST_TRANSLATED:
                    func_8027C4C0((struct GraphNodeDisplayListTranslated *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_BILLBOARD_OPTIONAL_DISPLAY_LIST:
                    func_8027BFE4((struct GraphNodeBillboardOptionalDisplayList *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_DISPLAY_LIST:
                    func_8027C18C((struct GraphNodeDisplayList *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_SCALE_OPTIONAL_DISPLAY_LIST:
                    func_8027BEC4((struct GraphNodeScaleOptionalDisplayList *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_SHADOW:
                    func_8027CB08((struct GraphNodeShadow *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_029:
                    func_8027D460((struct GraphNode029 *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_12A:
                    func_8027C1F4((struct GraphNode12A *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_BACKGROUND:
                    func_8027C2A8((struct GraphNodeBackground *) curGraphNode);
                    break;
                case GRAPH_NODE_TYPE_12E:
                    func_8027D4D4((struct GraphNode12E *) curGraphNode);
                    break;
                default:
                    func_8027D8B8((struct GraphNode *) curGraphNode);
                    break;
                }
            }
            //L8027DB20
        }
        //L8027DB28
        else
        {
            if (curGraphNode->type == GRAPH_NODE_TYPE_OBJECT)
                ((struct GraphNodeObject *)curGraphNode)->throwMatrix = NULL;
        }
        //L8027DB44
    } while (sp2E && (curGraphNode = curGraphNode->next) != rootGraphNode);
}

void func_8027DB80(struct GraphNodeScreenArea *a, Vp *b, Vp *c, s32 d)
{
    UNUSED s32 unused;

    if (a->node.flags & GRAPH_RENDER_01)
    {
        Mtx *sp30;
        Vp *viewport = alloc_display_list(sizeof(*viewport));

        D_8033B018 = alloc_only_pool_init(main_pool_available() - 16, MEMORY_POOL_LEFT);
        sp30 = alloc_display_list(sizeof(*sp30));
        D_8033A770 = 0;
        D_8033B008 = 0;
        vec3s_set(viewport->vp.vtrans, a->x * 4, a->y * 4, 511);
        vec3s_set(viewport->vp.vscale, a->width * 4, a->height * 4, 511);
        if (b != NULL)
        {
            clear_frame_buffer(d);
            make_viewport_clip_rect(b);
            *viewport = *b;
        }
        //L8027DC94
        else if (c != NULL)
        {
            clear_frame_buffer(d);
            make_viewport_clip_rect(c);
        }
        //L8027DCB0
        mtxf_identity(D_8033A778[D_8033A770]);
        mtxf_to_mtx(sp30, D_8033A778[D_8033A770]);
        D_8033AF78[D_8033A770] = sp30;
        gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(viewport));
        gSPMatrix(gDisplayListHead++,
            VIRTUAL_TO_PHYSICAL(D_8033AF78[D_8033A770]),
            G_MTX_MODELVIEW | G_MTX_LOAD);
        D_8032CF90 = a;
        if (a->node.children != NULL)
            func_8027D8F8(a->node.children);
        D_8032CF90 = NULL;
        if (gShowDebugText)
            print_text_fmt_int(180, 36, "MEM %d", D_8033B018->totalSpace - D_8033B018->usedSpace);
        main_pool_free(D_8033B018);
    }
    //L8027DE10
}

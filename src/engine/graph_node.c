#include <ultra64.h>
#include "sm64.h"

#include "game/level_update.h"
#include "math_util.h"
#include "game/memory.h"
#include "graph_node.h"
#include "game/rendering_graph_node.h"
#include "game/area.h"
#include "geo_layout.h"

// unused Mtx(s)
s16 identityMtx[4][4] = {
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1}
};
s16 zeroMtx[4][4] = {
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

Vec3f D_80385FD0 = {0.0f, 0.0f, 0.0f};
Vec3s D_80385FDC = {0, 0, 0};
Vec3f D_80385FE4 = {1.0f, 1.0f, 1.0f};
Vec3s unused80385FF0 = {1, 1, 1};

void init_scene_graph_node_links(struct GraphNode *graphNode, s32 type)
{
    graphNode->type = type;
    graphNode->flags = GRAPH_RENDER_01;
    graphNode->prev = graphNode;
    graphNode->next = graphNode;
    graphNode->parent = NULL;
    graphNode->children = NULL;
}

struct GraphNodeScreenArea *init_graph_node_screen_area(struct AllocOnlyPool *pool, struct GraphNodeScreenArea *graphNode,
    s16 sp20, s16 x, s16 y, s16 width, s16 height)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeScreenArea));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_SCREEN_AREA);

        graphNode->unk14 = sp20;
        graphNode->unk15 = 0;
        graphNode->x = x;
        graphNode->y = y;
        graphNode->width = width;
        graphNode->height = height;
        graphNode->unk20 = NULL;
        graphNode->unk1E = 0;
    }

    return graphNode;
}

struct GraphNode002 *init_graph_node_002(struct AllocOnlyPool *pool, struct GraphNode002 *graphNode, f32 sp20)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode002));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_002);

        graphNode->unk14 = sp20;
    }

    return graphNode;
}

struct GraphNodeCamFrustum *init_graph_node_cam_frustum(struct AllocOnlyPool *pool, struct GraphNodeCamFrustum *graphNode,
    f32 fov, s16 near, s16 far, GraphNodeFunc nodeFunc, s32 sp30)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeCamFrustum));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->fnNode.node, GRAPH_NODE_TYPE_103);

        graphNode->fov = fov;
        graphNode->near = near;
        graphNode->far = far;
        graphNode->fnNode.func = nodeFunc;
        graphNode->unk18 = sp30;

        if(nodeFunc != NULL)
        {
            nodeFunc(0, &graphNode->fnNode.node, pool);
        }
    }

    return graphNode;
}

struct GraphNodeStart *init_graph_node_start(struct AllocOnlyPool *pool, struct GraphNodeStart *graphNode)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeStart));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_START);
    }

    return graphNode;
}

struct GraphNodeToggleZBuffer *init_graph_node_toggle_z_buffer(struct AllocOnlyPool *pool, struct GraphNodeToggleZBuffer *graphNode, s16 on)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeToggleZBuffer));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_TOGGLE_Z_BUFFER);

        if(on)
        {
            graphNode->node.flags |= GRAPH_RENDER_Z_BUFFER;
        }
    }

    return graphNode;
}

struct GraphNodeRenderRange *init_graph_node_render_range(struct AllocOnlyPool *pool, struct GraphNodeRenderRange *graphNode,
    s16 minDistance, s16 maxDistance)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeRenderRange));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_RENDER_RANGE);
        graphNode->minDistance = minDistance;
        graphNode->maxDistance = maxDistance;
    }

    return graphNode;
}

// switch
struct GraphNodeSwitchCase *init_graph_node_switch_case(struct AllocOnlyPool *pool, struct GraphNodeSwitchCase *graphNode,
    s16 numCases, s16 sp26, GraphNodeFunc nodeFunc, s32 sp2c)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeSwitchCase));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->fnNode.node, GRAPH_NODE_TYPE_SWITCH_CASE);
        graphNode->numCases = numCases;
        graphNode->unk1E = sp26;
        graphNode->fnNode.func = nodeFunc;
        graphNode->unk18 = sp2c;

        if(nodeFunc != NULL)
        {
            nodeFunc(0, &graphNode->fnNode.node, pool);
        }
    }

    return graphNode;
}

struct GraphNode114 *init_graph_node_114(struct AllocOnlyPool *pool, struct GraphNode114 * graphNode,
    f32 *sp20, f32 *sp24, GraphNodeFunc sp28, s32 sp2c)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode114));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->fnNode.node, GRAPH_NODE_TYPE_114);
        vec3f_copy(graphNode->unk1C, sp20);
        vec3f_copy(graphNode->unk28, sp24);
        graphNode->fnNode.func = sp28;
        graphNode->unk18 = sp2c;
        graphNode->unk38 = 0;
        graphNode->unk3A = 0;

        if(sp28 != NULL)
        {
            sp28(0, &graphNode->fnNode.node, pool);
        }
    }

    return graphNode;
}

struct GraphNodeTranslationRotationOptionalDisplayList *init_graph_node_translation_rotation_optional_display_list(struct AllocOnlyPool *pool,
    struct GraphNodeTranslationRotationOptionalDisplayList *graphNode, s32 drawingLayer, void *displayList, Vec3s translation, Vec3s rotation)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeTranslationRotationOptionalDisplayList));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_TRANSLATION_ROTATION_OPTIONAL_DISPLAY_LIST);

        vec3s_copy(graphNode->translation, translation);
        vec3s_copy(graphNode->rotation, rotation);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNode016 *init_graph_node_016(struct AllocOnlyPool *pool, struct GraphNode016 *graphNode,
    s32 drawingLayer, void *displayList, Vec3s sp28)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode016));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_016);

        vec3s_copy(graphNode->unk18, sp28);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNode017 *init_graph_node_017(struct AllocOnlyPool *pool, struct GraphNode017 *graphNode,
    s32 drawingLayer, void *displayList, Vec3s sp28)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode017));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_017);
        vec3s_copy(graphNode->unk18, sp28);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNodeScaleOptionalDisplayList *init_graph_node_scale_optional_display_list(struct AllocOnlyPool *pool,
    struct GraphNodeScaleOptionalDisplayList *graphNode, s32 drawingLayer, void *displayList, f32 scale)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeScaleOptionalDisplayList));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_SCALE_OPTIONAL_DISPLAY_LIST);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->scale = scale;
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNodeObject *init_graph_node_object(struct AllocOnlyPool *pool, struct GraphNodeObject *graphNode,
    struct GraphNode *sp20, Vec3f pos, Vec3s angle, Vec3f scale)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeObject));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_OBJECT);
        vec3f_copy(graphNode->pos, pos);
        vec3f_copy(graphNode->scale, scale);
        vec3s_copy(graphNode->angle, angle);
        graphNode->asGraphNode = sp20;
        graphNode->throwMatrix = NULL;
        graphNode->unk38.animID = 0;
        graphNode->unk38.curAnim = NULL;
        graphNode->unk38.animFrame = 0;
        graphNode->unk38.animFrameAccelAssist = 0;
        graphNode->unk38.animAccel = 0x10000;
        graphNode->unk38.animTimer = 0;
        graphNode->node.flags |= GRAPH_RENDER_20;
    }

    return graphNode;
}

struct GraphNode02F *init_graph_node_02F(struct AllocOnlyPool *pool, struct GraphNode02F *graphNode,
    s16 sp22)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode02F));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_02F);
        graphNode->unk14 = sp22;
    }

    return graphNode;
}

struct GraphNodeDisplayListTranslated *init_graph_node_display_list_translated(struct AllocOnlyPool *pool,
    struct GraphNodeDisplayListTranslated * graphNode, s32 drawingLayer, void *displayList, Vec3s translation)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeDisplayListTranslated));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_DISPLAY_LIST_TRANSLATED);
        vec3s_copy(graphNode->translation, translation);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNodeBillboardOptionalDisplayList *init_graph_node_billboard_optional_display_list(struct AllocOnlyPool *pool,
    struct GraphNodeBillboardOptionalDisplayList *graphNode, s32 drawingLayer, void *displayList, Vec3s translation)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeBillboardOptionalDisplayList));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_BILLBOARD_OPTIONAL_DISPLAY_LIST);
        vec3s_copy(graphNode->translation, translation);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNodeDisplayList *init_graph_node_display_list(struct AllocOnlyPool *pool, struct GraphNodeDisplayList *graphNode,
    s32 drawingLayer, void *displayList)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeDisplayList));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_DISPLAY_LIST);
        graphNode->node.flags = (drawingLayer << 8) | (graphNode->node.flags & 0xFF);
        graphNode->displayList = displayList;
    }

    return graphNode;
}

struct GraphNodeShadow *init_graph_node_shadow(struct AllocOnlyPool *pool, struct GraphNodeShadow *graphNode,
    s16 shadowScale, u8 shadowSolidity, u8 shadowType)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeShadow));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_SHADOW);
        graphNode->shadowScale = shadowScale;
        graphNode->shadowSolidity = shadowSolidity;
        graphNode->shadowType = shadowType;
    }

    return graphNode;
}

struct GraphNode029 *init_graph_node_029(struct AllocOnlyPool *pool, struct GraphNode029 *graphNode,
    struct GraphNode *sp24)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode029));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->node, GRAPH_NODE_TYPE_029);
        graphNode->unk14 = sp24;
    }

    return graphNode;
}

struct GraphNode12A *init_graph_node_12A(struct AllocOnlyPool *pool, struct GraphNode12A *graphNode,
    GraphNodeFunc sp20, s32 sp24)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode12A));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->fnNode.node, GRAPH_NODE_TYPE_12A);
        graphNode->fnNode.func = sp20;
        graphNode->unk18 = sp24;

        if(sp20 != NULL)
        {
            sp20(0, &graphNode->fnNode.node, pool);
        }
    }

    return graphNode;
}

// background
struct GraphNodeBackground *init_graph_node_background(struct AllocOnlyPool *pool, struct GraphNodeBackground *graphNode,
    u16 background, GraphNodeFunc backgroundFunc, s32 zero)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNodeBackground));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->fnNode.node, GRAPH_NODE_TYPE_BACKGROUND);

        graphNode->background = (background << 16) | background;
        graphNode->fnNode.func = backgroundFunc;
        graphNode->unused = zero; // always 0, unused

        if(backgroundFunc != NULL)
        {
            backgroundFunc(0, &graphNode->fnNode.node, pool);
        }
    }

    return graphNode;
}

struct GraphNode12E *init_graph_node_12E(struct AllocOnlyPool *pool, struct GraphNode12E *graphNode,
    s32 sp20, Vec3s sp24, GraphNodeFunc nodeFunc, s32 sp2c)
{
    if(pool != NULL)
    {
        graphNode = alloc_only_pool_alloc(pool, sizeof(struct GraphNode12E));
    }

    if(graphNode != NULL)
    {
        init_scene_graph_node_links(&graphNode->fnNode.node, GRAPH_NODE_TYPE_12E);
        vec3s_copy(graphNode->unk20, sp24);
        graphNode->unk1C = (struct GraphNodeObject *) sp20; // assumed type
        graphNode->fnNode.func = nodeFunc;
        graphNode->unk18 = sp2c;

        if(nodeFunc != NULL)
        {
            nodeFunc(0, &graphNode->fnNode.node, pool);
        }
    }

    return graphNode;
}

struct GraphNode *func_8037C044(struct GraphNode *prevNode, struct GraphNode *node)
{
    struct GraphNode *sp04;
    struct GraphNode *sp00;

    if(node != NULL)
    {
        node->parent = prevNode;
        sp04 = prevNode->children;

        if(sp04 == NULL)
        {
            prevNode->children = node;
            node->prev = node;
            node->next = node;
        }
        else
        {
            sp00 = sp04->prev;
            node->prev = sp00;
            node->next = sp04;
            sp04->prev = node;
            sp00->next = node;
        }
    }

    return node;
}

struct GraphNode *func_8037C0BC(struct GraphNode *graphNode)
{
    struct GraphNode *sp04;
    struct GraphNode **sp00;

    sp04 = graphNode->parent;
    sp00 = &sp04->children;

    graphNode->prev->next = graphNode->next;
    graphNode->next->prev = graphNode->prev;

    if(*sp00 == graphNode)
    {
        if(graphNode->next == graphNode)
        {
            *sp00 = NULL;
        }
        else
        {
            *sp00 = graphNode->next;
        }
    }

    return sp04;
}

struct GraphNode *func_8037C138(struct GraphNode *a0)
{
    struct GraphNode *sp0c;
    struct GraphNode *sp08;
    struct GraphNode **sp04;

    sp08 = a0->parent;
    sp04 = &sp08->children;

    if(*sp04 != a0)
    {
        if((*sp04)->prev != a0)
        {
            a0->prev->next = a0->next;
            a0->next->prev = a0->prev;
            sp0c = (*sp04)->prev;
            a0->prev = sp0c;
            a0->next = *sp04;
            (*sp04)->prev = a0;
            sp0c->next = a0;
        }
        *sp04 = a0;
    }
    
    return sp08;
}

// recursive
void func_8037C1E4(struct GraphNode *graphNode, s32 sp34)
{
    struct GraphNode **sp2c;
    struct GraphNode *sp28;
    struct FnGraphNode *sp24;
    
    sp28 = graphNode;

    do
    {
        sp24 = (struct FnGraphNode *) sp28;

        if(sp28->type & GRAPH_NODE_TYPE_FUNCTIONAL)
        {
            if(sp24->func != NULL)
            {
                sp24->func(sp34, sp28, 0);
            }
        }

        if(sp28->children != NULL)
        {
            switch(sp28->type)
            {
            case GRAPH_NODE_TYPE_TOGGLE_Z_BUFFER: // zbuffer
                sp2c = (struct GraphNode **) &D_8032CF94;
                break;
            case GRAPH_NODE_TYPE_103: // camera frustum
                sp2c = (struct GraphNode **) &D_8032CF98;
                break;
            case GRAPH_NODE_TYPE_114:
                sp2c = (struct GraphNode **) &D_8032CF9C;
                break;
            case GRAPH_NODE_TYPE_OBJECT: // object
                sp2c = (struct GraphNode **) &D_8032CFA0;
                break;
            default:
                sp2c = NULL;
                break;
            }

            if(sp2c != NULL)
            {
                *sp2c = sp28;
            }

            func_8037C1E4(sp28->children, sp34);

            if(sp2c != NULL)
            {
                *sp2c = NULL;
            }
        }
    } while((sp28 = sp28->next) != graphNode);
}

// a0 = GraphNodeScreenArea?
void func_8037C360(struct GraphNode *graphNode, s32 sp1c)
{
    if(graphNode->flags & GRAPH_RENDER_01)
    {
        D_8032CF90 = (struct GraphNodeScreenArea *) graphNode;

        if(graphNode->children != NULL)
        {
            func_8037C1E4(graphNode->children, sp1c);
        }

        D_8032CF90 = 0;
    }
}

void func_8037C3D0(struct GraphNodeObject *graphNode)
{
    init_graph_node_object(NULL, graphNode, 0, D_80385FD0, D_80385FDC, D_80385FE4);
    
    func_8037C044(&D_8038BD88, &graphNode->node);
    graphNode->node.flags &= ~GRAPH_RENDER_01;
}

void func_8037C448(struct GraphNodeObject *graphNode, void *sp1c, Vec3f pos, Vec3s angle)
{
    vec3f_set(graphNode->scale, 1.0f, 1.0f, 1.0f);
    vec3f_copy(graphNode->pos, pos);
    vec3s_copy(graphNode->angle, angle);

    graphNode->asGraphNode = sp1c;
    graphNode->unk4C = 0;
    graphNode->throwMatrix = NULL;
    graphNode->unk38.curAnim = NULL;

    graphNode->node.flags |= GRAPH_RENDER_01;
    graphNode->node.flags &= ~GRAPH_RENDER_10;
    graphNode->node.flags |= GRAPH_RENDER_20;
    graphNode->node.flags &= ~GRAPH_RENDER_BILLBOARD;
}

void func_8037C51C(struct GraphNodeObject *graphNode, struct SpawnInfo *sp1c)
{
    vec3f_set(graphNode->scale, 1.0f, 1.0f, 1.0f);
    vec3s_copy(graphNode->angle, sp1c->startAngle);

    graphNode->pos[0] = (f32) sp1c->startPos[0];
    graphNode->pos[1] = (f32) sp1c->startPos[1];
    graphNode->pos[2] = (f32) sp1c->startPos[2];

    graphNode->unk18 = sp1c->areaIndex;
    graphNode->unk19 = sp1c->activeAreaIndex;
    graphNode->asGraphNode = sp1c->unk18;
    graphNode->unk4C = sp1c;
    graphNode->throwMatrix = NULL;
    graphNode->unk38.curAnim = 0;

    graphNode->node.flags |= GRAPH_RENDER_01;
    graphNode->node.flags &= ~GRAPH_RENDER_10;
    graphNode->node.flags |= GRAPH_RENDER_20;
    graphNode->node.flags &= ~GRAPH_RENDER_BILLBOARD;
}

void func_8037C658(struct GraphNodeObject *graphNode, void *sp34)
{
    void *sp2c = segmented_to_virtual(sp34);
    struct Animation *sp28 = segmented_to_virtual((void*)*(s32*)sp2c);

    if(graphNode->unk38.curAnim != sp28)
    {
        graphNode->unk38.curAnim = sp28;
        graphNode->unk38.animFrame = (sp28->unk04) + ((sp28->flags & ANIM_FLAG_1) ? 1 : -1);
        graphNode->unk38.animAccel = 0;
        graphNode->unk38.animYTrans = 0;
    }
}

void func_8037C708(struct GraphNodeObject *graphNode, void *sp34, u32 sp38)
{
    void **sp2c = segmented_to_virtual(sp34);
    struct Animation *sp28 = segmented_to_virtual(sp2c[0]);

    if(graphNode->unk38.curAnim != sp28)
    {
        graphNode->unk38.curAnim = sp28;
        graphNode->unk38.animYTrans = 0;
        graphNode->unk38.animFrameAccelAssist = (sp28->unk04 << 16) + ((sp28->flags & ANIM_FLAG_1) ? sp38 : -sp38);
        graphNode->unk38.animFrame = graphNode->unk38.animFrameAccelAssist >> 16;
    }

    graphNode->unk38.animAccel = sp38;
}

s32 func_8037C7D8(s32 a0, u16 **a1)
{
    s32 sp04;

    if(a0 < (*a1)[0])
    {
        sp04 = (*a1)[1] + a0;
    }
    else
    {
        sp04 = (*a1)[1] + (*a1)[0] - 1;
    }

    *a1 += 2;

    return sp04;
}

s16 func_8037C844(struct GraphNodeObject_sub *a0, s32* a1)
{
    s32 sp04;
    struct Animation *sp00;
    
    sp00 = a0->curAnim;

    if(a0->animTimer == gAreaUpdateCounter || sp00->flags & ANIM_FLAG_2)
    {
        if(a1 != NULL)
        {
            a1[0] = a0->animFrameAccelAssist;
        }

        return a0->animFrame;
    }

    if(sp00->flags & ANIM_FLAG_1)
    {
        if(a0->animAccel)
        {
            sp04 = a0->animFrameAccelAssist - a0->animAccel;
        }
        else
        {
            sp04 = (a0->animFrame - 1) << 16;
        }

        if(GET_HIGH_S16_OF_32(sp04) < sp00->unk06)
        {
            if(sp00->flags & ANIM_FLAG_NOLOOP)
            {
                SET_HIGH_S16_OF_32(sp04, sp00->unk06);
            }
            else
            {
                SET_HIGH_S16_OF_32(sp04, sp00->unk08 - 1);
            }
        }
    }
    else
    {
        if(a0->animAccel != 0)
        {
            sp04 = a0->animFrameAccelAssist + a0->animAccel;
        }
        else
        {
            sp04 = (a0->animFrame + 1) << 16;
        }

        if(GET_HIGH_S16_OF_32(sp04) >= sp00->unk08)
        {
            if(sp00->flags & ANIM_FLAG_NOLOOP)
            {
                SET_HIGH_S16_OF_32(sp04, sp00->unk08 - 1);
            }
            else
            {
                SET_HIGH_S16_OF_32(sp04, sp00->unk06);
            }
        }
    }

    if(a1 != 0)
    {
        a1[0] = sp04;
    }

    return GET_HIGH_S16_OF_32(sp04);
}

void Unknown8037C9E8(struct GraphNodeObject *sp28, Vec3f sp2c)
{
    struct Animation *sp24 = sp28->unk38.curAnim;
    u16 *sp20;
    s16 *sp1c;
    s16 sp1a;
    
    if(sp24 != NULL)
    {
        sp20 = segmented_to_virtual(sp24->index);
        sp1c = segmented_to_virtual(sp24->values);

        sp1a = sp28->unk38.animFrame;

        if(sp1a < 0)
        {
            sp1a = 0;
        }

        if(1) // ? necessary to match
        {
            sp2c[0] = (f32) sp1c[func_8037C7D8(sp1a, &sp20)];
            sp2c[1] = (f32) sp1c[func_8037C7D8(sp1a, &sp20)];
            sp2c[2] = (f32) sp1c[func_8037C7D8(sp1a, &sp20)];
        }
    }
    else
    {
        vec3f_set(sp2c, 0, 0, 0);
    }
}

struct GraphNodeScreenArea *Unknown8037CB10(struct GraphNode *graphNode)
{
    struct GraphNodeScreenArea *resGraphNode = NULL;

    while(graphNode->parent != NULL)
    {
        graphNode = graphNode->parent;
    }

    if(graphNode->type == GRAPH_NODE_TYPE_SCREEN_AREA)
    {
        resGraphNode = (struct GraphNodeScreenArea *) graphNode;
    }

    return resGraphNode;
}

s16 *read_vec3s_to_vec3f(Vec3f dst, s16 *src)
{
    dst[0] = *src++;
    dst[1] = *src++;
    dst[2] = *src++;
    return src;
}

s16 *read_vec3s(Vec3s dst, s16 *src)
{
    dst[0] = *src++;
    dst[1] = *src++;
    dst[2] = *src++;
    return src;
}

s16 *read_vec3s_angle(Vec3s dst, s16 *src)
{
    dst[0] = ((*src++) << 15) / 180;
    dst[1] = ((*src++) << 15) / 180;
    dst[2] = ((*src++) << 15) / 180;
    return src;
}

void register_scene_graph_node(struct GraphNode *graphNode)
{
    if(graphNode != NULL)
    {
        gCurGraphNodeList[gCurGraphNodeIndex] = graphNode;

        if(gCurGraphNodeIndex == 0)
        {
            if(gCurRootGraphNode == NULL)
            {
                gCurRootGraphNode = graphNode;
            }
        }
        else
        {
            if(gCurGraphNodeList[gCurGraphNodeIndex-1]->type == GRAPH_NODE_TYPE_029)
            {
                ((struct GraphNode029 *) gCurGraphNodeList[gCurGraphNodeIndex-1])->unk14 = graphNode;
            }
            else
            {
                func_8037C044(gCurGraphNodeList[gCurGraphNodeIndex-1], graphNode);
            }
        }
    }
}

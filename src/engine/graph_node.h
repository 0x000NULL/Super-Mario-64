#ifndef _GRAPH_NODE_H_
#define _GRAPH_NODE_H_

#include "types.h"
#include "game/memory.h"

struct AllocOnlyPool;

extern struct GraphNodeToggleZBuffer *D_8032CF94;
extern struct GraphNodeCamFrustum *D_8032CF98;
extern struct GraphNode114 *D_8032CF9C;
extern struct GraphNode12E *D_8032CFA4;
extern u16 gAreaUpdateCounter;

extern struct GraphNode *gCurRootGraphNode;
extern struct GraphNode *gCurGraphNodeList[];

extern s16 gCurGraphNodeIndex;

extern struct GraphNode D_8038BD88;

extern Vec3f D_80385FD0;
extern Vec3s D_80385FDC;
extern Vec3f D_80385FE4;
extern Vec3s unused80385FF0;

#define GRAPH_RENDER_01        (1 << 0)
#define GRAPH_RENDER_02        (1 << 1)
#define GRAPH_RENDER_BILLBOARD (1 << 2)
#define GRAPH_RENDER_Z_BUFFER  (1 << 3)
#define GRAPH_RENDER_10        (1 << 4)
#define GRAPH_RENDER_20        (1 << 5)

#define GRAPH_NODE_TYPE_FUNCTIONAL                                  0x100

#define GRAPH_NODE_TYPE_SCREEN_AREA                                 0x001
#define GRAPH_NODE_TYPE_002                                         0x002
#define GRAPH_NODE_TYPE_103                                        (0x003 | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_TOGGLE_Z_BUFFER                             0x004
#define GRAPH_NODE_TYPE_START                                       0x00A
#define GRAPH_NODE_TYPE_RENDER_RANGE                                0x00B
#define GRAPH_NODE_TYPE_SWITCH_CASE                                (0x00C | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_114                                        (0x014 | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_TRANSLATION_ROTATION_OPTIONAL_DISPLAY_LIST  0x015
#define GRAPH_NODE_TYPE_016                                         0x016
#define GRAPH_NODE_TYPE_017                                         0x017
#define GRAPH_NODE_TYPE_OBJECT                                      0x018
#define GRAPH_NODE_TYPE_DISPLAY_LIST_TRANSLATED                     0x019
#define GRAPH_NODE_TYPE_BILLBOARD_OPTIONAL_DISPLAY_LIST             0x01A
#define GRAPH_NODE_TYPE_DISPLAY_LIST                                0x01B
#define GRAPH_NODE_TYPE_SCALE_OPTIONAL_DISPLAY_LIST                 0x01C
#define GRAPH_NODE_TYPE_SHADOW                                      0x028
#define GRAPH_NODE_TYPE_029                                         0x029
#define GRAPH_NODE_TYPE_12A                                        (0x02A | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_BACKGROUND                                 (0x02C | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_12E                                        (0x02E | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_02F                                         0x02F

typedef s32 (*GraphNodeFunc)(s32 unk, struct GraphNode *node, void *);

struct FnGraphNode
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ GraphNodeFunc func;
};

struct GraphNodeScreenArea
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ u8 unk14;
    /*0x15*/ s8 unk15;
    /*0x16*/ s16 x;
    /*0x18*/ s16 y;
    /*0x1A*/ s16 width;
    /*0x1C*/ s16 height;
    /*0x1E*/ s16 unk1E;
    /*0x20*/ struct GraphNode **unk20; // TODO: check type
};

// related to background rendering
struct GraphNode002
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ f32 unk14;
};

struct GraphNodeCamFrustum
{
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unk18;
    /*0x1C*/ f32 fov;
    /*0x20*/ s16 near;
    /*0x22*/ s16 far;
};

struct GraphNodeToggleZBuffer_sub
{
    void *unk0;
    void *unk4;
    struct GraphNodeToggleZBuffer_sub *unk8;
};

struct GraphNodeToggleZBuffer
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ struct GraphNodeToggleZBuffer_sub *unk14[8];
    /*0x34*/ struct GraphNodeToggleZBuffer_sub *unk34[1];  // unknown length
    u8 filler38[0x54-0x38];
};

struct GraphNodeStart
{
    /*0x00*/ struct GraphNode node;
};

struct GraphNodeRenderRange
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ s16 minDistance;
    /*0x16*/ s16 maxDistance;
};

struct GraphNodeSwitchCase
{
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unk18;
    /*0x1C*/ s16 numCases;
    /*0x1E*/ s16 unk1E;
};

struct GraphNode114
{
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unk18;
    /*0x1C*/ Vec3f unk1C;
    /*0x28*/ Vec3f unk28;
    /*0x34*/ void *unk34; // matrix ptr
    /*0x38*/ s16 unk38;
    /*0x3A*/ s16 unk3A;
};

struct GraphNodeTranslationRotationOptionalDisplayList
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
    /*0x18*/ Vec3s translation;
    /*0x1E*/ Vec3s rotation;
};

struct GraphNode016
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
    /*0x18*/ Vec3s unk18;
    u8 pad1E[2];
};

struct GraphNode017
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
    /*0x18*/ Vec3s unk18;
    u8 pad1E[2];
};

struct GraphNodeDisplayListTranslated
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
    /*0x18*/ Vec3s translation;
};

struct GraphNodeBillboardOptionalDisplayList
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
    /*0x18*/ Vec3s translation;
};

struct GraphNodeDisplayList
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
};

struct GraphNodeScaleOptionalDisplayList
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *displayList;
    /*0x18*/ f32 scale;
};

struct GraphNodeShadow
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ s16 shadowScale;
    /*0x16*/ u8 shadowSolidity;
    /*0x17*/ u8 shadowType;
};

struct GraphNode029
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ struct GraphNode *unk14;
};

struct GraphNode12A
{
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ u32 unk18;
};

struct GraphNodeBackground
{
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unused;
    /*0x1C*/ s32 background; // background ID, or rgba5551 color if fnNode.func is null
};

struct GraphNode12E
{
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unk18;
    /*0x1C*/ struct GraphNodeObject *unk1C; // assumed type
    /*0x20*/ Vec3s unk20;
};

struct GraphNode02F
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ s16 unk14;
    u8 pad1E[2];
};

void init_scene_graph_node_links(struct GraphNode *, s32);

struct GraphNodeScreenArea *init_graph_node_screen_area(struct AllocOnlyPool *, struct GraphNodeScreenArea *,
    s16, s16 x, s16 y, s16 width, s16 height);
struct GraphNode002 *init_graph_node_002(struct AllocOnlyPool *, struct GraphNode002 *, f32);
struct GraphNodeCamFrustum *init_graph_node_cam_frustum(struct AllocOnlyPool *pool, struct GraphNodeCamFrustum *sp1c,
    f32 sp20, s16 sp26, s16 sp2a, GraphNodeFunc sp2c, s32 sp30);
struct GraphNodeStart *init_graph_node_start(struct AllocOnlyPool *pool, struct GraphNodeStart *sp1c);
struct GraphNodeToggleZBuffer *init_graph_node_toggle_z_buffer(struct AllocOnlyPool *pool, struct GraphNodeToggleZBuffer *, s16 sp22);
struct GraphNodeRenderRange *init_graph_node_render_range(struct AllocOnlyPool *pool, struct GraphNodeRenderRange *graphNode,
    s16 minDistance, s16 maxDistance);
struct GraphNodeSwitchCase *init_graph_node_switch_case(struct AllocOnlyPool *pool, struct GraphNodeSwitchCase *graphNode,
    s16 numCases, s16 sp26, GraphNodeFunc nodeFunc, s32 sp2c);
struct GraphNode114 *init_graph_node_114(struct AllocOnlyPool *pool, struct GraphNode114 * sp1c,
    f32 *sp20, f32 *sp24, GraphNodeFunc sp28, s32 sp2c);
struct GraphNodeTranslationRotationOptionalDisplayList *init_graph_node_translation_rotation_optional_display_list(struct AllocOnlyPool *pool,
    struct GraphNodeTranslationRotationOptionalDisplayList *graphNode, s32 drawingLayer, void *displayList, Vec3s sp28, Vec3s sp2c);
struct GraphNode016 *init_graph_node_016(struct AllocOnlyPool *pool, struct GraphNode016 *graphNode,
    s32 drawingLayer, void *displayList, Vec3s sp28);
struct GraphNode017 *init_graph_node_017(struct AllocOnlyPool *pool, struct GraphNode017 *graphNode,
    s32 drawingLayer, void *displayList, Vec3s sp28);
struct GraphNodeScaleOptionalDisplayList *init_graph_node_scale_optional_display_list(struct AllocOnlyPool *pool,
    struct GraphNodeScaleOptionalDisplayList *graphNode, s32 drawingLayer, void *displayList, f32 sp28);
struct GraphNodeObject *init_graph_node_object(struct AllocOnlyPool *pool, struct GraphNodeObject *graphNode,
    struct GraphNode *sp20, Vec3f pos, Vec3s angle, Vec3f scale);
struct GraphNode02F *init_graph_node_02F(struct AllocOnlyPool *pool, struct GraphNode02F *sp1c,
    s16 sp22);
struct GraphNodeDisplayListTranslated *init_graph_node_display_list_translated(struct AllocOnlyPool *pool, struct GraphNodeDisplayListTranslated * graphNode,
    s32 drawingLayer, void *displayList, Vec3s relativePos);
struct GraphNodeBillboardOptionalDisplayList *init_graph_node_billboard_optional_display_list(struct AllocOnlyPool *pool,
    struct GraphNodeBillboardOptionalDisplayList *graphNode, s32 drawingLayer, void *displayList, Vec3s sp28);
struct GraphNodeDisplayList *init_graph_node_display_list(struct AllocOnlyPool *pool, struct GraphNodeDisplayList *graphNode,
    s32 drawingLayer, void *displayList);
struct GraphNodeShadow *init_graph_node_shadow(struct AllocOnlyPool *pool, struct GraphNodeShadow *sp1c,
    s16 sp22, u8 sp27, u8 sp2b);
struct GraphNode029 *init_graph_node_029(struct AllocOnlyPool *pool, struct GraphNode029 *sp1c,
    struct GraphNode *sp20);
struct GraphNode12A *init_graph_node_12A(struct AllocOnlyPool *pool, struct GraphNode12A *sp1c,
    GraphNodeFunc sp20, s32 sp24);
struct GraphNodeBackground *init_graph_node_background(struct AllocOnlyPool *pool, struct GraphNodeBackground *sp1c,
    u16 sp22, GraphNodeFunc sp24, s32 sp28);
struct GraphNode12E *init_graph_node_12E(struct AllocOnlyPool *pool, struct GraphNode12E *sp1c,
    s32 sp20, Vec3s sp24, GraphNodeFunc sp28, s32 sp2c);

struct GraphNode *func_8037C044(struct GraphNode *, struct GraphNode *);
struct GraphNode *func_8037C0BC(struct GraphNode *);
struct GraphNode *func_8037C138(struct GraphNode *a0);

void func_8037C1E4(struct GraphNode *, s32);
void func_8037C360(struct GraphNode *graphNode, s32 sp1c);

void func_8037C3D0(struct GraphNodeObject *sp20);
void func_8037C448(struct GraphNodeObject *sp18, void *sp1c, Vec3f sp20, Vec3s sp24);
void func_8037C51C(struct GraphNodeObject *sp18, struct SpawnInfo *sp1c);
void func_8037C658(struct GraphNodeObject *, void *);
void func_8037C708(struct GraphNodeObject *sp30, void *sp34, u32 sp38);

s32 func_8037C7D8(s32 a0, u16 **a1);

s16 func_8037C844(struct GraphNodeObject_sub *a0, s32* a1);
void Unknown8037C9E8(struct GraphNodeObject *sp28, Vec3f sp2c);

struct GraphNodeScreenArea *Unknown8037CB10(struct GraphNode *graphNode);

s16 *read_vec3s_to_vec3f(Vec3f, s16 *src);
s16 *read_vec3s(Vec3s dst, s16 *src);
s16 *read_vec3s_angle(Vec3s dst, s16 *src);

void register_scene_graph_node(struct GraphNode *);

#endif /* _GRAPH_NODE_H_ */

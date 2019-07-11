#include <ultra64.h>
#include "sm64.h"

#include "geo_layout.h"
#include "math_util.h"
#include "game/memory.h"
#include "graph_node.h"



typedef void (*GeoLayoutCommandProc)(void);

GeoLayoutCommandProc GeoLayoutJumpTable[] = 
{
    geo_layout_cmd_branch_and_link,
    geo_layout_cmd_end,
    geo_layout_cmd_branch,
    geo_layout_cmd_return,
    geo_layout_cmd_open_node,
    geo_layout_cmd_close_node,
    geo_layout_cmd_06,
    geo_layout_cmd_update_node_flags,
    geo_layout_cmd_node_screen_area,
    geo_layout_cmd_09,
    geo_layout_cmd_node_cam_frustum,
    geo_layout_cmd_node_start,
    geo_layout_cmd_node_toggle_z_buffer,
    geo_layout_cmd_node_render_range,
    geo_layout_cmd_node_switch_case,
    geo_layout_cmd_0F,
    geo_layout_cmd_node_translation_rotation_optional_display_list,
    geo_layout_cmd_node_translation_node_optional_display_list,
    geo_layout_cmd_node_rotation_node_optional_display_list,
    geo_layout_cmd_node_display_list_translated,
    geo_layout_cmd_node_billboard_optional_display_list,
    geo_layout_cmd_node_display_list,
    geo_layout_cmd_node_shadow,
    geo_layout_cmd_node_render_obj,
    geo_layout_cmd_node_poly_addr,
    geo_layout_cmd_node_background,
    geo_layout_cmd_nop,
    geo_layout_cmd_1B,
    geo_layout_cmd_1C,
    geo_layout_cmd_node_scale,
    geo_layout_cmd_nop2,
    geo_layout_cmd_nop3,
    geo_layout_cmd_20,
};

struct AllocOnlyPool *gGraphNodePool;
struct GraphNode *gCurRootGraphNode;
UNUSED s32 D_8038BCA8;
struct GraphNode **D_8038BCAC;
u16 D_8038BCB0;
u32 gGeoLayoutStack[16];
struct GraphNode *gCurGraphNodeList[32];
s16 gCurGraphNodeIndex;
s16 gGeoLayoutStackIndex;
UNUSED s16 D_8038BD7C;
s16 D_8038BD7E;
u8 *gGeoLayoutCommand;
struct GraphNode D_8038BD88;

u32 unused_8038B894[3] = { 0 };

extern Vec3s D_80385FDC;
extern Vec3f D_80385FE4;

/*
  0x00: Branch and store return address
   cmd+0x04: void *branchTarget
*/
void geo_layout_cmd_branch_and_link(void)
{
    gGeoLayoutStack[gGeoLayoutStackIndex++] = (u32) &gGeoLayoutCommand[8];
    gGeoLayoutStack[gGeoLayoutStackIndex++] = (gCurGraphNodeIndex << 16) + D_8038BD7E;
    D_8038BD7E = gGeoLayoutStackIndex;
    gGeoLayoutCommand = (u8 *) segmented_to_virtual((void *) cur_geo_cmd_s32(0x04));
}

// 0x01: Terminate geo layout
void geo_layout_cmd_end(void)
{
    gGeoLayoutStackIndex = D_8038BD7E;
    D_8038BD7E = gGeoLayoutStack[--gGeoLayoutStackIndex] & 0xFFFF;
    gCurGraphNodeIndex = gGeoLayoutStack[gGeoLayoutStackIndex] >> 16;
    gGeoLayoutCommand = (u8 *) gGeoLayoutStack[--gGeoLayoutStackIndex];
}

/*
  0x02: Branch
   cmd+0x04: void *branchTarget
*/
void geo_layout_cmd_branch(void)
{
    if(cur_geo_cmd_u8(0x01) == 1)
    {
        gGeoLayoutStack[gGeoLayoutStackIndex++] = (u32) &gGeoLayoutCommand[8];
    }

    gGeoLayoutCommand = (u8 *) segmented_to_virtual((void *) cur_geo_cmd_s32(0x04));
}

// 0x03: Return from branch
void geo_layout_cmd_return(void)
{
    gGeoLayoutCommand = (u8 *) gGeoLayoutStack[--gGeoLayoutStackIndex];
}

// 0x04: Open node
void geo_layout_cmd_open_node(void)
{
    gCurGraphNodeList[gCurGraphNodeIndex + 1] = gCurGraphNodeList[gCurGraphNodeIndex];
    gCurGraphNodeIndex++;
    gGeoLayoutCommand += 0x04;
}

// 0x05: Close node
void geo_layout_cmd_close_node(void)
{
    gCurGraphNodeIndex--;
    gGeoLayoutCommand += 0x04;
}

/*
  0x06: ?
   cmd+0x02: index (of D_8038BCAC)

  set D_8038BCAC[index] to the current scene graph node if index < D_8038BCB0
*/
void geo_layout_cmd_06(void)
{
    u16 index = cur_geo_cmd_s16(0x02);

    // if index is less than num allocated (see geo_layout_cmd_node_screen_area)
    if(index < D_8038BCB0)
    {
        D_8038BCAC[index] = gCurGraphNodeList[gCurGraphNodeIndex];
    }

    gGeoLayoutCommand += 0x04;
}

/*
  0x07: Update current scene graph node flags
   cmd+0x01: u8 operation (0 = reset, 1 = set, 2 = clear)
   cmd+0x02: s16 bits
*/
void geo_layout_cmd_update_node_flags(void)
{
    u16 operation = cur_geo_cmd_u8(0x01);
    u16 flagBits = cur_geo_cmd_s16(0x02);

    switch(operation)
    {
    case GEO_CMD_FLAGS_RESET:
        gCurGraphNodeList[gCurGraphNodeIndex]->flags = flagBits;
        break;
    case GEO_CMD_FLAGS_SET:
        gCurGraphNodeList[gCurGraphNodeIndex]->flags |= flagBits;
        break;
    case GEO_CMD_FLAGS_CLEAR:
        gCurGraphNodeList[gCurGraphNodeIndex]->flags &= ~flagBits;
        break;
    }

    gGeoLayoutCommand += 0x04;
}

/*
  0x08: Create screen area scene graph node
   cmd+0x02: s16 num entries (+2) to allocate for D_8038BCAC
   cmd+0x04: s16 x
   cmd+0x06: s16 y
   cmd+0x08: s16 width
   cmd+0x0A: s16 height
*/
void geo_layout_cmd_node_screen_area(void)
{
    s32 sp34;
    struct GraphNodeScreenArea *graphNode;

    s16 x = cur_geo_cmd_s16(0x04);
    s16 y = cur_geo_cmd_s16(0x06);
    s16 width = cur_geo_cmd_s16(0x08);
    s16 height = cur_geo_cmd_s16(0x0A);
    
    // number of entries to allocate for D_8038BCAC array
    // at least 2 are allocated by default
    // cmd+0x02 = 0x00: mario face, 0x0A: all other levels
    D_8038BCB0 = cur_geo_cmd_s16(0x02) + 2;

    graphNode = init_graph_node_screen_area(gGraphNodePool, NULL, 0, x, y, width, height);

    // TODO: check type
    D_8038BCAC = (struct GraphNode **) alloc_only_pool_alloc(gGraphNodePool, D_8038BCB0 * sizeof(s32));

    graphNode->unk20 = D_8038BCAC;
    graphNode->unk1E = D_8038BCB0;
    
    // clear D_8038BCAC array
    for(sp34 = 0; sp34 < D_8038BCB0; sp34++)
    {
        D_8038BCAC[sp34] = NULL;
    }

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x0C;
}

/*
  0x09: Create ? scene graph node (GraphNode002)
   cmd+0x02: s16 ?
*/
void geo_layout_cmd_09(void)
{
    struct GraphNode002 *graphNode;
    f32 sp18 = (f32) cur_geo_cmd_s16(0x02) / 100.0f;
    
    graphNode = init_graph_node_002(gGraphNodePool, NULL, sp18);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04;
}

/*
  0x0A: Create camera frustum scene graph node
   cmd+0x01: u8  if nonzero, enable frustumFunc field
   cmd+0x02: s16 field of view
   cmd+0x04: s16 near
   cmd+0x06: s16 far
   [cmd+0x08: GraphNodeFunc frustumFunc]
*/
void geo_layout_cmd_node_cam_frustum(void)
{
    struct GraphNodeCamFrustum *graphNode;
    GraphNodeFunc frustumFunc = NULL;
    s16 fov = cur_geo_cmd_s16(0x02);
    s16 near = cur_geo_cmd_s16(0x04);
    s16 far = cur_geo_cmd_s16(0x06);

    if(cur_geo_cmd_u8(0x01) != 0)
    {
        // optional asm function
        frustumFunc = (GraphNodeFunc) cur_geo_cmd_s32(0x08);
        gGeoLayoutCommand += 0x04;
    }

    graphNode = init_graph_node_cam_frustum(gGraphNodePool, NULL, (f32)fov, near, far, frustumFunc, 0);

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08;
}

// 0x0B: Create a root scene graph node
void geo_layout_cmd_node_start(void)
{
    struct GraphNodeStart *graphNode;
    
    graphNode = init_graph_node_start(gGraphNodePool, NULL);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04;
}

// 0x1F: No operation
void geo_layout_cmd_nop3(void)
{
    gGeoLayoutCommand += 0x10;
}

/*
  0x0C: Create zbuffer-toggling scene graph node
   cmd+0x01: u8 enableZBuffer (1 = on, 0 = off)
*/
void geo_layout_cmd_node_toggle_z_buffer(void)
{
    struct GraphNodeToggleZBuffer *graphNode;
    
    graphNode = init_graph_node_toggle_z_buffer(gGraphNodePool, NULL, cur_geo_cmd_u8(0x01));

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04;
}

/*
  0x0D: Create render range scene graph node
   cmd+0x04: s16 minDistance
   cmd+0x06: s16 maxDistance
*/
void geo_layout_cmd_node_render_range(void)
{
    struct GraphNodeRenderRange *graphNode;
    s16 minDistance = cur_geo_cmd_s16(0x04);
    s16 maxDistance = cur_geo_cmd_s16(0x06);

    graphNode = init_graph_node_render_range(gGraphNodePool, NULL, minDistance, maxDistance);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08;
}

/*
  0x0E: Create switch-case scene graph node
   cmd+0x02: s16 numCases
   cmd+0x04: GraphNodeFunc caseSelectorFunc
  
  caseSelectorFunc returns an index which is used to select the node to render.
  Used for animating coins, blinking, color selection, etc.
*/
void geo_layout_cmd_node_switch_case(void)
{
    struct GraphNodeSwitchCase *graphNode;
    
    graphNode = init_graph_node_switch_case(gGraphNodePool, NULL,
        cur_geo_cmd_s16(0x02), // num cases
        0,
        (GraphNodeFunc) cur_geo_cmd_s32(0x04), // asm routine
        0);

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08;
}

/*
 0x0F: Create ? scene graph node (GraphNode114)
  cmd+0x02: s16 ?
  cmd+0x04: s16 unkX
  cmd+0x06: s16 unkY
  cmd+0x08: s16 unkZ
  cmd+0x0A: s16 unkX_2
  cmd+0x0C: s16 unkY_2
  cmd+0x0E: s16 unkZ_2
  cmd+0x10: GraphNodeFunc func
*/
void geo_layout_cmd_0F(void)
{
    struct GraphNode114 *graphNode;
    s16 *cmdPos = (s16 *) &gGeoLayoutCommand[4];

    Vec3f sp2c, sp20;

    cmdPos = read_vec3s_to_vec3f(sp2c, cmdPos);
    cmdPos = read_vec3s_to_vec3f(sp20, cmdPos);

    graphNode = init_graph_node_114(gGraphNodePool, NULL, sp2c, sp20,
        (GraphNodeFunc) cur_geo_cmd_s32(0x10),
        cur_geo_cmd_s16(0x02));

    register_scene_graph_node(&graphNode->fnNode.node);

    D_8038BCAC[0] = &graphNode->fnNode.node;

    gGeoLayoutCommand += 0x14;
}

/*
  0x10: Create translation & rotation scene graph node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     ((params & 0x70)>>4): fieldLayout
     (params & 0x0F): drawingLayer
  
   fieldLayout == 0:
    cmd+0x04: s16 xTranslation
    cmd+0x06: s16 yTranslation
    cmd+0x08: s16 zTranslation
    cmd+0x0A: s16 xRotation
    cmd+0x0C: s16 yRotation
    cmd+0x0E: s16 zRotation
  
   fieldLayout == 1:
    cmd+0x02: s16 xTranslation
    cmd+0x04: s16 yTranslation
    cmd+0x06: s16 zTranslation
    (rotation gets copied from D_80385FDC)
  
   fieldLayout == 2:
    cmd+0x02: s16 xRotation
    cmd+0x04: s16 yRotation
    cmd+0x06: s16 zRotation
    (translation gets copied from D_80385FDC)
  
   fieldLayout == 3:
    cmd+0x02: s16 yRotation
    (translation gets copied from D_80385FDC)
    (x and z translation are set to 0)
  
   [cmd+var: void *displayList]
*/
void geo_layout_cmd_node_translation_rotation_optional_display_list(void)
{
    struct GraphNodeTranslationRotationOptionalDisplayList *graphNode;

    Vec3s translation, rotation;

    void *displayList = NULL;
    s16 drawingLayer = 0;

    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;

    switch((params & 0x70) >> 4)
    {
    case 0:
        cmdPos = read_vec3s(translation, &cmdPos[2]);
        cmdPos = read_vec3s_angle(rotation, cmdPos);
        break;
    case 1:
        cmdPos = read_vec3s(translation, &cmdPos[1]);
        vec3s_copy(rotation, D_80385FDC);
        break;
    case 2:
        cmdPos = read_vec3s_angle(rotation, &cmdPos[1]);
        vec3s_copy(translation, D_80385FDC);
        break;
    case 3:
        vec3s_copy(translation, D_80385FDC);
        vec3s_set(rotation, 0, (cmdPos[1] << 15) / 180, 0);
        cmdPos += 2;
        break;
    }

    if(params & 0x80)
    {
        displayList = *(void **)&cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2;
    }

    graphNode = init_graph_node_translation_rotation_optional_display_list(gGraphNodePool, NULL, drawingLayer, displayList, translation, rotation);
    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x11: Create translation scene graph node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x02: s16 unkX
   cmd+0x04: s16 unkY
   cmd+0x06: s16 unkZ
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_translation_node_optional_display_list(void)
{
    struct GraphNode016 *graphNode;

    Vec3s sp2c;

    s16 drawingLayer = 0;
    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;
    void *displayList = NULL;

    cmdPos = read_vec3s(sp2c, &cmdPos[1]);

    if(params & 0x80)
    {
        displayList = *(void **)&cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2;
    }

    graphNode = init_graph_node_016(gGraphNodePool, NULL, drawingLayer, displayList, sp2c);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x12: Create ? scene graph node
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x02: s16 unkX
   cmd+0x04: s16 unkY
   cmd+0x06: s16 unkZ
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_rotation_node_optional_display_list(void)
{
    struct GraphNode017 *graphNode;

    Vec3s sp2c;

    s16 drawingLayer = 0;
    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;
    void *displayList = NULL;

    cmdPos = read_vec3s_angle(sp2c, &cmdPos[1]);

    if(params & 0x80)
    {
        displayList = *(void **)&cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2;
    }

    graphNode = init_graph_node_017(gGraphNodePool, NULL, drawingLayer, displayList, sp2c);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x1D: Create scale scene graph node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x04: u32 scale (0x10000 = 1.0)
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_scale(void)
{
    struct GraphNodeScaleOptionalDisplayList *graphNode;

    s16 drawingLayer = 0;
    s16 params = cur_geo_cmd_u8(0x01);
    f32 scale = cur_geo_cmd_u32(0x04) / 65536.0f;
    void *displayList = NULL;

    if(params & 0x80)
    {
        displayList = (void *) cur_geo_cmd_s32(0x08);
        drawingLayer = params & 0x0F;
        gGeoLayoutCommand += 0x04;
    }

    graphNode = init_graph_node_scale_optional_display_list(gGraphNodePool, NULL, drawingLayer, displayList, scale);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08;
}

// 0x1E: No operation
void geo_layout_cmd_nop2(void)
{
    gGeoLayoutCommand += 0x08;
}

/*
  0x13: Create display list scene graph node with translation
   cmd+0x01: u8 drawingLayer
   cmd+0x02: s16 xTranslation
   cmd+0x04: s16 yTranslation
   cmd+0x06: s16 zTranslation
   cmd+0x08: void *displayList
*/
void geo_layout_cmd_node_display_list_translated(void)
{
    struct GraphNodeDisplayListTranslated *graphNode;
    Vec3s relativePos;
    s32 drawingLayer = cur_geo_cmd_u8(0x01);
    void *displayList = (void *) cur_geo_cmd_s32(0x08);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;

    read_vec3s(relativePos, &cmdPos[1]);

    graphNode = init_graph_node_display_list_translated(gGraphNodePool, NULL, drawingLayer, displayList, relativePos);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x0C;
}

/*
  0x14: Create billboarding node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x02: s16 xTranslation
   cmd+0x04: s16 yTranslation
   cmd+0x06: s16 zTranslation
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_billboard_optional_display_list(void)
{
    struct GraphNodeBillboardOptionalDisplayList *graphNode;
    Vec3s translation;
    s16 drawingLayer = 0;
    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;
    void *displayList = NULL;

    cmdPos = read_vec3s(translation, &cmdPos[1]);

    if(params & 0x80)
    {
        displayList = *(void **)&cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2;
    }

    graphNode = init_graph_node_billboard_optional_display_list(gGraphNodePool, NULL, drawingLayer, displayList, translation);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x15: Create plain display list scene graph node
   cmd+0x01: u8 drawingLayer
   cmd+0x04: void *displayList
*/
void geo_layout_cmd_node_display_list(void)
{
    struct GraphNodeDisplayList *graphNode;
    s32 drawingLayer = cur_geo_cmd_u8(0x01);
    void *displayList = (void *) cur_geo_cmd_s32(0x04);

    graphNode = init_graph_node_display_list(gGraphNodePool, NULL, drawingLayer, displayList);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08;
}

/*
  0x16: Create shadow scene graph node
   cmd+0x02: s16 shadowType
   cmd+0x04: s16 shadowSolidity
   cmd+0x06: s16 shadowScale
*/
void geo_layout_cmd_node_shadow(void)
{
    struct GraphNodeShadow *graphNode;
    u8 shadowType = cur_geo_cmd_s16(0x02);
    u8 shadowSolidity = cur_geo_cmd_s16(0x04);
    s16 shadowScale = cur_geo_cmd_s16(0x06);

    graphNode = init_graph_node_shadow(gGraphNodePool, NULL, shadowScale, shadowSolidity, shadowType);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08;
}

// 0x17: Create render object scene graph node (GraphNode029)
void geo_layout_cmd_node_render_obj(void)
{
    struct GraphNode029 *graphNode;

    graphNode = init_graph_node_029(gGraphNodePool, NULL, &D_8038BD88);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04;
}

/*
  0x18: Create polygons address scene graph node (GraphNode12A)
   cmd+0x02: s16 parameter
   cmd+0x04: GraphNodeFunc func
*/
void geo_layout_cmd_node_poly_addr(void)
{
    struct GraphNode12A *graphNode;

    graphNode = init_graph_node_12A(gGraphNodePool, NULL,
        (GraphNodeFunc) cur_geo_cmd_s32(0x04), // asm function
        cur_geo_cmd_s16(0x02)); // parameter

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08;
}

/*
  0x19: Create background scene graph node
   cmd+0x02: s16 background // background ID, or RGBA5551 color if backgroundFunc is null
   cmd+0x04: GraphNodeFunc backgroundFunc
*/
void geo_layout_cmd_node_background(void)
{
    struct GraphNodeBackground *graphNode;

    graphNode = init_graph_node_background(gGraphNodePool, NULL,
        cur_geo_cmd_s16(0x02), // background ID, or RGBA5551 color if asm function is null
        (GraphNodeFunc) cur_geo_cmd_s32(0x04), // asm function
        0);
    
    register_scene_graph_node(&graphNode->fnNode.node);
    
    gGeoLayoutCommand += 0x08;
}

// 0x1A: No operation
void geo_layout_cmd_nop(void)
{
    gGeoLayoutCommand += 0x08;
}

/*
  0x1B: Create ? scene graph node (GraphNode029)
   cmd+0x02: s16 index (of D_8038BCAC)
 
  if D_8038BCAC[index]'s type is GRAPH_NODE_TYPE_029,
  copy D_8038BCAC[index]->unk14 to graphNode->unk14
*/
void geo_layout_cmd_1B(void)
{
    struct GraphNode029 *graphNode;
    struct GraphNode *sp20 = NULL;
    s16 index = cur_geo_cmd_s16(0x02);

    if(index >= 0)
    {
        sp20 = D_8038BCAC[index];

        if(sp20->type == GRAPH_NODE_TYPE_029)
        {
            sp20 = ((struct GraphNode029 *)sp20)->unk14;
        }
        else
        {
            sp20 = NULL;
        }
    }

    graphNode = init_graph_node_029(gGraphNodePool, NULL, sp20);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04;
}

/*
  0x1C: Create ? scene graph node (GraphNode12E)
   cmd+0x01: u8 unk01
   cmd+0x02: s16 unkX
   cmd+0x04: s16 unkY
   cmd+0x06: s16 unkZ
   cmd+0x08: GraphNodeFunc nodeFunc
*/
void geo_layout_cmd_1C(void)
{
    struct GraphNode12E *graphNode;
    Vec3s sp24;

    read_vec3s(sp24, (s16 *) &gGeoLayoutCommand[0x02]);

    graphNode = init_graph_node_12E(gGraphNodePool, NULL, 0, sp24,
        (GraphNodeFunc) cur_geo_cmd_s32(0x08),
        cur_geo_cmd_u8(0x01));

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x0C;
}

/*
  0x20: Create render distance scene graph node (unconfirmed?)
   cmd+0x02: s16 renderDistance?
*/
void geo_layout_cmd_20(void)
{
    struct GraphNode02F *graphNode;
    graphNode = init_graph_node_02F(gGraphNodePool, NULL, cur_geo_cmd_s16(0x02));
    register_scene_graph_node(&graphNode->node);
    gGeoLayoutCommand += 0x04;
}

struct GraphNode *process_geo_layout(struct AllocOnlyPool *pool, void *segptr)
{
    // set by register_scene_graph_node when gCurGraphNodeIndex is 0
    // and gCurRootGraphNode is NULL
    gCurRootGraphNode = NULL;

    D_8038BCB0 = 0; // number of entries in D_8038BCAC

    gCurGraphNodeList[0] = 0;
    gCurGraphNodeIndex = 0; // incremented by cmd_open_node, decremented by cmd_close_node

    gGeoLayoutStackIndex = 2;
    D_8038BD7E = 2; // stack index is often copied here?

    gGeoLayoutCommand = (u8 *) segmented_to_virtual(segptr);

    gGraphNodePool = pool; 

    gGeoLayoutStack[0] = 0;
    gGeoLayoutStack[1] = 0;

    while(gGeoLayoutCommand != NULL)
    {
        GeoLayoutJumpTable[gGeoLayoutCommand[0x00]]();
    }

    return gCurRootGraphNode;
}

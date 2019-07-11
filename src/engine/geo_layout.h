#ifndef _GEO_LAYOUT_H_
#define _GEO_LAYOUT_H_

#include "game/memory.h"

#define GEO_CMD_FLAGS_RESET 0
#define GEO_CMD_FLAGS_SET   1
#define GEO_CMD_FLAGS_CLEAR 2

#define cur_geo_cmd_u8(offset) \
    (gGeoLayoutCommand[offset])

#define cur_geo_cmd_s16(offset) \
    (*(s16 *) &gGeoLayoutCommand[offset])

#define cur_geo_cmd_s32(offset) \
    (*(s32 *) &gGeoLayoutCommand[offset])

#define cur_geo_cmd_u32(offset) \
    (*(u32 *) &gGeoLayoutCommand[offset])

extern struct AllocOnlyPool *gGraphNodePool;
extern struct GraphNode *gCurRootGraphNode;
extern UNUSED s32 D_8038BCA8;
extern struct GraphNode **D_8038BCAC;
extern u16 D_8038BCB0;
extern u32 gGeoLayoutStack[];
extern struct GraphNode *gCurGraphNodeList[];
extern s16 gCurGraphNodeIndex;
extern s16 gGeoLayoutStackIndex;
extern UNUSED s16 D_8038BD7C;
extern s16 D_8038BD7E;
extern u8 *gGeoLayoutCommand;
extern struct GraphNode D_8038BD88;

extern struct AllocOnlyPool *D_8038BCA0;
extern struct GraphNode *D_8038BCA4;
extern s16 D_8038BD78;
extern struct GraphNode *D_8038BCF8[];

extern u32 D_8038B8A0;
extern u16 D_8038B8A4;
extern u16 D_8038B8A8;
extern s16 D_8038B8AC;


void geo_layout_cmd_branch_and_link(void);
void geo_layout_cmd_end(void);
void geo_layout_cmd_branch(void);
void geo_layout_cmd_return(void);
void geo_layout_cmd_open_node(void);
void geo_layout_cmd_close_node(void);
void geo_layout_cmd_06(void);
void geo_layout_cmd_update_node_flags(void);
void geo_layout_cmd_node_screen_area(void);
void geo_layout_cmd_09(void);
void geo_layout_cmd_node_cam_frustum(void);
void geo_layout_cmd_node_start(void);
void geo_layout_cmd_nop3(void);
void geo_layout_cmd_node_toggle_z_buffer(void);
void geo_layout_cmd_node_render_range(void);
void geo_layout_cmd_node_switch_case(void);
void geo_layout_cmd_0F(void);
void geo_layout_cmd_node_translation_rotation_optional_display_list(void);
void geo_layout_cmd_node_translation_node_optional_display_list(void);
void geo_layout_cmd_node_rotation_node_optional_display_list(void);
void geo_layout_cmd_node_scale(void);
void geo_layout_cmd_nop2(void);
void geo_layout_cmd_node_display_list_translated(void);
void geo_layout_cmd_node_billboard_optional_display_list(void);
void geo_layout_cmd_node_display_list(void);
void geo_layout_cmd_node_shadow(void);
void geo_layout_cmd_node_render_obj(void);
void geo_layout_cmd_node_poly_addr(void);
void geo_layout_cmd_node_background(void);
void geo_layout_cmd_nop(void);
void geo_layout_cmd_1B(void);
void geo_layout_cmd_1C(void);
void geo_layout_cmd_20(void);

struct GraphNode *process_geo_layout(struct AllocOnlyPool *a0, void *segptr);

#endif /* _GEO_LAYOUT_H_ */

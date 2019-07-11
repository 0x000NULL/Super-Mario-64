#ifndef RENDERING_GRAPH_NODE_H
#define RENDERING_GRAPH_NODE_H

#include "engine/graph_node.h"

extern struct GraphNodeScreenArea *D_8032CF90;
extern struct GraphNodeToggleZBuffer *D_8032CF94;
extern struct GraphNodeCamFrustum *D_8032CF98;
extern struct GraphNode114 *D_8032CF9C;
extern struct GraphNodeObject *D_8032CFA0;
extern struct GraphNode12E *D_8032CFA4;
extern u16 gAreaUpdateCounter;



extern void func_8027D8F8(struct GraphNode *rootGraphNode);
extern void func_8027DB80();

#endif // RENDERING_GRAPH_NODE_H

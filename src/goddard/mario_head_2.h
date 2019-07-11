#ifndef _MARIO_HEAD_2_H_
#define _MARIO_HEAD_2_H_

#include "types.h"
#include "gd_types.h"

/****** Static .bss ******/
// struct ObjWeight* sSkinNetCurWeight; // D_801B9EA0
// Mat4 D_801B9EA8;
// struct ObjJoint* D_801B9EE8;

/****** Possibly Declared Here But Not Used .bss ******/
// extern ? D_801B5D50;
// extern ? D_801B5D7C;

extern void func_80181760(struct ObjGroup *);
extern void move_skin(struct ObjNet *);
extern void func_80181894(struct ObjJoint *);
extern void Unknown80181B88(struct ObjJoint *);

// extern void Unknown801815E0(Mat4*);
// extern void Unknown801819D0(struct ObjVertex*);
// extern void reset_weight(struct ObjWeight*);

#endif /* _MARIO_HEAD_2_H_ */

#ifndef _MARIO_HEAD_3_H_
#define _MARIO_HEAD_3_H_

#include "types.h"
#include "gd_types.h"

// functions
extern void func_801823A0(struct ObjNet *);
extern struct ObjParticle* make_particle(u32, s32, f32, f32, f32);
extern void move_particles_in_grp(struct ObjGroup *);

#endif /* _MARIO_HEAD_3_H_ */

#ifndef _MARIO_STEP_H
#define _MARIO_STEP_H

#include "types.h"

extern struct Surface gWaterSurfacePseudoFloor;

extern f32 zero_80254E20(void);
extern void nop_80254E3C(struct MarioState *);
extern void nop_80254E50(void);

extern void mario_bonk_reflection(struct MarioState *, u32);
extern u32 mario_update_quicksand(struct MarioState *, f32);
extern u32 mario_push_off_steep_floor(struct MarioState *, u32, u32);
extern u32 mario_update_moving_sand(struct MarioState *);
extern u32 mario_update_windy_ground(struct MarioState *);
extern void stop_and_set_height_to_floor(struct MarioState *);
extern u32 stationary_ground_step(struct MarioState *);
extern u32 perform_ground_step(struct MarioState *);
extern u32 perform_air_step(struct MarioState *, u32);

#endif /* _MARIO_STEP_H */

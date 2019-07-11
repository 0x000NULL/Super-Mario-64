#ifndef _MARIO_H
#define _MARIO_H

#include "types.h"

extern u32 D_80226EB8;
extern u32 D_80335280;
extern u32 D_80335288;
extern u32 D_80335290;

extern struct Object *gMarioObject;
extern struct Object *gLuigiObject;

extern s32 func_80250770(struct MarioState *);
extern s32 func_802507AC(struct MarioState *);
extern s16 set_mario_animation(struct MarioState *, s32);
extern s16 func_80250934(struct MarioState *, s32, s32);
extern void func_80250AAC(struct MarioState *, s16);
extern s32 func_80250B68(struct MarioState *, s16);
extern s16 func_80250C84(struct Object *, s32, Vec3s);
extern void func_80250E50(struct MarioState *);
extern s16 func_80250F0C(struct MarioState *);
extern void func_80250F50(struct MarioState *, u32, u32);
extern void func_80250FBC(struct MarioState *);
extern void func_80251048(struct MarioState *);
extern void func_802510E4(struct MarioState *, u32, u32);
extern void func_80251218(struct MarioState *, u32, u32);
extern void func_80251280(struct MarioState *, u32);
extern void func_802512E4(struct MarioState *, u32);
extern void func_80251348(struct MarioState *, u32);
extern void func_802513AC(struct MarioState *, u32);
extern void func_80251410(struct MarioState *, s32, s32);
extern void mario_set_forward_vel(struct MarioState *m, f32 speed);
extern s32 mario_get_floor_class(struct MarioState *m);
extern u32 func_8025167C(struct MarioState *);
extern struct Surface *func_8025181C(Vec3f, f32, f32);
extern f32 func_802518D0(Vec3f, f32, struct Surface **);
extern s32 mario_facing_downhill(struct MarioState *, s32);
extern u32 func_802519A8(struct MarioState *);
extern s32 mario_floor_is_slope(struct MarioState *m);
extern s32 func_80251BF8(struct MarioState *);
extern f32 find_floor_height_relative_polar(struct MarioState *, s32, f32);
extern s16 func_80251DD4(struct MarioState *, s16);
extern void func_80251F74(struct MarioState *);
extern void func_80252070(struct MarioState *);
extern u32 set_mario_action(struct MarioState *, u32 action, u32 actionArg);
extern s32 func_80252C30(struct MarioState *);
extern s32 func_80252E74(struct MarioState *, u32, u32);
extern s32 drop_and_set_mario_action(struct MarioState *, u32, u32);
extern s32 func_80252F98(struct MarioState *, u32, u32, s16);
extern s32 func_80252FEC(struct MarioState *);
extern s32 func_802530D4(struct MarioState *);
extern s32 func_802531B8(struct MarioState *);
extern s32 func_8025325C(struct MarioState *);
// extern ? func_8025335C(?);
// extern ? func_802534F4(?);
// extern ? func_8025360C(?);
// extern ? func_80253730(?);
// extern ? func_80253834(?);
// extern ? func_80253B2C(?);
// extern ? func_80253C94(?);
// extern ? func_80253E34(?);
// extern ? func_80254088(?);
// extern ? func_8025410C(?);
// extern ? func_80254164(?);
// extern ? func_802541BC(?);
// extern ? func_8025435C(?);
// extern ? Unknown8025453C(?);
extern s32 func_80254604(struct Object *);
extern void func_802548BC(void);
extern void func_80254CE0(void);

#endif /* _MARIO_H */

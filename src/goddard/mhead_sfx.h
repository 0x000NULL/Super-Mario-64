#ifndef _MARIO_HEAD_SFX_
#define _MARIO_HEAD_SFX_

#include <ultra64.h>

// Sfx for Mario Head Screen
enum GdSfx {
    GD_SFX_NONE           = 0x00,
    GD_SFX_HAND_APPEAR    = 0x01,
    GD_SFX_HAND_DISAPPEAR = 0x02,
    GD_SFX_UNUSED_COIN    = 0x04,
    GD_SFX_PINCH_FACE     = 0x08,
    GD_SFX_PINCH_FACE_2   = 0x10,
    GD_SFX_LET_GO_FACE    = 0x20,
    GD_SFX_CAM_ZOOM_IN    = 0x40,
    GD_SFX_CAM_ZOOM_OUT   = 0x80
};

// functions
extern void gd_reset_sfx(void);
extern u32 gd_new_sfx_to_play(void);
extern void gd_sfx_played(void);
extern void gd_play_sfx(u32);

#endif /* _MARIO_HEAD_SFX_ */

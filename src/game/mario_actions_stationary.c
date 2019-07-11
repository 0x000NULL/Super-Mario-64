#include <ultra64.h>

#include "sm64.h"
#include "engine/math_util.h"
#include "mario.h"
#include "interaction.h"
#include "mario_step.h"
#include "mario_actions_stationary.h"
#include "area.h"
#include "audio/interface_2.h"
#include "memory.h"
#include "behavior_data.h"
#include "sound_init.h"
#include "level_update.h"
#include "save_file.h"
#include "camera.h"

s32 func_802604E0(struct MarioState *m) {
    mario_drop_held_object(m);
    if (m->floor->normal.y < 0.29237169f)
    {
        return mario_push_off_steep_floor(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10)
    {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }

    if (m->input & INPUT_A_PRESSED)
    {
        return func_80252E74(m, ACT_JUMP, 0);
    }

    if (m->input & INPUT_OFF_FLOOR)
    {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE)
    {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    if (m->input & INPUT_FIRST_PERSON)
    {
        return set_mario_action(m, ACT_FIRST_PERSON, 0);
    }
    
    if (m->input & INPUT_NONZERO_ANALOG)
    {
        m->faceAngle[1] = (s16) m->intendedYaw;
        return set_mario_action(m, ACT_WALKING, 0);
    }
    
    if (m->input & INPUT_B_PRESSED)
    {
        return set_mario_action(m, ACT_PUNCHING, 0);
    }
    
    if (m->input & INPUT_Z_DOWN)
    {
        return set_mario_action(m, ACT_START_CROUCHING, 0);
    }
    
    return 0;
}

s32 func_802606DC(struct MarioState *m) {
    if (m->floor->normal.y < 0.29237169f)
    {
        return mario_push_off_steep_floor(m, ACT_HOLD_FREEFALL, 0);
    }
    
    if (m->heldObj->oUnk190 & 0x40)
    {
        m->heldObj->oUnk190 = (s32) (m->heldObj->oUnk190 & -0x41);
        return set_mario_action(m, ACT_PLACING_DOWN, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10)
    {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_A_PRESSED)
    {
        return func_80252E74(m, ACT_HOLD_JUMP, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR)
    {
        return set_mario_action(m, ACT_HOLD_FREEFALL, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE)
    {
        return set_mario_action(m, ACT_HOLD_BEGIN_SLIDING, 0);
    }
    
    if (m->input & INPUT_NONZERO_ANALOG)
    {
        m->faceAngle[1] = (s16) m->intendedYaw;
        return set_mario_action(m, ACT_HOLD_WALKING, 0);
    }
    
    if (m->input & INPUT_B_PRESSED)
    {
        return set_mario_action(m, ACT_THROWING, 0);
    }
    
    if (m->input & INPUT_Z_DOWN)
    {
        return drop_and_set_mario_action(m, ACT_START_CROUCHING, 0);
    }
    
    return 0;
}

s32 act_idle(struct MarioState *m) {
    if (m->quicksandDepth > 30.0f) {
        return set_mario_action(m, ACT_IN_QUICKSAND, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_8) {
        return set_mario_action(m, ACT_COUGHING, 0);
    }
    
    if (!(m->actionArg & 1) && m->health < 0x300) {
        return set_mario_action(m, ACT_PANTING, 0);
    }
    
    if (func_802604E0(m)) {
        return 1;
    }
    
    if (m->actionState == 3) {
        if ((m->area->terrainType & 7) == TERRAIN_SNOW) {
            return set_mario_action(m, ACT_SHIVERING, 0);
        } else {
            return set_mario_action(m, ACT_START_SLEEPING, 0);
        }
    }
    
    if (m->actionArg & 1) {
        set_mario_animation(m, MARIO_ANIM_STAND_AGAINST_WALL);
    } else {
        switch (m->actionState) {
            case 0:
                set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_LEFT);
                break;
            
            case 1:
                set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_RIGHT);
                break;
            
            case 2:
                set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_CENTER);
                break;
        }
        
        if (func_80250770(m)) {
            // Fall asleep after 10 head turning cycles.
            // act_start_sleeping is triggered earlier in the function
            // when actionState == 3. This happens when Mario's done
            // turning his head back and forth. However, we do some checks
            // here to make sure that Mario would be able to sleep here,
            // and that he's gone through 10 cycles before sleeping.
            // actionTimer is used to track how many cycles have passed.
            if (++m->actionState == 3) {
                f32 sp24 = m->pos[1] - find_floor_height_relative_polar(m, -0x8000, 60.0f);
                if (sp24 < -24.0f || 24.0f < sp24 || m->floor->flags & 1) {
                    m->actionState = 0;
                } else {
                    // If Mario hasn't turned his head 10 times yet, stay idle instead of going to sleep.
                    m->actionTimer++;
                    if (m->actionTimer < 10) {
                        m->actionState = 0;
                    }
                }
            }
        }
    }
    
    stationary_ground_step(m);
    
    return 0;
}

void func_80260BC4(struct MarioState *m, u32 actionState, s32 animFrame, u32 sound)
{
    if (m->actionState == actionState && m->marioObj->header.gfx.unk38.animFrame == animFrame)
    {
        SetSound(sound, m->marioObj->header.gfx.cameraToObject);
    }
}

s32 act_start_sleeping(struct MarioState *m) {
#ifdef VERSION_US
    s32 sp24;
#endif
    
    if (func_802604E0(m))
        return 1;
    
    if (m->quicksandDepth > 30.0f)
        return set_mario_action(m, ACT_IN_QUICKSAND, 0);
    
    if (m->actionState == 4)
        return set_mario_action(m, ACT_SLEEPING, 0);
    
    switch (m->actionState) {
        case 0:
#ifdef VERSION_US
            sp24 = set_mario_animation(m, MARIO_ANIM_START_SLEEP_IDLE);
#else
            set_mario_animation(m, MARIO_ANIM_START_SLEEP_IDLE);
#endif
            break;
        
        case 1:
#ifdef VERSION_US
            sp24 = set_mario_animation(m, MARIO_ANIM_START_SLEEP_SCRATCH);
#else
            set_mario_animation(m, MARIO_ANIM_START_SLEEP_SCRATCH);
#endif
            break;
        
        case 2:
#ifdef VERSION_US
            sp24 = set_mario_animation(m, MARIO_ANIM_START_SLEEP_YAWN);
#else
            set_mario_animation(m, MARIO_ANIM_START_SLEEP_YAWN);
#endif
            m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;
            break;
        
        case 3:
#ifdef VERSION_US
            sp24 = set_mario_animation(m, MARIO_ANIM_START_SLEEP_SITTING);
#else
            set_mario_animation(m, MARIO_ANIM_START_SLEEP_SITTING);
#endif
            m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;
            break;
    }
    
    func_80260BC4(m, 1, 0x29, SOUND_ACTION_PATBACK);
    func_80260BC4(m, 1, 0x31, SOUND_ACTION_PATBACK);
    func_80260BC4(m, 3, 0x0F, (m->unk14 + SOUND_TERRAIN_4));
    
    if(func_80250770(m))
        m->actionState++;

#ifdef VERSION_US
    if (m->actionState == 2) {
        if (sp24 == -1) {
            SetSound(SOUND_MARIO_YAWNING, m->marioObj->header.gfx.cameraToObject);
        }
    }
    
    if (m->actionState == 1) {
        if (sp24 == -1) {
            SetSound(SOUND_MARIO_IMATIRED, m->marioObj->header.gfx.cameraToObject);
        }
    }
#else
    if (m->actionState == 2)
        func_80250F50(m, SOUND_MARIO_YAWNING, MARIO_UNKNOWN_17);
#endif

    stationary_ground_step(m);
    return 0;
}

s32 act_sleeping(struct MarioState *m) {
    s32 sp24;
    if (m->input & INPUT_UNKNOWN_A41F /* ? */) {
        return set_mario_action(m, ACT_WAKING_UP, m->actionState);
    }
    
    if (m->quicksandDepth > 30.0f) {
        return set_mario_action(m, ACT_WAKING_UP, m->actionState);
    }

    if (m->pos[1] - find_floor_height_relative_polar(m, -0x8000, 60.0f) > 24.0f) {
        return set_mario_action(m, ACT_WAKING_UP, m->actionState);
    }
    
    m->unk98->eyeState = MARIO_EYES_CLOSED;
    stationary_ground_step(m);
    switch (m->actionState) {
        case 0: {
            sp24 = set_mario_animation(m, MARIO_ANIM_SLEEP_IDLE);
            
            if (sp24 == -1 && !m->actionTimer) {
                func_80248C28(2);
            }
            
            if (sp24 == 2) {
                SetSound(SOUND_MARIO_SNORING1, m->marioObj->header.gfx.cameraToObject);   
            }

            if (sp24 == 20) {
                SetSound(SOUND_MARIO_SNORING2, m->marioObj->header.gfx.cameraToObject);
            }

            if (func_80250770(m)) {
                m->actionTimer++;
                if (m->actionTimer > 45) {
                    m->actionState++;
                }
            }
            break;
        }
        case 1: {
            if (set_mario_animation(m, MARIO_ANIM_SLEEP_START_LYING) == 18) {
                func_80251348(m, SOUND_TERRAIN_4);
            }

            if (func_80250770(m)) {
                m->actionState++;
            }
            break;
        }
        case 2: {
            sp24 = set_mario_animation(m, MARIO_ANIM_SLEEP_LYING);
#ifdef VERSION_US
            func_80250F50(m, SOUND_MARIO_SNORING3, MARIO_UNKNOWN_16);
#else
            if (sp24 == 2) {
                SetSound(SOUND_MARIO_SNORING2, m->marioObj->header.gfx.cameraToObject);
            }

            if (sp24 == 25) {
                SetSound(SOUND_MARIO_SNORING1, m->marioObj->header.gfx.cameraToObject);
            }
#endif
            break;
        }
    }
    return 0;
}

s32 act_waking_up(struct MarioState *m) {
    if (!m->actionTimer) {
        func_803205E8(SOUND_MARIO_SNORING1, m->marioObj->header.gfx.cameraToObject);
        func_803205E8(SOUND_MARIO_SNORING2, m->marioObj->header.gfx.cameraToObject);
#ifdef VERSION_US
        func_803205E8(SOUND_MARIO_SNORING3, m->marioObj->header.gfx.cameraToObject);
#endif
        func_80248CB8(2);
    }
    
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    m->actionTimer++;

    if (m->actionTimer > 20) {
        return set_mario_action(m, ACT_IDLE, 0);
    }

    stationary_ground_step(m);

    set_mario_animation(m, !m->actionArg ? MARIO_ANIM_WAKE_FROM_SLEEP : MARIO_ANIM_WAKE_FROM_LYING);

    return 0;
}

s32 act_shivering(struct MarioState *m) {
    s32 sp24;

    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_A41F) {
        m->actionState = 2;
    }

    stationary_ground_step(m);
    switch(m->actionState) {
        case 0: {
            sp24 = set_mario_animation(m, MARIO_ANIM_SHIVERING_WARMING_HAND);
            if(sp24 == 0x31) {
                m->particleFlags |= PARTICLE_17;
                SetSound(SOUND_MARIO_HOO2, m->marioObj->header.gfx.cameraToObject);
            }
            if(sp24 == 7 || sp24 == 0x51) {
                SetSound(SOUND_ACTION_UNKNOWN62C, m->marioObj->header.gfx.cameraToObject);
            }
            if(func_802507AC(m)) {
                m->actionState = 1;
            }
            break;
        }
        case 1: {
            sp24 = set_mario_animation(m, MARIO_ANIM_SHIVERING);
            if(sp24 == 9 || sp24 == 0x19 || sp24 == 0x2C) {
                SetSound(SOUND_ACTION_UNKNOWN62C, m->marioObj->header.gfx.cameraToObject);
            }
            break;
        }
        case 2: {
            set_mario_animation(m, MARIO_ANIM_SHIVERING_RETURN_TO_IDLE);
            if (func_802507AC(m)) {
                set_mario_action(m, ACT_IDLE, 0);
            }
            break;
        }
    }
    return 0;
}

s32 act_coughing(struct MarioState *m) {
    s32 sp1C;

    if (func_802604E0(m)) {
        return 1;
    }

    stationary_ground_step(m);
    sp1C = set_mario_animation(m, MARIO_ANIM_COUGHING);
    if (sp1C == 0x19 || sp1C == 0x23) {
        SetSound(SOUND_MARIO_COUGHING4, m->marioObj->header.gfx.cameraToObject);
    }

    if (sp1C == 0x32 || sp1C == 0x3A) {
        SetSound(SOUND_MARIO_COUGHING3, m->marioObj->header.gfx.cameraToObject);
    }

    if (sp1C == 0x47 || sp1C == 0x50) {
        SetSound(SOUND_MARIO_COUGHING2, m->marioObj->header.gfx.cameraToObject);
    }

    return 0;
}

s32 func_802615C4(struct MarioState *m) {
    if (segmented_to_virtual(&bhvJumpingBox) == m->heldObj->behavior) {
        return set_mario_action(m, ACT_CRAZY_BOX_BOUNCE, 0);
    }

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_IDLE, 0);           
    }

    if (m->quicksandDepth > 30.0f) {
        return drop_and_set_mario_action(m, ACT_IN_QUICKSAND, 0);
    }

    if (func_802606DC(m)) {
        return 1;
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
    return 0;
}

s32 func_802616C4(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE) {
        return drop_and_set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    if (m->input & INPUT_NONZERO_ANALOG) {
        return set_mario_action(m, ACT_HOLD_HEAVY_WALKING, 0);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_HEAVY_THROW, 0);
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_IDLE_HEAVY_OBJ);
    return 0;
}

s32 act_standing_against_wall(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED 
                    | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
    {
        return func_80252FEC(m);
    }
    
    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_FIRST_PERSON, 0);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_PUNCHING, 0);
    }
    
    set_mario_animation(m, MARIO_ANIM_STAND_AGAINST_WALL);
    stationary_ground_step(m);
    return 0;
}

s32 act_in_quicksand(struct MarioState *m) {
    if (m->quicksandDepth < 30.0f) {
        return set_mario_action(m, ACT_IDLE, 0);
    }
    
    if (func_802604E0(m) != 0) {
        return 1;
    }
    
    if (m->quicksandDepth > 70.0f) {
        set_mario_animation(m, MARIO_ANIM_DYING_IN_QUICKSAND);
    } else {
        set_mario_animation(m, MARIO_ANIM_IDLE_IN_QUICKSAND);
    }
    
    stationary_ground_step(m);
    return 0;
}

s32 act_crouching(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_A_PRESSED) {
        return func_80252E74(m, ACT_BACKFLIP, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_UNKNOWN_022, 0);
    }
    
    if (!(m->input & INPUT_Z_DOWN)) {
        return set_mario_action(m, ACT_UNKNOWN_022, 0);
    }
    
    if (m->input & INPUT_NONZERO_ANALOG) {
        return set_mario_action(m, ACT_START_CRAWLING, 0);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_PUNCHING, 9);
    }
    
    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_CROUCHING);
    return 0;
}

s32 act_panting(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->health >= 0x500) {
        return set_mario_action(m, ACT_IDLE, 0);
    }
    
    if (func_802604E0(m)) {
        return 1;
    }
    
    if (set_mario_animation(m, MARIO_ANIM_WALK_PANTING) == 1) {
        SetSound(SOUND_MARIO_HOO3 + ((D_80226EB8 % 3U) << 0x10),
            m->marioObj->header.gfx.cameraToObject);
    }
    
    stationary_ground_step(m);
    m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;
    return 0;
}

s32 func_80261C74(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_PANTING, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->health >= 0x500) {
        return set_mario_action(m, ACT_UNKNOWN_007, 0);
    }
    
    if (func_802606DC(m) != 0) {
        return 1;
    }
    
    set_mario_animation(m, MARIO_ANIM_WALK_PANTING);
    stationary_ground_step(m);
    m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;
    return 0;
}

void func_80261D70(struct MarioState *m, s32 animID, u32 action) {
    stationary_ground_step(m);
    set_mario_animation(m, animID);
    if (func_80250770(m) != 0) {
        set_mario_action(m, action, 0);
    }
}

s32 act_braking_stop(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_PUNCHING, 0);
    }
    
    if (!(m->input & INPUT_FIRST_PERSON) && m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED
                        | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE)) {
            return func_80252FEC(m);
    }
    
    func_80261D70(m, MARIO_ANIM_STOP_SKID, ACT_IDLE);
    return 0;
}

s32 act_butt_slide_stop(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED
                    | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
    {
        return func_80252FEC(m);
    }
    
    func_80261D70(m, MARIO_ANIM_STOP_SLIDE, ACT_IDLE);
    if (m->marioObj->header.gfx.unk38.animFrame == 6) {
        func_80251280(m, SOUND_TERRAIN_2);
    }
    
    return 0;
}

s32 func_80261F8C(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_IDLE, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED
                    | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
    {
        return func_802530D4(m);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_THROWING, 0);
    }
    
    func_80261D70(m, MARIO_ANIM_STAND_UP_FROM_SLIDING_WITH_LIGHT_OBJ, ACT_UNKNOWN_007);
    return 0;
}

s32 act_slide_kick_slide_stop(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    func_80261D70(m, MARIO_ANIM_CROUCH_FROM_SLIDE_KICK, ACT_CROUCHING);
    return 0;
}

s32 act_start_crouching(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_A_PRESSED) {
        return func_80252E74(m, ACT_BACKFLIP, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_START_CROUCHING);
    if (func_802507AC(m)) {
        set_mario_action(m, ACT_CROUCHING, 0);
    }
    return 0;
}

s32 act_stop_crouching(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_A_PRESSED) {
        return func_80252E74(m, ACT_BACKFLIP, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_STOP_CROUCHING);
    if (func_802507AC(m)) {
        set_mario_action(m, ACT_IDLE, 0);
    }
    return 0;
}

s32 act_start_crawling(struct MarioState *m) {
    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_UNKNOWN_022, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }
    
    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_START_CRAWLING);
    if (func_802507AC(m)) {
        set_mario_action(m, ACT_CRAWLING, 0);
    }
    
    return 0;
}

s32 act_stop_crawling(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_STOP_CRAWLING);
    if (func_802507AC(m)) {
        set_mario_action(m, ACT_CROUCHING, 0);
    }
    return 0;
}

s32 func_80262574(struct MarioState *m) {
    s16 sp1E;
    f32 sp18;

    if (m->marioObj->oInteractStatus & 0x10) {
        return func_80252F98(m, ACT_SHOCKED, 0, 4);
    }
    
    if (m->actionTimer == 0) {
        if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK1) {
            return func_80252F98(m, ACT_BACKWARD_GROUND_KB, 0, 0xc);
        }
    }
    
    if (++m->actionTimer == 0x30) {
        return set_mario_action(m, ACT_IDLE, 0);
    }

    sp1E = (m->actionTimer % 0x10) << 0xC;
    sp18 = (f32) (((f32) (6 - m->actionTimer / 8) * 8.0f) + 4.0f);
    mario_set_forward_vel(m, 0);
    vec3f_set(m->vel, 0, 0, 0);
    if (sins(sp1E) >= 0.0f) {
        m->pos[1] = sins(sp1E) * sp18 + m->floorHeight;
    } else {
        m->pos[1] = m->floorHeight - sins(sp1E) * sp18;
    }
    
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    return 0;
}

s32 func_802627B8(struct MarioState *m, s32 arg1, u32 action) {
    stationary_ground_step(m);
    set_mario_animation(m, arg1);
    if(func_80250770(m)) {
        return set_mario_action(m, action, 0);
    }
    return 0;
}

s32 func_80262828(struct MarioState *m, u32 action) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }

    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_IDLE, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        if (!action) {
            return func_80252C30(m);
        } else {
            return func_80252E74(m, action, 0);
        }
    }

    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED
                    | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
    {
        return func_80252FEC(m);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_PUNCHING, 0);
    }

    return 0;
}

s32 act_jump_land_stop(struct MarioState *m) {
    if (func_80262828(m, 0)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_LAND_FROM_SINGLE_JUMP, ACT_IDLE);
    return 0;
}

s32 act_double_jump_land_stop(struct MarioState *m) {
    if (func_80262828(m, 0)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_LAND_FROM_DOUBLE_JUMP, ACT_IDLE);
    return 0;
}

s32 act_side_flip_land_stop(struct MarioState *m) {
    if (func_80262828(m, 0)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_SLIDEFLIP_LAND, ACT_IDLE);
    m->marioObj->header.gfx.angle[1] += 0x8000;
    return 0;
}

s32 act_freefall_land_stop(struct MarioState *m) {
    if (func_80262828(m, 0)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_GENERAL_LAND, ACT_IDLE);
    return 0;
}

s32 act_triple_jump_land_stop(struct MarioState *m) {
    if (func_80262828(m, ACT_JUMP)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_IDLE);
    return 0;
}

s32 act_backflip_land_stop(struct MarioState *m) {
    if (!(m->input & INPUT_Z_DOWN) || m->marioObj->header.gfx.unk38.animFrame >= 6) {
        m->input &= -3;
    }

    if (func_80262828(m, ACT_BACKFLIP)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_IDLE);
    return 0;
}

s32 act_lava_boost_land(struct MarioState *m) {
    m->input &= -0x2011;

    if (func_80262828(m, 0)) {
        return 1;
    }

    func_802627B8(m, MARIO_ANIM_STAND_UP_FROM_LAVA_BOOST, ACT_IDLE);
    return 0;
}

s32 act_long_jump_land_stop(struct MarioState *m) {
    m->input &= -0x2001;
    if (func_80262828(m, ACT_JUMP)) {
        return 1;
    }

    func_802627B8(m, !m->marioObj->oMarioLongJumpIsSlow ? MARIO_ANIM_CROUCH_FROM_FAST_LONGJUMP : MARIO_ANIM_CROUCH_FROM_SLOW_LONGJUMP, ACT_CROUCHING);
    return 0;
}

s32 func_80262CEC(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_IDLE, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED
                    | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
    {
        return func_802530D4(m);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_THROWING, 0);
    }
    
    func_802627B8(m, MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ, ACT_UNKNOWN_007);
    return 0;
}

s32 func_80262DE4(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_IDLE, 0);
    }
    
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED
                    | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))
    {
        return func_802530D4(m);
    }
    
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_THROWING, 0);
    }
    func_802627B8(m, MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ, ACT_UNKNOWN_007);
    return 0;
}

s32 act_air_throw_land(struct MarioState *m) {
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (++m->actionTimer == 4) {
        mario_throw_held_object(m);
    }
    
    func_802627B8(m, MARIO_ANIM_THROW_LIGHT_OBJECT, ACT_IDLE);
    return 0;
}

s32 act_twirl_land(struct MarioState *m) {
    m->actionState = 1;
    if (m->input & INPUT_UNKNOWN_10) {
        return set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_TWIRL_LAND);
    if (m->angleVel[1] > 0) {
        m->angleVel[1] -= 0x400;
        if (m->angleVel[1] < 0) {
            m->angleVel[1] = 0;
        }
        
        m->twirlYaw += m->angleVel[1];
    }
    
    m->marioObj->header.gfx.angle[1] += m->twirlYaw;
    if (func_80250770(m)) {
        if (m->angleVel[1] == 0) {
            m->faceAngle[1] += m->twirlYaw;
            set_mario_action(m, ACT_IDLE, 0);
        }
    }
    
    return 0;
}

s32 act_ground_pound_land(struct MarioState *m) {
    m->actionState = 1;
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);
    }
    
    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BUTT_SLIDE, 0);
    }
    
    func_802627B8(m, MARIO_ANIM_GROUND_POUND_LANDING, ACT_BUTT_SLIDE_STOP);
    return 0;
}

s32 act_first_person(struct MarioState *m) {
    s32 sp1C;
    s16 sp1A;
    s16 sp18;

    sp1C = 0U < (m->input & (INPUT_UNKNOWN_10 | 0xC));
    if (m->actionState == 0) {
        func_80248C28(2);
        func_80285BD8(m->area->camera, 6, 0x10);
        m->actionState = 1;
    } else {
        if (!(m->input & INPUT_FIRST_PERSON) || sp1C) {
            func_80248CB8(2);
            func_80285BD8(m->area->camera, -1, 1);
            return set_mario_action(m, ACT_IDLE, 0);
        }
    }
    
    if (m->floor->type == 0x2F) {
        if (save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 0x18) >= 10) {
            sp1A = m->unk94->unk16[0];
            sp18 = ((m->unk94->unk16[1] * 4) / 3) + m->faceAngle[1];
            if (sp1A == -0x1800) {
                if (sp18 < -0x6FFF || sp18 >= 0x7000) {
                    level_trigger_warp(m, 1);
                }
            }
        }
    }
    
    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
    return 0;
}

s32 func_80263378(struct MarioState *m) {
    if (m->pos[1] < m->waterLevel - 100) {
        if (m->action == ACT_SPAWN_SPIN_LANDING) {
            func_8024980C(0);
        }
        func_80251F74(m);
        return func_8025325C(m);
    }
    
    if (m->input & INPUT_SQUISHED) {
        func_80251F74(m);
        return drop_and_set_mario_action(m, ACT_SQUISHED, 0);
    }
    
    if (m->action != 0x0002020E) {
        if (m->health < 0x100) {
            func_80251F74(m);
            return drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
        }
    }
    return 0;
}

s32 mario_execute_stationary_action(struct MarioState *m) {
    s32 sp24;

    if (func_80263378(m)) {
        return 1;
    }
    
    if (mario_update_quicksand(m, 0.5f)) {
        return 1;
    }
    
    switch (m->action) {
    case ACT_IDLE:                  sp24 = act_idle(m);                             break;
    case ACT_START_SLEEPING:        sp24 = act_start_sleeping(m);                   break;
    case ACT_SLEEPING:              sp24 = act_sleeping(m);                         break;
    case ACT_WAKING_UP:             sp24 = act_waking_up(m);                        break;
    case ACT_PANTING:               sp24 = act_panting(m);                          break;
    case ACT_UNKNOWN_006:           sp24 = func_80261C74(m);                        break;
    case ACT_UNKNOWN_007:           sp24 = func_802615C4(m);                        break;
    case ACT_UNKNOWN_008:           sp24 = func_802616C4(m);                        break;
    case ACT_IN_QUICKSAND:          sp24 = act_in_quicksand(m);                     break;
    case ACT_STANDING_AGAINST_WALL: sp24 = act_standing_against_wall(m);            break;
    case ACT_COUGHING:              sp24 = act_coughing(m);                         break;
    case ACT_SHIVERING:             sp24 = act_shivering(m);                        break;
    case ACT_CROUCHING:             sp24 = act_crouching(m);                        break;
    case ACT_START_CROUCHING:       sp24 = act_start_crouching(m);                  break;
    case ACT_UNKNOWN_022:           sp24 = act_stop_crouching(m);                   break;
    case ACT_START_CRAWLING:        sp24 = act_start_crawling(m);                   break;
    case ACT_UNKNOWN_024:           sp24 = act_stop_crawling(m);                    break;
    case ACT_SLIDE_KICK_SLIDE_STOP: sp24 = act_slide_kick_slide_stop(m);            break;
    case ACT_UNKNOWN_026:           sp24 = func_80262574(m);                        break;
    case ACT_FIRST_PERSON:          sp24 = act_first_person(m);                     break;
    case ACT_JUMP_LAND_STOP:        sp24 = act_jump_land_stop(m);                   break;
    case ACT_DOUBLE_JUMP_LAND_STOP: sp24 = act_double_jump_land_stop(m);            break;
    case ACT_FREEFALL_LAND_STOP:    sp24 = act_freefall_land_stop(m);               break;
    case ACT_SIDE_FLIP_LAND_STOP:   sp24 = act_side_flip_land_stop(m);              break;
    case ACT_UNKNOWN_034:           sp24 = func_80262CEC(m);                        break;
    case ACT_UNKNOWN_035:           sp24 = func_80262DE4(m);                        break;
    case ACT_AIR_THROW_LAND:        sp24 = act_air_throw_land(m);                   break;
    case ACT_LAVA_BOOST_LAND:       sp24 = act_lava_boost_land(m);                  break;
    case ACT_TWIRL_LAND:            sp24 = act_twirl_land(m);                       break;
    case ACT_TRIPLE_JUMP_LAND_STOP: sp24 = act_triple_jump_land_stop(m);            break;
    case ACT_BACKFLIP_LAND_STOP:    sp24 = act_backflip_land_stop(m);               break;
    case ACT_LONG_JUMP_LAND_STOP:   sp24 = act_long_jump_land_stop(m);              break;
    case ACT_GROUND_POUND_LAND:     sp24 = act_ground_pound_land(m);                break;
    case ACT_BRAKING_STOP:          sp24 = act_braking_stop(m);                     break;
    case ACT_BUTT_SLIDE_STOP:       sp24 = act_butt_slide_stop(m);                  break;
    case ACT_UNKNOWN_03F:           sp24 = func_80261F8C(m);                        break;
    }
    
    if (!sp24) {
        if (m->input & INPUT_IN_WATER) {
            m->particleFlags |= PARTICLE_7;
        }
    }
    
    return sp24;
}



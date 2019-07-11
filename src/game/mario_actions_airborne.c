#include <ultra64.h>

#include "sm64.h"
#include "area.h"
#include "level_update.h"
#include "engine/math_util.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "display.h"
#include "camera.h"
#include "save_file.h"
#include "audio/interface_2.h"
#include "engine/graph_node.h"

static void play_flip_sounds(struct MarioState *m, s16 frame1, s16 frame2, s16 frame3)
{
    s32 animFrame = m->marioObj->header.gfx.unk38.animFrame;
    if (animFrame == frame1 || animFrame == frame2 || animFrame == frame3)
        SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);
}

static void play_far_fall_sound(struct MarioState *m)
{
    u32 action = m->action;
    if (!(action & ACT_FLAG_INVULNERABLE) &&
        action != ACT_TWIRLING &&
        action != ACT_FLYING &&
        !(m->flags & MARIO_UNKNOWN_18))
    {
        if (m->peakHeight - m->pos[1] > 1150.0f)
        {
            SetSound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
            m->flags |= MARIO_UNKNOWN_18;
        }
    }
}

#if VERSION_US
void func_u_8026A090(struct MarioState *m)
{
    if (m->actionArg == 0 && (m->forwardVel <= -28.0f || m->forwardVel >= 28.0f))
        func_80250F50(m, SOUND_MARIO_DOH, MARIO_UNKNOWN_17);
    else
        func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
}
#endif

static s32 lava_boost_on_wall(struct MarioState *m)
{
    m->faceAngle[1] = atan2s(m->wall->normal.z, m->wall->normal.x);

    if (m->forwardVel < 24.0f)
       m->forwardVel = 24.0f;

    if (!(m->flags & MARIO_METAL_CAP))
        m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;

    SetSound(SOUND_MARIO_ONFIRE, m->marioObj->header.gfx.cameraToObject);
    func_80251F74(m);
    return drop_and_set_mario_action(m, ACT_LAVA_BOOST, 1);
}

static s32 check_fall_damage(struct MarioState *m, u32 hardFallAction)
{
    f32 fallHeight;
    f32 damageHeight;

    fallHeight = m->peakHeight - m->pos[1];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"

    //! Never true
    if (m->actionState == ACT_GROUND_POUND)
        damageHeight = 600.0f;
    else
        damageHeight = 1150.0f;

#pragma GCC diagnostic pop

    if (m->action != ACT_TWIRLING && m->floor->type != SURFACE_LAVA)
    {
        if (m->vel[1] < -55.0f)
        {
            if (fallHeight > 3000.0f)
            {
                m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 16 : 24;
                set_camera_shake(SHAKE_FALL_DAMAGE);
                SetSound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
                return drop_and_set_mario_action(m, hardFallAction, 4);
            }
            else if (fallHeight > damageHeight && !func_802519A8(m))
            {
                m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 8 : 12;
                m->squishTimer = 30;
                set_camera_shake(SHAKE_FALL_DAMAGE);
                SetSound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
            }
        }
    }

    return FALSE;
}

static s32 check_kick_or_dive_in_air(struct MarioState *m)
{
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, m->forwardVel > 28.0f ? ACT_DIVE : ACT_JUMP_KICK, 0);
    return FALSE;
}

static s32 should_get_stuck_in_ground(struct MarioState *m)
{
    u32 terrainType = m->area->terrainType & 0x0007;
    struct Surface *floor = m->floor;
    s32 flags = floor->flags;
    s32 type = floor->type;

    if (floor != NULL &&
        (terrainType == TERRAIN_SNOW || terrainType == TERRAIN_SAND) &&
        type != SURFACE_LAVA &&
        SURFACE_IS_NOT_HARD(type))
    {
        if (!(flags & 0x01) &&
            m->peakHeight - m->pos[1] > 1000.0f &&
            floor->normal.y >= 0.8660254f)
        {
            return TRUE;
        }
    }

    return FALSE;
}

static s32 check_fall_damage_or_get_stuck(struct MarioState *m, u32 hardFallAction)
{
    if (should_get_stuck_in_ground(m))
    {
#ifdef VERSION_JP
        SetSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
#else
        SetSound(SOUND_MARIO_OOOF2, m->marioObj->header.gfx.cameraToObject);
#endif
        m->particleFlags |= 0x00010000;
        drop_and_set_mario_action(m, ACT_FEET_STUCK_IN_GROUND, 0);
        return TRUE;
    }

    return check_fall_damage(m, hardFallAction);
}

static s32 check_horizontal_wind(struct MarioState *m)
{
    struct Surface *floor;
    f32 speed;
    s16 pushAngle;

    floor = m->floor;

    if (floor->type == SURFACE_HORIZONTAL_WIND)
    {
        pushAngle = floor->force << 8;

        m->slideVelX += 1.2f * sins(pushAngle);
        m->slideVelZ += 1.2f * coss(pushAngle);

        speed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

        if (speed > 48.0f)
        {
            m->slideVelX = m->slideVelX * 48.0f / speed;
            m->slideVelZ = m->slideVelZ * 48.0f / speed;
            speed = 32.0f; //! This was meant to be 48?
        }
        else if (speed > 32.0f)
        {
            speed = 32.0f;
        }

        m->vel[0] = m->slideVelX;
        m->vel[2] = m->slideVelZ;
        m->slideYaw = atan2s(m->slideVelZ, m->slideVelX);
        m->forwardVel = speed * coss(m->faceAngle[1] - m->slideYaw);

#if !VERSION_US
        SetSound(SOUND_ENVIRONMENT_WIND2, m->marioObj->header.gfx.cameraToObject);
#endif
        return TRUE;
    }

    return FALSE;
}

static void update_air_with_turn(struct MarioState *m)
{
    f32 dragThreshold;
    s16 intendedDYaw;
    f32 intendedMag;

    if (!check_horizontal_wind(m))
    {
        dragThreshold = m->action == ACT_LONG_JUMP ? 48.0f : 32.0f;
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);

        if (m->input & INPUT_NONZERO_ANALOG)
        {
            intendedDYaw = m->intendedYaw - m->faceAngle[1];
            intendedMag = m->intendedMag / 32.0f;

            m->forwardVel += 1.5f * coss(intendedDYaw) * intendedMag;
            m->faceAngle[1] += 512.0f * sins(intendedDYaw) * intendedMag;
        }

        //! Uncapped air speed. Net positive when moving forward.
        if (m->forwardVel > dragThreshold)
            m->forwardVel -= 1.0f;
        if (m->forwardVel < -16.0f)
            m->forwardVel += 2.0f;

        m->vel[0] = m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
        m->vel[2] = m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);
    }
}

static void update_air_without_turn(struct MarioState *m)
{
    f32 sidewaysSpeed = 0.0f;
    f32 dragThreshold;
    s16 intendedDYaw;
    f32 intendedMag;

    if (!check_horizontal_wind(m))
    {
        dragThreshold = m->action == ACT_LONG_JUMP ? 48.0f : 32.0f;
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);

        if (m->input & INPUT_NONZERO_ANALOG)
        {
            intendedDYaw = m->intendedYaw - m->faceAngle[1];
            intendedMag = m->intendedMag / 32.0f;

            m->forwardVel += intendedMag * coss(intendedDYaw) * 1.5f;
            sidewaysSpeed = intendedMag * sins(intendedDYaw) * 10.0f;
        }

        //! Uncapped air speed. Net positive when moving forward.
        if (m->forwardVel > dragThreshold)
            m->forwardVel -= 1.0f;
        if (m->forwardVel < -16.0f)
            m->forwardVel += 2.0f;

        m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
        m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);

        m->slideVelX += sidewaysSpeed * sins(m->faceAngle[1] + 0x4000);
        m->slideVelZ += sidewaysSpeed * coss(m->faceAngle[1] + 0x4000);

        m->vel[0] = m->slideVelX;
        m->vel[2] = m->slideVelZ;
    }
}

static void update_lava_boost_or_twirling(struct MarioState *m)
{
    s16 intendedDYaw;
    f32 intendedMag;

    if (m->input & INPUT_NONZERO_ANALOG)
    {
        intendedDYaw = m->intendedYaw - m->faceAngle[1];
        intendedMag = m->intendedMag / 32.0f;

        m->forwardVel += coss(intendedDYaw) * intendedMag;
        m->faceAngle[1] += sins(intendedDYaw) * intendedMag * 1024.0f;

        if (m->forwardVel < 0.0f)
        {
            m->faceAngle[1] += 0x8000;
            m->forwardVel *= -1.0f;
        }

        if (m->forwardVel > 32.0f)
            m->forwardVel -= 2.0f;
    }

    m->vel[0] = m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
    m->vel[2] = m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);
}

static void update_flying_yaw(struct MarioState *m)
{
    s16 targetYawVel = -(s16) (m->controller->stickX * (m->forwardVel / 4.0f));

    if (targetYawVel > 0)
    {
        if (m->angleVel[1] < 0)
        {
            m->angleVel[1] += 0x40;
            if (m->angleVel[1] > 0x10)
                m->angleVel[1] = 0x10;
        }
        else
        {
            m->angleVel[1] = approach_s32(m->angleVel[1], targetYawVel, 0x10, 0x20);
        }
    }
    else if (targetYawVel < 0)
    {
        if (m->angleVel[1] > 0)
        {
            m->angleVel[1] -= 0x40;
            if (m->angleVel[1] < -0x10)
                m->angleVel[1] = -0x10;
        }
        else
        {
            m->angleVel[1] = approach_s32(m->angleVel[1], targetYawVel, 0x20, 0x10);
        }
    }
    else
    {
        m->angleVel[1] = approach_s32(m->angleVel[1], 0, 0x40, 0x40);
    }

    m->faceAngle[1] += m->angleVel[1];
    m->faceAngle[2] = 20 * -m->angleVel[1];
}

static void update_flying_pitch(struct MarioState *m)
{
    s16 targetPitchVel = -(s16) (m->controller->stickY * (m->forwardVel / 5.0f));

    if (targetPitchVel > 0)
    {
        if (m->angleVel[0] < 0)
        {
            m->angleVel[0] += 0x40;
            if (m->angleVel[0] > 0x20)
                m->angleVel[0] = 0x20;
        }
        else
        {
            m->angleVel[0] = approach_s32(m->angleVel[0], targetPitchVel, 0x20, 0x40);
        }
    }
    else if (targetPitchVel < 0)
    {
        if (m->angleVel[0] > 0)
        {
            m->angleVel[0] -= 0x40;
            if (m->angleVel[0] < -0x20)
                m->angleVel[0] = -0x20;
        }
        else
        {
            m->angleVel[0] = approach_s32(m->angleVel[0], targetPitchVel, 0x40, 0x20);
        }
    }
    else
    {
        m->angleVel[0] = approach_s32(m->angleVel[0], 0, 0x40, 0x40);
    }
}

static void update_flying(struct MarioState *m)
{
    UNUSED u32 unused;

    update_flying_pitch(m);
    update_flying_yaw(m);

    m->forwardVel -= 2.0f * ((f32) m->faceAngle[0] / 0x4000) + 0.1f;
    m->forwardVel -= 0.5f * (1.0f - coss(m->angleVel[1]));

    if (m->forwardVel < 0.0f)
        m->forwardVel = 0.0f;

    if (m->forwardVel > 16.0f)
        m->faceAngle[0] += (m->forwardVel - 32.0f) * 6.0f;
    else if (m->forwardVel > 4.0f)
        m->faceAngle[0] += (m->forwardVel - 32.0f) * 10.0f;
    else
        m->faceAngle[0] -= 0x400;

    m->faceAngle[0] += m->angleVel[0];

    if (m->faceAngle[0] > 0x2AAA)
        m->faceAngle[0] = 0x2AAA;
    if (m->faceAngle[0] < -0x2AAA)
        m->faceAngle[0] = -0x2AAA;

    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);

    m->slideVelX = m->vel[0];
    m->slideVelZ = m->vel[2];
}

static u32 common_air_action_step(
    struct MarioState *m, u32 landAction, s32 animation, u32 stepArg)
{
    u32 stepResult;

    update_air_without_turn(m);

    stepResult = perform_air_step(m, stepArg);
    switch (stepResult)
    {
    case AIR_STEP_NONE:
        set_mario_animation(m, animation);
        break;

    case AIR_STEP_LANDED:
        if (!check_fall_damage_or_get_stuck(m, ACT_HARD_BACKWARD_GROUND_KB))
            set_mario_action(m, landAction, 0);
        break;

    case AIR_STEP_HIT_WALL:
        set_mario_animation(m, animation);

        if (m->forwardVel > 16.0f)
        {
            mario_bonk_reflection(m, 0);
            m->faceAngle[1] += 0x8000;

            if (m->wall != NULL)
            {
                set_mario_action(m, ACT_AIR_HIT_WALL, 0);
            }
            else
            {
                if (m->vel[1] > 0.0f)
                    m->vel[1] = 0.0f;

                if (m->forwardVel >= 38.0f)
                {
                    m->particleFlags |= 0x00000002;
                    set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
                }
                else
                {
                    if (m->forwardVel > 8.0f)
                        mario_set_forward_vel(m, -8.0f);
                    return set_mario_action(m, ACT_SOFT_BONK, 0);
                }
            }
        }
        else
        {
            mario_set_forward_vel(m, 0.0f);
        }
        break;

    case AIR_STEP_GRABBED_LEDGE:
        set_mario_animation(m, MARIO_ANIM_IDLE_ON_LEDGE);
        drop_and_set_mario_action(m, ACT_LEDGE_GRAB, 0);
        break;

    case AIR_STEP_GRABBED_CEILING:
        set_mario_action(m, ACT_START_HANGING, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return stepResult;
}

static s32 act_jump(struct MarioState *m)
{
    if (check_kick_or_dive_in_air(m))
        return TRUE;

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80251410(m, SOUND_TERRAIN_1, 0);
    common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP,
        AIR_STEP_CHECK_LEDGE_GRAB | AIR_STEP_CHECK_HANG);
    return FALSE;
}

static s32 act_double_jump(struct MarioState *m)
{
    s32 animation;

    if (m->vel[1] >= 0.0f)
        animation = MARIO_ANIM_DOUBLE_JUMP_RISE;
    else
        animation = MARIO_ANIM_DOUBLE_JUMP_FALL;

    if (check_kick_or_dive_in_air(m))
        return TRUE;

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_HOOHOO);
    common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, animation,
        AIR_STEP_CHECK_LEDGE_GRAB | AIR_STEP_CHECK_HANG);
    return FALSE;
}

static s32 act_triple_jump(struct MarioState *m)
{
    if (gSpecialTripleJump)
        return set_mario_action(m, ACT_SPECIAL_TRIPLE_JUMP, 0);

    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

#if VERSION_US
    func_80251410(m, SOUND_TERRAIN_1, 0);
#else
    func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_YAHOO);
#endif

    common_air_action_step(m, ACT_TRIPLE_JUMP_LAND, MARIO_ANIM_TRIPLE_JUMP, 0);
    play_flip_sounds(m, 2, 8, 20);
    return FALSE;
}

static s32 act_backflip(struct MarioState *m)
{
    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_YAH);
    common_air_action_step(m, ACT_BACKFLIP_LAND, MARIO_ANIM_BACKFLIP, 0);
    play_flip_sounds(m, 2, 3, 17);
    return FALSE;
}

static s32 act_freefall(struct MarioState *m)
{
    s32 animation;

    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    switch (m->actionArg)
    {
    case 0: animation = MARIO_ANIM_GENERAL_FALL;         break;
    case 1: animation = MARIO_ANIM_FALL_FROM_SLIDE;      break;
    case 2: animation = MARIO_ANIM_FALL_FROM_SLIDE_KICK; break;
    }

    common_air_action_step(m, ACT_FREEFALL_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

static s32 act_hold_jump(struct MarioState *m)
{
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);

    if ((m->input & INPUT_B_PRESSED) && !(m->heldObj->oUnk190 & 0x00000010))
        return set_mario_action(m, ACT_AIR_THROW, 0);

    if (m->input & INPUT_Z_PRESSED)
        return drop_and_set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80251410(m, SOUND_TERRAIN_1, 0);
    common_air_action_step(m, ACT_HOLD_JUMP_LAND, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

static s32 act_hold_freefall(struct MarioState *m)
{
    s32 animation;
    if (m->actionArg == 0)
        animation = MARIO_ANIM_FALL_WITH_LIGHT_OBJ;
    else
        animation = MARIO_ANIM_FALL_FROM_SLIDING_WITH_LIGHT_OBJ;

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);

    if ((m->input & INPUT_B_PRESSED) && !(m->heldObj->oUnk190 & 0x00000010))
        return set_mario_action(m, ACT_AIR_THROW, 0);

    if (m->input & INPUT_Z_PRESSED)
        return drop_and_set_mario_action(m, ACT_GROUND_POUND, 0);

    common_air_action_step(m, ACT_HOLD_FREEFALL_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

static s32 act_side_flip(struct MarioState *m)
{
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80251410(m, SOUND_TERRAIN_1, 0);

    if (common_air_action_step(
            m, ACT_SIDE_FLIP_LAND, MARIO_ANIM_SLIDEFLIP, AIR_STEP_CHECK_LEDGE_GRAB) !=
        AIR_STEP_GRABBED_LEDGE)
    {
        m->marioObj->header.gfx.angle[1] += 0x8000;
    }

    // (this need to be on one line to match on PAL)
    if (m->marioObj->header.gfx.unk38.animFrame == 6) SetSound(SOUND_ACTION_UNKNOWN45A, m->marioObj->header.gfx.cameraToObject);
    return FALSE;
}

static s32 act_wall_kick_air(struct MarioState *m)
{
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80250FBC(m);
    common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SLIDEJUMP, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

static s32 act_long_jump(struct MarioState *m)
{
    s32 animation;
    if (!m->marioObj->oMarioLongJumpIsSlow)
        animation = MARIO_ANIM_FAST_LONGJUMP;
    else
        animation = MARIO_ANIM_SLOW_LONGJUMP;

    func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_YAHOO);

    if (m->floor->type == SURFACE_VERTICAL_WIND && m->actionState == 0)
    {
        SetSound(SOUND_MARIO_HEREWEGO, m->marioObj->header.gfx.cameraToObject);
        m->actionState = 1;
    }

    common_air_action_step(m, ACT_LONG_JUMP_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

static s32 act_riding_shell_air(struct MarioState *m)
{
    func_80251410(m, SOUND_TERRAIN_1, 0);
    set_mario_animation(m, MARIO_ANIM_JUMP_RIDING_SHELL);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_RIDING_SHELL_GROUND, 1);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 0.0f);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    m->marioObj->header.gfx.pos[1] += 42.0f;
    return FALSE;
}

static s32 act_twirling(struct MarioState *m)
{
    s16 startTwirlYaw = m->twirlYaw;
    s16 yawVelTarget;

    if (m->input & INPUT_A_DOWN)
        yawVelTarget = 0x2000;
    else
        yawVelTarget = 0x1800;

    m->angleVel[1] = approach_s32(m->angleVel[1], yawVelTarget, 0x200, 0x200);
    m->twirlYaw += m->angleVel[1];

    set_mario_animation(m, m->actionArg == 0 ? MARIO_ANIM_START_TWIRL : MARIO_ANIM_TWIRL);
    if (func_802507AC(m))
        m->actionArg = 1;

    if (startTwirlYaw > m->twirlYaw)
        SetSound(SOUND_ACTION_UNKNOWN438, m->marioObj->header.gfx.cameraToObject);

    update_lava_boost_or_twirling(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_TWIRL_LAND, 0);
        break;

    case AIR_STEP_HIT_WALL:
        mario_bonk_reflection(m, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    m->marioObj->header.gfx.angle[1] += m->twirlYaw;
    return FALSE;
}

static s32 act_dive(struct MarioState *m)
{
    if (m->actionArg == 0)
        func_80251410(m, SOUND_ACTION_UNKNOWN435, SOUND_MARIO_HOOHOO);
    else
        func_80251410(m, SOUND_TERRAIN_1, 0);

    set_mario_animation(m, MARIO_ANIM_DIVE);
    if (mario_check_object_grab(m))
    {
        mario_grab_used_object(m);
        m->unk98->unk0A = 1;
        if (m->action != ACT_DIVE)
            return TRUE;
    }

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_NONE:
        if (m->vel[1] < 0.0f && m->faceAngle[0] > -0x2AAA)
        {
            m->faceAngle[0] -= 0x200;
            if (m->faceAngle[0] < -0x2AAA)
                m->faceAngle[0] = -0x2AAA;
        }
        m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
        break;

    case AIR_STEP_LANDED:
        if (should_get_stuck_in_ground(m) && m->faceAngle[0] == -0x2AAA)
        {
#ifdef VERSION_JP
            SetSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
#else
            SetSound(SOUND_MARIO_OOOF2, m->marioObj->header.gfx.cameraToObject);
#endif
            m->particleFlags |= 0x00010000;
            drop_and_set_mario_action(m, ACT_HEAD_STUCK_IN_GROUND, 0);
        }
        else if (!check_fall_damage(m, ACT_HARD_FORWARD_GROUND_KB))
        {
            if (m->heldObj == NULL)
                set_mario_action(m, ACT_DIVE_SLIDE, 0);
            else
                set_mario_action(m, ACT_DIVE_PICKING_UP, 0);
        }
        m->faceAngle[0] = 0;
        break;

    case AIR_STEP_HIT_WALL:
        mario_bonk_reflection(m, 1);
        m->faceAngle[0] = 0;

        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        m->particleFlags |= 0x00000002;
        drop_and_set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return FALSE;
}

static s32 act_air_throw(struct MarioState *m)
{
    if (++(m->actionTimer) == 4)
        mario_throw_held_object(m);

    func_80250F50(m, SOUND_MARIO_WAH2, MARIO_UNKNOWN_17);
    set_mario_animation(m, MARIO_ANIM_THROW_LIGHT_OBJECT);
    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (!check_fall_damage_or_get_stuck(m, ACT_HARD_BACKWARD_GROUND_KB))
            m->action = ACT_AIR_THROW_LAND;
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 0.0f);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return FALSE;
}

static s32 act_water_jump(struct MarioState *m)
{
    if (m->forwardVel < 15.0f)
        mario_set_forward_vel(m, 15.0f);

    func_80251410(m, SOUND_ACTION_UNKNOWN432, 0);
    set_mario_animation(m, MARIO_ANIM_SINGLE_JUMP);

    switch (perform_air_step(m, AIR_STEP_CHECK_LEDGE_GRAB))
    {
    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_JUMP_LAND, 0);
        func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 15.0f);
        break;

    case AIR_STEP_GRABBED_LEDGE:
#if VERSION_US
        set_mario_animation(m, MARIO_ANIM_IDLE_ON_LEDGE);
#endif
        set_mario_action(m, ACT_LEDGE_GRAB, 0);
        func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return FALSE;
}

static s32 act_hold_water_jump(struct MarioState *m)
{
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);

    if (m->forwardVel < 15.0f)
        mario_set_forward_vel(m, 15.0f);

    func_80251410(m, SOUND_ACTION_UNKNOWN432, 0);
    set_mario_animation(m, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_HOLD_JUMP_LAND, 0);
        func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 15.0f);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return FALSE;
}

static s32 act_steep_jump(struct MarioState *m)
{
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    func_80251410(m, SOUND_TERRAIN_1, 0);
    mario_set_forward_vel(m, 0.98f * m->forwardVel);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (!check_fall_damage_or_get_stuck(m, ACT_HARD_BACKWARD_GROUND_KB))
        {
            m->faceAngle[0] = 0;
            set_mario_action(
                m, m->forwardVel < 0.0f ? ACT_BEGIN_SLIDING : ACT_JUMP_LAND, 0);
        }
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 0.0f);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    set_mario_animation(m, MARIO_ANIM_SINGLE_JUMP);
    m->marioObj->header.gfx.angle[1] = m->marioObj->oMarioSteepJumpYaw;
    return FALSE;
}

static s32 act_ground_pound(struct MarioState *m)
{
    u32 stepResult;
    f32 yOffset;

    func_80250F50(m, SOUND_ACTION_UNKNOWN435, MARIO_UNKNOWN_16);

    if (m->actionState == 0)
    {
        if (m->actionTimer < 10)
        {
            yOffset = 20 - 2 * m->actionTimer;
            if (m->pos[1] + yOffset + 160.0f < m->ceilHeight)
            {
                m->pos[1] += yOffset;
                m->peakHeight = m->pos[1];
                vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
            }
        }

        m->vel[1] = -50.0f;
        mario_set_forward_vel(m, 0.0f);

        set_mario_animation(m, m->actionArg == 0 ? MARIO_ANIM_START_GROUND_POUND : MARIO_ANIM_TRIPLE_JUMP_GROUND_POUND);
        if (m->actionTimer == 0)
            SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);

        m->actionTimer += 1;
        if (m->actionTimer >= m->marioObj->header.gfx.unk38.curAnim->unk08 + 4)
        {
            SetSound(SOUND_MARIO_WAH3, m->marioObj->header.gfx.cameraToObject);
            m->actionState = 1;
        }
    }
    else
    {
        set_mario_animation(m, MARIO_ANIM_GROUND_POUND);

        stepResult = perform_air_step(m, 0);
        if (stepResult == AIR_STEP_LANDED)
        {
            if (should_get_stuck_in_ground(m))
            {
#ifdef VERSION_JP
                SetSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
#else
                SetSound(SOUND_MARIO_OOOF2, m->marioObj->header.gfx.cameraToObject);
#endif
                m->particleFlags |= 0x00010000;
                set_mario_action(m, ACT_BUTT_STUCK_IN_GROUND, 0);
            }
            else
            {
                func_80251348(m, SOUND_ACTION_UNKNOWN460);
                if (!check_fall_damage(m, ACT_HARD_BACKWARD_GROUND_KB))
                {
                    m->particleFlags |= 0x00010010;
                    set_mario_action(m, ACT_GROUND_POUND_LAND, 0);
                }
            }
            set_camera_shake(SHAKE_GROUND_POUND);
        }
        else if (stepResult == AIR_STEP_HIT_WALL)
        {
            mario_set_forward_vel(m, -16.0f);
            if (m->vel[1] > 0.0f)
                m->vel[1] = 0.0f;

            m->particleFlags |= 0x00000002;
            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        }
    }

    return FALSE;
}

static s32 act_burning_jump(struct MarioState *m)
{
    func_80251410(m, SOUND_TERRAIN_1, m->actionArg == 0 ? 0 : -1);
    mario_set_forward_vel(m, m->forwardVel);

    if (perform_air_step(m, 0) == AIR_STEP_LANDED)
    {
        func_80251280(m, SOUND_TERRAIN_2);
        set_mario_action(m, ACT_BURNING_GROUND, 0);
    }

    set_mario_animation(m, m->actionArg == 0 ? MARIO_ANIM_SINGLE_JUMP : MARIO_ANIM_FIRE_LAVA_BURN);
    m->particleFlags |= 0x00000800;
    SetSound(SOUND_UNKNOWN_UNK1410, m->marioObj->header.gfx.cameraToObject);

    m->marioObj->oMarioBurnTimer += 3;

    m->health -= 10;
    if (m->health < 0x100)
        m->health = 0xFF;
    return FALSE;
}

static s32 act_burning_fall(struct MarioState *m)
{
    mario_set_forward_vel(m, m->forwardVel);

    if (perform_air_step(m, 0) == AIR_STEP_LANDED)
    {
        func_80251280(m, SOUND_TERRAIN_2);
        set_mario_action(m, ACT_BURNING_GROUND, 0);
    }

    set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    m->particleFlags |= 0x00000800;
    m->marioObj->oMarioBurnTimer += 3;

    m->health -= 10;
    if (m->health < 0x100)
        m->health = 0xFF;
    return FALSE;
}

static s32 act_crazy_box_bounce(struct MarioState *m)
{
    f32 minSpeed;

    if (m->actionTimer == 0)
    {
        switch (m->actionArg)
        {
        case 0:
            m->vel[1] = 45.0f;
            minSpeed = 32.0f;
            break;

        case 1:
            m->vel[1] = 60.0f;
            minSpeed = 36.0f;
            break;

        case 2:
            m->vel[1] = 100.0f;
            minSpeed = 48.0f;
            break;
        }

        SetSound(minSpeed < 40.0f ? SOUND_GENERAL_BOING1 : SOUND_GENERAL_BOING2, m->marioObj->header.gfx.cameraToObject);

        if (m->forwardVel < minSpeed)
            mario_set_forward_vel(m, minSpeed);

        m->actionTimer = 1;
    }

    func_80251410(m, SOUND_TERRAIN_1, 0);
    set_mario_animation(m, MARIO_ANIM_DIVE);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (m->actionArg < 2)
        {
            set_mario_action(m, ACT_CRAZY_BOX_BOUNCE, m->actionArg + 1);
        }
        else
        {
            m->heldObj->oInteractStatus = INT_STATUS_STOP_RIDING;
            m->heldObj = NULL;
            set_mario_action(m, ACT_STOMACH_SLIDE, 0);
        }
        m->particleFlags |= 0x00010000;
        break;

    case AIR_STEP_HIT_WALL:
        mario_bonk_reflection(m, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    m->marioObj->header.gfx.angle[0] = atan2s(m->forwardVel, -m->vel[1]);
    return FALSE;
}

static u32 common_air_knockback_step(
    struct MarioState *m, u32 landAction, u32 hardFallAction, s32 animation, f32 speed)
{
    u32 stepResult;

    mario_set_forward_vel(m, speed);

    stepResult = perform_air_step(m, 0);
    switch (stepResult)
    {
    case AIR_STEP_NONE:
        set_mario_animation(m, animation);
        break;

    case AIR_STEP_LANDED:
        if (!check_fall_damage_or_get_stuck(m, hardFallAction))
        {
#if VERSION_US
            if (m->action == ACT_THROWN_FORWARD || m->action == ACT_THROWN_BACKWARD)
                set_mario_action(m, landAction, m->hurtCounter);
            else
                set_mario_action(m, landAction, m->actionArg);
#else
            set_mario_action(m, landAction, m->actionArg);
#endif
        }
        break;

    case AIR_STEP_HIT_WALL:
        set_mario_animation(m, MARIO_ANIM_BACKWARD_AIR_KB);
        mario_bonk_reflection(m, 0);

        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        mario_set_forward_vel(m, -speed);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return stepResult;
}

static s32 func_8026CDFC(struct MarioState *m)
{
    if ((m->input & INPUT_A_PRESSED) && m->wallKickTimer != 0 &&
        m->prevAction == ACT_AIR_HIT_WALL)
    {
        m->faceAngle[1] += 0x8000;
        return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
    }

    return FALSE;
}

static s32 act_backward_air_kb(struct MarioState *m)
{
    if (func_8026CDFC(m))
        return 1;

#if VERSION_US
    func_u_8026A090(m);
#else
    func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
#endif
    common_air_knockback_step(
        m, ACT_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, -16.0f);
    return FALSE;
}

static s32 act_forward_air_kb(struct MarioState *m)
{
    if (func_8026CDFC(m))
        return 1;

#if VERSION_US
    func_u_8026A090(m);
#else
    func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
#endif
    common_air_knockback_step(
        m, ACT_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB, 0x002D, 16.0f);
    return FALSE;
}

static s32 act_hard_backward_air_kb(struct MarioState *m)
{
#if VERSION_US
    func_u_8026A090(m);
#else
    func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
#endif
    common_air_knockback_step(
        m, ACT_HARD_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, -16.0f);
    return FALSE;
}

static s32 act_hard_forward_air_kb(struct MarioState *m)
{
#if VERSION_US
    func_u_8026A090(m);
#else
    func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
#endif
    common_air_knockback_step(
        m, ACT_HARD_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB, 0x002D, 16.0f);
    return FALSE;
}

static s32 act_thrown_backward(struct MarioState *m)
{
    u32 landAction;
    if (m->actionArg != 0)
        landAction = ACT_HARD_BACKWARD_GROUND_KB;
    else
        landAction = ACT_BACKWARD_GROUND_KB;

    func_80250F50(m, SOUND_MARIO_WAAAOOOW, MARIO_UNKNOWN_17);

    common_air_knockback_step(
        m, landAction, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, m->forwardVel);

    m->forwardVel *= 0.98f;
    return FALSE;
}

static s32 act_thrown_forward(struct MarioState *m)
{
    s16 pitch;

    u32 landAction;
    if (m->actionArg != 0)
        landAction = ACT_HARD_FORWARD_GROUND_KB;
    else
        landAction = ACT_FORWARD_GROUND_KB;

    func_80250F50(m, SOUND_MARIO_WAAAOOOW, MARIO_UNKNOWN_17);

    if (common_air_knockback_step(
            m, landAction, ACT_HARD_FORWARD_GROUND_KB, 0x002D, m->forwardVel) == AIR_STEP_NONE)
    {
        pitch = atan2s(m->forwardVel, -m->vel[1]);
        if (pitch > 0x1800)
            pitch = 0x1800;

        m->marioObj->header.gfx.angle[0] = pitch + 0x1800;
    }

    m->forwardVel *= 0.98f;
    return FALSE;
}

static s32 act_soft_bonk(struct MarioState *m)
{
    if (func_8026CDFC(m))
        return 1;

#if VERSION_US
    func_u_8026A090(m);
#else
    func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
#endif

    common_air_knockback_step(
        m, ACT_FREEFALL_LAND, ACT_HARD_BACKWARD_GROUND_KB, 0x0056, m->forwardVel);
    return FALSE;
}

static s32 act_getting_blown(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        if (m->forwardVel > -60.0f)
            m->forwardVel -= 6.0f;
        else
            m->actionState = 1;
    }
    else
    {
        if (m->forwardVel < -16.0f)
            m->forwardVel += 0.8f;

        if (m->vel[1] < 0.0f && m->unkC4 < 4.0f)
            m->unkC4 += 0.05f;
    }

    if (++(m->actionTimer) == 20)
        mario_blow_off_cap(m, 50.0f);

    mario_set_forward_vel(m, m->forwardVel);
#if !VERSION_US
    func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
#endif
    set_mario_animation(m, MARIO_ANIM_BACKWARD_AIR_KB);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_HARD_BACKWARD_AIR_KB, 0);
        break;

    case AIR_STEP_HIT_WALL:
        set_mario_animation(m, MARIO_ANIM_AIR_FORWARD_KB);
        mario_bonk_reflection(m, 0);

        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        mario_set_forward_vel(m, -m->forwardVel);
        break;
    }

    return FALSE;
}

static s32 act_air_hit_wall(struct MarioState *m)
{
    if (m->heldObj != NULL)
        mario_drop_held_object(m);

    if (++(m->actionTimer) <= 2)
    {
        if (m->input & INPUT_A_PRESSED)
        {
            m->vel[1] = 52.0f;
            m->faceAngle[1] += 0x8000;
            return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
        }
    }
    else if (m->forwardVel >= 38.0f)
    {
        m->wallKickTimer = 5;
        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        m->particleFlags |= 0x00000002;
        return set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
    }
    else
    {
        m->wallKickTimer = 5;
        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        if (m->forwardVel > 8.0f)
            mario_set_forward_vel(m, -8.0f);
        return set_mario_action(m, ACT_SOFT_BONK, 0);
    }

    set_mario_animation(m, MARIO_ANIM_START_WALLKICK);

    //! Missing return statement. The returned value is the result of the call
    // to set_mario_animation. In practice, this value is nonzero.
    // This results in this action "cancelling" into itself. It is supposed to
    // execute three times, each on a separate frame, but instead it executes
    // three times on the same frame.
    // This results in firsties only being possible for a single frame, instead
    // of three.
}

static s32 act_forward_rollout(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        m->vel[1] = 30.0f;
        m->actionState = 1;
    }

    func_80251410(m, SOUND_TERRAIN_1, 0);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_NONE:
        if (m->actionState == 1)
        {
            if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 4)
                SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);
        }
        else
        {
            set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
        }
        break;

    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);
        func_80251280(m, SOUND_TERRAIN_2);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 0.0f);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    if (m->actionState == 1 && func_802507AC(m))
        m->actionState = 2;
    return FALSE;
}

static s32 act_backward_rollout(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        m->vel[1] = 30.0f;
        m->actionState = 1;
    }

    func_80251410(m, SOUND_TERRAIN_1, 0);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_NONE:
        if (m->actionState == 1)
        {
            if (set_mario_animation(m, MARIO_ANIM_BACKWARD_SPINNING) == 4)
                SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);
        }
        else
        {
            set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
        }
        break;

    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);
        func_80251280(m, SOUND_TERRAIN_2);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 0.0f);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    if (m->actionState == 1 && m->marioObj->header.gfx.unk38.animFrame == 2)
        m->actionState = 2;
    return FALSE;
}

static s32 act_butt_slide_air(struct MarioState *m)
{
    if (++(m->actionTimer) > 30 && m->pos[1] - m->floorHeight > 500.0f)
        return set_mario_action(m, ACT_FREEFALL, 1);

    update_air_with_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (m->actionState == 0 && m->vel[1] < 0.0f && m->floor->normal.y >= 0.9848077f)
        {
            m->vel[1] = -m->vel[1] / 2.0f;
            m->actionState = 1;
        }
        else
        {
            set_mario_action(m, ACT_BUTT_SLIDE, 0);
        }
        func_80251280(m, SOUND_TERRAIN_2);
        break;

    case AIR_STEP_HIT_WALL:
        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;
        m->particleFlags |= 0x00000002;
        set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    set_mario_animation(m, MARIO_ANIM_SLIDE);
    return FALSE;
}

static s32 act_hold_butt_slide_air(struct MarioState *m)
{
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_HOLD_FREEFALL, 1);

    if (++(m->actionTimer) > 30 && m->pos[1] - m->floorHeight > 500.0f)
        return set_mario_action(m, ACT_HOLD_FREEFALL, 1);

    update_air_with_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (m->actionState == 0 && m->vel[1] < 0.0f && m->floor->normal.y >= 0.9848077f)
        {
            m->vel[1] = -m->vel[1] / 2.0f;
            m->actionState = 1;
        }
        else
        {
            set_mario_action(m, ACT_HOLD_BUTT_SLIDE, 0);
        }
        func_80251280(m, SOUND_TERRAIN_2);
        break;

    case AIR_STEP_HIT_WALL:
        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        mario_drop_held_object(m);
        m->particleFlags |= 0x00000002;
        set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    set_mario_animation(m, MARIO_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ);
    return FALSE;
}

static s32 act_lava_boost(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_ONFIRE, MARIO_UNKNOWN_17);

    if (!(m->input & INPUT_NONZERO_ANALOG))
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);

    update_lava_boost_or_twirling(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (m->floor->type == SURFACE_LAVA)
        {
            m->actionState = 0;
            if (!(m->flags & MARIO_METAL_CAP))
                m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
            m->vel[1] = 84.0f;
            SetSound(SOUND_MARIO_ONFIRE, m->marioObj->header.gfx.cameraToObject);
        }
        else
        {
            func_80251348(m, SOUND_TERRAIN_4);
            if (m->actionState < 2 && m->vel[1] < 0.0f)
            {
                m->vel[1] = -m->vel[1] * 0.4f;
                mario_set_forward_vel(m, m->forwardVel * 0.5f);
                m->actionState += 1;
            }
            else
            {
                set_mario_action(m, ACT_LAVA_BOOST_LAND, 0);
            }
        }
        break;

    case AIR_STEP_HIT_WALL:
        mario_bonk_reflection(m, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    set_mario_animation(m, MARIO_ANIM_FIRE_LAVA_BURN);
    if ((m->area->terrainType & 0x0007) != TERRAIN_SNOW && !(m->flags & MARIO_METAL_CAP) &&
        m->vel[1] > 0.0f)
    {
        m->particleFlags |= 0x00000800;
        if (m->actionState == 0)
            SetSound(SOUND_UNKNOWN_UNK1410, m->marioObj->header.gfx.cameraToObject);
    }

    if (m->health < 0x100)
        level_trigger_warp(m, WARP_OP_DEATH);

    m->unk98->eyeState = MARIO_EYES_DEAD;
    return FALSE;
}

static s32 act_slide_kick(struct MarioState *m)
{
    if (m->actionState == 0 && m->actionTimer == 0)
    {
        func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_HOOHOO);
        set_mario_animation(m, MARIO_ANIM_SLIDE_KICK);
    }

    if (++(m->actionTimer) > 30 && m->pos[1] - m->floorHeight > 500.0f)
        return set_mario_action(m, ACT_FREEFALL, 2);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_NONE:
        if (m->actionState == 0)
        {
            m->marioObj->header.gfx.angle[0] = atan2s(m->forwardVel, -m->vel[1]);
            if (m->marioObj->header.gfx.angle[0] > 0x1800)
                m->marioObj->header.gfx.angle[0] = 0x1800;
        }
        break;

    case AIR_STEP_LANDED:
        if (m->actionState == 0 && m->vel[1] < 0.0f)
        {
            m->vel[1] = -m->vel[1] / 2.0f;
            m->actionState = 1;
            m->actionTimer = 0;
        }
        else
        {
            set_mario_action(m, ACT_SLIDE_KICK_SLIDE, 0);
        }
        func_80251280(m, SOUND_TERRAIN_2);
        break;

    case AIR_STEP_HIT_WALL:
        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        m->particleFlags |= 0x00000002;

        set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return FALSE;
}

static s32 act_jump_kick(struct MarioState *m)
{
    s32 animFrame;

    if (m->actionState == 0)
    {
        func_80250F50(m, SOUND_MARIO_HOO6, MARIO_UNKNOWN_16);
        m->marioObj->header.gfx.unk38.animID = -1;
        set_mario_animation(m, MARIO_ANIM_AIR_KICK);
        m->actionState = 1;
    }

    animFrame = m->marioObj->header.gfx.unk38.animFrame;
    if (animFrame == 0)
        m->unk98->unk0B = 0x86;
    if (animFrame >= 0 && animFrame < 8)
        m->flags |= MARIO_KICKING;

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (!check_fall_damage_or_get_stuck(m, ACT_HARD_BACKWARD_GROUND_KB))
            set_mario_action(m, ACT_FREEFALL_LAND, 0);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, 0.0f);
        break;
    }

    return FALSE;
}

static s32 act_shot_from_cannon(struct MarioState *m)
{
    if (m->area->camera->currPreset != CAMERA_PRESET_BEHIND_MARIO)
        m->unk94->unk1C[1] = 2;

    mario_set_forward_vel(m, m->forwardVel);

    func_80250F50(m, SOUND_MARIO_YAHOO, MARIO_UNKNOWN_17);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_NONE:
        set_mario_animation(m, MARIO_ANIM_AIRBORNE_ON_STOMACH);
        m->faceAngle[0] = atan2s(m->forwardVel, m->vel[1]);
        m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
        break;

    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_DIVE_SLIDE, 0);
        m->faceAngle[0] = 0;
        func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, -16.0f);

        m->faceAngle[0] = 0;
        if (m->vel[1] > 0.0f)
            m->vel[1] = 0.0f;

        m->particleFlags |= 0x00000002;
        set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    if ((m->flags & MARIO_WING_CAP) && m->vel[1] < 0.0f)
        set_mario_action(m, ACT_FLYING, 0);

    if ((m->forwardVel -= 0.05) < 10.0f)
        mario_set_forward_vel(m, 10.0f);

    if (m->vel[1] > 0.0f)
        m->particleFlags |= 0x00000001;
    return FALSE;
}

static s32 act_flying(struct MarioState *m)
{
    s16 startPitch = m->faceAngle[0];

    if (m->input & INPUT_Z_PRESSED)
    {
        if (m->area->camera->currPreset == CAMERA_PRESET_BEHIND_MARIO)
            func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        return set_mario_action(m, ACT_GROUND_POUND, 1);
    }

    if (!(m->flags & MARIO_WING_CAP))
    {
        if (m->area->camera->currPreset == CAMERA_PRESET_BEHIND_MARIO)
            func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->area->camera->currPreset != CAMERA_PRESET_BEHIND_MARIO)
        func_80285BD8(m->area->camera, 3, 1);

    if (m->actionState == 0)
    {
        if (m->actionArg == 0)
        {
            set_mario_animation(m, MARIO_ANIM_FLY_FROM_CANNON);
        }
        else
        {
            set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING_FLIP);
            if (m->marioObj->header.gfx.unk38.animFrame == 1)
                SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);
        }

        if (func_80250770(m))
        {
            if (m->actionArg == 2)
            {
                func_8024980C(0);
                m->actionArg = 1;
            }

            set_mario_animation(m, MARIO_ANIM_WING_CAP_FLY);
            m->actionState = 1;
        }
    }

    update_flying(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_NONE:
        m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
        m->marioObj->header.gfx.angle[2] = m->faceAngle[2];
        m->actionTimer = 0;
        break;

    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_DIVE_SLIDE, 0);

        set_mario_animation(m, MARIO_ANIM_DIVE);
        func_80250AAC(m, 7);

        m->faceAngle[0] = 0;
        func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        break;

    case AIR_STEP_HIT_WALL:
        if (m->wall != NULL)
        {
            mario_set_forward_vel(m, -16.0f);
            m->faceAngle[0] = 0;

            if (m->vel[1] > 0.0f)
                m->vel[1] = 0.0f;

            SetSound(
                (m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_UNKNOWN442 : SOUND_ACTION_UNKNOWN445,
                m->marioObj->header.gfx.cameraToObject);

            m->particleFlags |= 0x00000002;
            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
            func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        }
        else {
            if (m->actionTimer++ == 0)
                SetSound(SOUND_ACTION_UNKNOWN444, m->marioObj->header.gfx.cameraToObject);

            if (m->actionTimer == 30)
                m->actionTimer = 0;

            m->faceAngle[0] -= 0x200;
            if (m->faceAngle[0] < -0x2AAA)
                m->faceAngle[0] = -0x2AAA;

            m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
            m->marioObj->header.gfx.angle[2] = m->faceAngle[2];
        }
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    if (m->faceAngle[0] > 0x800 && m->forwardVel >= 48.0f)
        m->particleFlags |= 0x00000001;

    if (startPitch <= 0 && m->faceAngle[0] > 0 && m->forwardVel >= 48.0f)
    {
        SetSound(SOUND_ACTION_UNKNOWN456, m->marioObj->header.gfx.cameraToObject);
#if VERSION_US
        SetSound(SOUND_MARIO_YAHOO2 + ((D_80226EB8 % 5) << 16), m->marioObj->header.gfx.cameraToObject);
#endif
    }

    SetSound(SOUND_UNKNOWN_UNK1417, m->marioObj->header.gfx.cameraToObject);
    func_80251048(m);
    return FALSE;
}

static s32 act_riding_hoot(struct MarioState *m)
{
    if (!(m->input & INPUT_A_DOWN) || (m->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK7))
    {
        m->usedObj->oInteractStatus = 0;
        m->usedObj->oHootMarioReleaseTime = gGlobalTimer;

        func_80250F50(m, SOUND_MARIO_UH, MARIO_UNKNOWN_17);
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    m->pos[0] = m->usedObj->oPosX;
    m->pos[1] = m->usedObj->oPosY - 92.5f;
    m->pos[2] = m->usedObj->oPosZ;

    m->faceAngle[1] = 0x4000 - m->usedObj->oMoveAngleYaw;

    if (m->actionState == 0)
    {
        set_mario_animation(m, MARIO_ANIM_HANG_ON_CEILING);
        if (func_80250770(m))
        {
            set_mario_animation(m, MARIO_ANIM_HANG_ON_OWL);
            m->actionState = 1;
        }
    }

    vec3f_set(m->vel, 0.0f, 0.0f, 0.0f);
    vec3f_set(m->marioObj->header.gfx.pos, m->pos[0], m->pos[1], m->pos[2]);
    vec3s_set(m->marioObj->header.gfx.angle, 0, 0x4000 - m->faceAngle[1], 0);
    return FALSE;
}

static s32 act_flying_triple_jump(struct MarioState *m)
{
#if VERSION_US
    if (m->input & (INPUT_B_PRESSED | INPUT_Z_PRESSED))
    {
        if (m->area->camera->currPreset == CAMERA_PRESET_BEHIND_MARIO)
            func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
        if (m->input & INPUT_B_PRESSED)
            return set_mario_action(m, ACT_DIVE, 0);
        else
            return set_mario_action(m, ACT_GROUND_POUND, 0);
    }
#else
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);
#endif

    func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_YAHOO);
    if (m->actionState == 0)
    {
        set_mario_animation(m, MARIO_ANIM_TRIPLE_JUMP_FLY);

        if (m->marioObj->header.gfx.unk38.animFrame == 7)
            SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);

        if (func_802507AC(m))
        {
            set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING);
            m->actionState = 1;
        }
    }

    if (m->actionState == 1 && m->marioObj->header.gfx.unk38.animFrame == 1)
        SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);

    if (m->vel[1] < 4.0f)
    {
        if (m->area->camera->currPreset != CAMERA_PRESET_BEHIND_MARIO)
            func_80285BD8(m->area->camera, 3, 1);

        if (m->forwardVel < 32.0f)
            mario_set_forward_vel(m, 32.0f);

        set_mario_action(m, ACT_FLYING, 1);
    }

    if (m->actionTimer++ == 10 && m->area->camera->currPreset != CAMERA_PRESET_BEHIND_MARIO)
        func_80285BD8(m->area->camera, 3, 1);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (!check_fall_damage_or_get_stuck(m, ACT_HARD_BACKWARD_GROUND_KB))
            set_mario_action(m, ACT_DOUBLE_JUMP_LAND, 0);
        break;

    case AIR_STEP_HIT_WALL:
        mario_bonk_reflection(m, 0);
        break;

    case AIR_STEP_HIT_LAVA_WALL:
        lava_boost_on_wall(m);
        break;
    }

    return FALSE;
}

static s32 act_top_of_pole_jump(struct MarioState *m)
{
    func_80250FBC(m);
    common_air_action_step(m, ACT_FREEFALL_LAND, MARIO_ANIM_HANDSTAND_JUMP, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

static s32 act_vertical_wind(struct MarioState *m)
{
    s16 intendedDYaw = m->intendedYaw - m->faceAngle[1];
    f32 intendedMag = m->intendedMag / 32.0f;

    func_80250F50(m, SOUND_MARIO_HEREWEGO, MARIO_UNKNOWN_17);
    if (m->actionState == 0)
    {
        set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING_FLIP);
        if (m->marioObj->header.gfx.unk38.animFrame == 1)
            SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);

        if (func_802507AC(m))
            m->actionState = 1;
    }
    else
    {
        set_mario_animation(m, MARIO_ANIM_AIRBORNE_ON_STOMACH);
    }

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        set_mario_action(m, ACT_DIVE_SLIDE, 0);
        break;

    case AIR_STEP_HIT_WALL:
        mario_set_forward_vel(m, -16.0f);
        break;
    }

    m->marioObj->header.gfx.angle[0] = (s16) (6144.0f * intendedMag * coss(intendedDYaw));
    m->marioObj->header.gfx.angle[2] = (s16) (-4096.0f * intendedMag * sins(intendedDYaw));
    return FALSE;
}

static s32 act_special_triple_jump(struct MarioState *m)
{
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_DIVE, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_GROUND_POUND, 0);

    func_80251410(m, SOUND_TERRAIN_1, SOUND_MARIO_YAHOO);

    update_air_without_turn(m);

    switch (perform_air_step(m, 0))
    {
    case AIR_STEP_LANDED:
        if (m->actionState++ == 0)
            m->vel[1] = 42.0f;
        else
            set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);
        func_80251280(m, SOUND_TERRAIN_2);
        break;

    case AIR_STEP_HIT_WALL:
        mario_bonk_reflection(m, 1);
        break;
    }

    if (m->actionState == 0 || m->vel[1] > 0.0f)
    {
        if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 0)
            SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);
    }
    else
    {
        set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    }

    m->particleFlags |= 0x00000008;
    return FALSE;
}

static s32 check_common_airborne_cancels(struct MarioState *m)
{
    if (m->pos[1] < m->waterLevel - 100)
        return func_8025325C(m);

    if (m->input & INPUT_SQUISHED)
        return drop_and_set_mario_action(m, ACT_SQUISHED, 0);

    if (m->floor->type == SURFACE_VERTICAL_WIND && (m->action & ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION))
        return drop_and_set_mario_action(m, ACT_VERTICAL_WIND, 0);

    m->quicksandDepth = 0.0f;
    return FALSE;
}

s32 mario_execute_airborne_action(struct MarioState *m)
{
    u32 cancel;

    if (check_common_airborne_cancels(m))
        return TRUE;

    play_far_fall_sound(m);

    switch (m->action)
    {
    case ACT_JUMP:                 cancel = act_jump(m);                 break;
    case ACT_DOUBLE_JUMP:          cancel = act_double_jump(m);          break;
    case ACT_FREEFALL:             cancel = act_freefall(m);             break;
    case ACT_HOLD_JUMP:            cancel = act_hold_jump(m);            break;
    case ACT_HOLD_FREEFALL:        cancel = act_hold_freefall(m);        break;
    case ACT_SIDE_FLIP:            cancel = act_side_flip(m);            break;
    case ACT_WALL_KICK_AIR:        cancel = act_wall_kick_air(m);        break;
    case ACT_TWIRLING:             cancel = act_twirling(m);             break;
    case ACT_WATER_JUMP:           cancel = act_water_jump(m);           break;
    case ACT_HOLD_WATER_JUMP:      cancel = act_hold_water_jump(m);      break;
    case ACT_STEEP_JUMP:           cancel = act_steep_jump(m);           break;
    case ACT_BURNING_JUMP:         cancel = act_burning_jump(m);         break;
    case ACT_BURNING_FALL:         cancel = act_burning_fall(m);         break;
    case ACT_TRIPLE_JUMP:          cancel = act_triple_jump(m);          break;
    case ACT_BACKFLIP:             cancel = act_backflip(m);             break;
    case ACT_LONG_JUMP:            cancel = act_long_jump(m);            break;
    case ACT_RIDING_SHELL_JUMP:
    case ACT_RIDING_SHELL_FALL:    cancel = act_riding_shell_air(m);     break;
    case ACT_DIVE:                 cancel = act_dive(m);                 break;
    case ACT_AIR_THROW:            cancel = act_air_throw(m);            break;
    case ACT_BACKWARD_AIR_KB:      cancel = act_backward_air_kb(m);      break;
    case ACT_FORWARD_AIR_KB:       cancel = act_forward_air_kb(m);       break;
    case ACT_HARD_FORWARD_AIR_KB:  cancel = act_hard_forward_air_kb(m);  break;
    case ACT_HARD_BACKWARD_AIR_KB: cancel = act_hard_backward_air_kb(m); break;
    case ACT_SOFT_BONK:            cancel = act_soft_bonk(m);            break;
    case ACT_AIR_HIT_WALL:         cancel = act_air_hit_wall(m);         break;
    case ACT_FORWARD_ROLLOUT:      cancel = act_forward_rollout(m);      break;
    case ACT_SHOT_FROM_CANNON:     cancel = act_shot_from_cannon(m);     break;
    case ACT_BUTT_SLIDE_AIR:       cancel = act_butt_slide_air(m);       break;
    case ACT_HOLD_BUTT_SLIDE_AIR:  cancel = act_hold_butt_slide_air(m);  break;
    case ACT_LAVA_BOOST:           cancel = act_lava_boost(m);           break;
    case ACT_GETTING_BLOWN:        cancel = act_getting_blown(m);        break;
    case ACT_BACKWARD_ROLLOUT:     cancel = act_backward_rollout(m);     break;
    case ACT_CRAZY_BOX_BOUNCE:     cancel = act_crazy_box_bounce(m);     break;
    case ACT_SPECIAL_TRIPLE_JUMP:  cancel = act_special_triple_jump(m);  break;
    case ACT_GROUND_POUND:         cancel = act_ground_pound(m);         break;
    case ACT_THROWN_FORWARD:       cancel = act_thrown_forward(m);       break;
    case ACT_THROWN_BACKWARD:      cancel = act_thrown_backward(m);      break;
    case ACT_FLYING_TRIPLE_JUMP:   cancel = act_flying_triple_jump(m);   break;
    case ACT_SLIDE_KICK:           cancel = act_slide_kick(m);           break;
    case ACT_JUMP_KICK:            cancel = act_jump_kick(m);            break;
    case ACT_FLYING:               cancel = act_flying(m);               break;
    case ACT_RIDING_HOOT:          cancel = act_riding_hoot(m);          break;
    case ACT_TOP_OF_POLE_JUMP:     cancel = act_top_of_pole_jump(m);     break;
    case ACT_VERTICAL_WIND:        cancel = act_vertical_wind(m);        break;
    }

    return cancel;
}

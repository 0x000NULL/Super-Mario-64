#include <ultra64.h>

#include "sm64.h"
#include "mario.h"
#include "audio/interface_2.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "mario_step.h"
#include "area.h"
#include "interaction.h"
#include "mario_actions_object.h"
#include "memory.h"
#include "behavior_data.h"

struct LandingAction
{
    s16 numFrames;
    s16 unk02;
    u32 verySteepAction;
    u32 endAction;
    u32 aPressedAction;
    u32 offFloorAction;
    u32 slideAction;
};


static struct LandingAction sJumpLandAction =
{
    4,
    5,
    ACT_FREEFALL,
    ACT_JUMP_LAND_STOP,
    ACT_DOUBLE_JUMP,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static struct LandingAction sFreefallLandAction =
{
    4,
    5,
    ACT_FREEFALL,
    ACT_FREEFALL_LAND_STOP,
    ACT_DOUBLE_JUMP,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static struct LandingAction sSideFlipLandAction =
{
    4,
    5,
    ACT_FREEFALL,
    ACT_SIDE_FLIP_LAND_STOP,
    ACT_DOUBLE_JUMP,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static struct LandingAction sHoldJumpLandAction =
{
    4,
    5,
    ACT_HOLD_FREEFALL,
    ACT_UNKNOWN_034,
    ACT_HOLD_JUMP,
    ACT_HOLD_FREEFALL,
    ACT_HOLD_BEGIN_SLIDING
};

static struct LandingAction sHoldFreefallLandAction =
{
    4,
    5,
    ACT_HOLD_FREEFALL,
    ACT_UNKNOWN_035,
    ACT_HOLD_JUMP,
    ACT_HOLD_FREEFALL,
    ACT_HOLD_BEGIN_SLIDING
};

static struct LandingAction sLongJumpLandAction =
{
    6,
    5,
    ACT_FREEFALL,
    ACT_LONG_JUMP_LAND_STOP,
    ACT_LONG_JUMP,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static struct LandingAction sDoubleJumpLandAction =
{
    4,
    5,
    ACT_FREEFALL,
    ACT_DOUBLE_JUMP_LAND_STOP,
    ACT_JUMP,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static struct LandingAction sTripleJumpLandAction =
{
    4,
    0,
    ACT_FREEFALL,
    ACT_TRIPLE_JUMP_LAND_STOP,
    ACT_UNINITIALIZED,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static struct LandingAction sBackflipLandAction =
{
    4,
    0,
    ACT_FREEFALL,
    ACT_BACKFLIP_LAND_STOP,
    ACT_BACKFLIP,
    ACT_FREEFALL,
    ACT_BEGIN_SLIDING
};

static Mat4 D_80339F50[2];


static s16 func_80263A50(struct MarioState *m)
{
    s16 pitch = func_80251DD4(m, 0);
    pitch = pitch * m->forwardVel / 40.0f;
    return -pitch;
}

void func_80263AD4(struct MarioState *m, s16 arg1, s16 arg2)
{
    if (func_80250B68(m, arg1) || func_80250B68(m, arg2))
    {
        if (m->flags & MARIO_METAL_CAP)
        {
            if (m->marioObj->header.gfx.unk38.animID == 0x0092)
                func_802510E4(m, SOUND_ACTION_UNKNOWN42F, 0);
            else
                func_802510E4(m, SOUND_ACTION_UNKNOWN42A, 0);
        }
        else if (m->quicksandDepth > 50.0f)
        {
            SetSound(SOUND_ACTION_UNKNOWN42E, m->marioObj->header.gfx.cameraToObject);
        }
        else if (m->marioObj->header.gfx.unk38.animID == 0x0092)
        {
            func_802510E4(m, SOUND_TERRAIN_5, 0);
        }
        else
        {
            func_802510E4(m, SOUND_TERRAIN_3, 0);
        }
    }
}

static void func_80263C14(struct MarioState *m)
{
    m->pos[1] = m->floorHeight;
    func_80379AA4(D_80339F50[m->unk00], m->pos, m->faceAngle[1], 40.0f);
    m->marioObj->header.gfx.throwMatrix = &D_80339F50[m->unk00];
}

static s32 begin_walking_action(
    struct MarioState *m, f32 forwardVel, u32 action, u32 actionArg)
{
    m->faceAngle[1] = m->intendedYaw;
    mario_set_forward_vel(m, forwardVel);
    return set_mario_action(m, action, actionArg);
}

static void check_ledge_climb_down(struct MarioState *m)
{
    struct WallCollisionData wallCols;
    struct Surface *floor;
    f32 floorHeight;
    struct Surface *wall;
    s16 wallAngle;
    s16 wallDYaw;

    if (m->forwardVel < 10.0f)
    {
        wallCols.x = m->pos[0];
        wallCols.y = m->pos[1];
        wallCols.z = m->pos[2];
        wallCols.radius = 10.0f;
        wallCols.offsetY = -10.0f;

        if (find_wall_collisions(&wallCols) != 0)
        {
            floorHeight = find_floor(wallCols.x, wallCols.y, wallCols.z, &floor);
            if (floor != NULL)
            {
                if (wallCols.y - floorHeight > 160.0f)
                {
                    wall = wallCols.walls[wallCols.numWalls - 1];
                    wallAngle = atan2s(wall->normal.z, wall->normal.x);
                    wallDYaw = wallAngle - m->faceAngle[1];

                    if (wallDYaw > -0x4000 && wallDYaw < 0x4000)
                    {
                        m->pos[0] = wallCols.x - 20.0f * wall->normal.x;
                        m->pos[2] = wallCols.z - 20.0f * wall->normal.z;

                        m->faceAngle[0] = 0;
                        m->faceAngle[1] = wallAngle + 0x8000;

                        set_mario_action(m, ACT_LEDGE_CLIMB_DOWN, 0);
                        set_mario_animation(m, MARIO_ANIM_CLIMB_DOWN_LEDGE);
                    }
                }
            }
        }
    }
}

static void slide_bonk(struct MarioState *m, u32 fastAction, u32 slowAction)
{
    if (m->forwardVel > 16.0f)
    {
        mario_bonk_reflection(m, TRUE);
        drop_and_set_mario_action(m, fastAction, 0);
    }
    else
    {
        mario_set_forward_vel(m, 0.0f);
        set_mario_action(m, slowAction, 0);
    }
}

static s32 set_triple_jump_action(struct MarioState *m, UNUSED u32 action, UNUSED u32 actionArg)
{
    if (m->flags & MARIO_WING_CAP)
        return set_mario_action(m, ACT_FLYING_TRIPLE_JUMP, 0);
    else if (m->forwardVel > 20.0f)
        return set_mario_action(m, ACT_TRIPLE_JUMP, 0);
    else
        return set_mario_action(m, ACT_JUMP, 0);

    return 0;
}

static void update_sliding_angle(struct MarioState *m, f32 accel, f32 lossFactor)
{
    s32 newFacingDYaw;
    s16 facingDYaw;

    struct Surface *floor = m->floor;
    s16 slopeAngle = atan2s(floor->normal.z, floor->normal.x);
    f32 steepness = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);
    UNUSED f32 normalY = floor->normal.y;

    m->slideVelX += accel * steepness * sins(slopeAngle);
    m->slideVelZ += accel * steepness * coss(slopeAngle);

    m->slideVelX *= lossFactor;
    m->slideVelZ *= lossFactor;

    m->slideYaw = atan2s(m->slideVelZ, m->slideVelX);

    facingDYaw = m->faceAngle[1] - m->slideYaw;
    newFacingDYaw = facingDYaw;

    //! -0x4000 not handled - can slide down a slope while facing perpendicular to it
    if (newFacingDYaw > 0 && newFacingDYaw <= 0x4000)
    {
        if ((newFacingDYaw -= 0x200) < 0)
            newFacingDYaw = 0;
    }
    else if (newFacingDYaw > -0x4000 && newFacingDYaw < 0)
    {
        if ((newFacingDYaw += 0x200) > 0)
            newFacingDYaw = 0;
    }
    else if (newFacingDYaw > 0x4000 && newFacingDYaw < 0x8000)
    {
        if ((newFacingDYaw += 0x200) > 0x8000)
            newFacingDYaw = 0x8000;
    }
    else if (newFacingDYaw > -0x8000 && newFacingDYaw < -0x4000)
    {
        if ((newFacingDYaw -= 0x200) < -0x8000)
            newFacingDYaw = -0x8000;
    }

    m->faceAngle[1] = m->slideYaw + newFacingDYaw;

    m->vel[0] = m->slideVelX;
    m->vel[1] = 0.0f;
    m->vel[2] = m->slideVelZ;

    mario_update_moving_sand(m);
    mario_update_windy_ground(m);

    //! Speed is capped a frame late (butt slide HSG)
    m->forwardVel = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);
    if (m->forwardVel > 100.0f)
    {
        m->slideVelX = m->slideVelX * 100.0f / m->forwardVel;
        m->slideVelZ = m->slideVelZ * 100.0f / m->forwardVel;
    }

    if (newFacingDYaw < -0x4000 || newFacingDYaw > 0x4000)
        m->forwardVel *= -1.0f;
}

static s32 update_sliding(struct MarioState *m, f32 stopSpeed)
{
    f32 lossFactor;
    f32 accel;
    f32 oldSpeed;
    f32 newSpeed;

    s32 stopped = FALSE;

    s16 intendedDYaw = m->intendedYaw - m->slideYaw;
    f32 forward = coss(intendedDYaw);
    f32 sideward = sins(intendedDYaw);

    //! 10k glitch
    if (forward < 0.0f && m->forwardVel >= 0.0f)
        forward *= 0.5f + 0.5f * m->forwardVel / 100.0f;

    switch (mario_get_floor_class(m))
    {
    case SURFACE_CLASS_SLIDE:
        accel = 10.0f;
        lossFactor = m->intendedMag / 32.0f * forward * 0.02f + 0.98f;
        break;

    case SURFACE_CLASS_SLIPPERY:
        accel = 8.0f;
        lossFactor = m->intendedMag / 32.0f * forward * 0.02f + 0.96f;
        break;

    default:
        accel = 7.0f;
        lossFactor = m->intendedMag / 32.0f * forward * 0.02f + 0.92f;
        break;

    case SURFACE_CLASS_NOT_SLIPPERY:
        accel = 5.0f;
        lossFactor = m->intendedMag / 32.0f * forward * 0.02f + 0.92f;
        break;
    }

    oldSpeed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

    //! This is attempting to use trig derivatives to rotate mario's speed.
    // It is slightly off/asymmetric since it uses the new X speed, but the old
    // Z speed.
    m->slideVelX += m->slideVelZ * (m->intendedMag / 32.0f) * sideward * 0.05f;
    m->slideVelZ -= m->slideVelX * (m->intendedMag / 32.0f) * sideward * 0.05f;

    newSpeed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

    if (oldSpeed > 0.0f && newSpeed > 0.0f)
    {
        m->slideVelX = m->slideVelX * oldSpeed / newSpeed;
        m->slideVelZ = m->slideVelZ * oldSpeed / newSpeed;
    }

    update_sliding_angle(m, accel, lossFactor);

    if (!mario_floor_is_slope(m) && m->forwardVel * m->forwardVel < stopSpeed * stopSpeed)
    {
        mario_set_forward_vel(m, 0.0f);
        stopped = TRUE;
    }

    return stopped;
}

static void apply_slope_accel(struct MarioState *m)
{
    f32 slopeAccel;

    struct Surface *floor = m->floor;
    f32 steepness = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);

    UNUSED f32 normalY = floor->normal.y;
    s16 floorDYaw = m->floorAngle - m->faceAngle[1];

    if (mario_floor_is_slope(m))
    {
        s16 slopeClass = 0;

        if (m->action != ACT_SOFT_BACKWARD_GROUND_KB && m->action != ACT_SOFT_FORWARD_GROUND_KB)
            slopeClass = mario_get_floor_class(m);

        switch (slopeClass)
        {
        case SURFACE_CLASS_SLIDE:        slopeAccel = 5.3f; break;
        case SURFACE_CLASS_SLIPPERY:     slopeAccel = 2.7f; break;
        default:                         slopeAccel = 1.7f; break;
        case SURFACE_CLASS_NOT_SLIPPERY: slopeAccel = 0.0f; break;
        }

        if (floorDYaw > -0x4000 && floorDYaw < 0x4000)
            m->forwardVel += slopeAccel * steepness;
        else
            m->forwardVel -= slopeAccel * steepness;
    }

    m->slideYaw = m->faceAngle[1];

    m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
    m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);

    m->vel[0] = m->slideVelX;
    m->vel[1] = 0.0f;
    m->vel[2] = m->slideVelZ;

    mario_update_moving_sand(m);
    mario_update_windy_ground(m);
}

static s32 apply_landing_accel(struct MarioState *m, f32 frictionFactor)
{
    s32 stopped = FALSE;

    apply_slope_accel(m);

    if (!mario_floor_is_slope(m))
    {
        m->forwardVel *= frictionFactor;
        if (m->forwardVel * m->forwardVel < 1.0f)
        {
            mario_set_forward_vel(m, 0.0f);
            stopped = TRUE;
        }
    }

    return stopped;
}

static void update_shell_speed(struct MarioState *m)
{
    f32 maxTargetSpeed;
    f32 targetSpeed;

    if (m->floorHeight < m->waterLevel)
    {
        m->floorHeight = m->waterLevel;
        m->floor = &gWaterSurfacePseudoFloor;
        m->floor->originOffset = m->waterLevel; //! Negative origin offset
    }

    if (m->floor != NULL && m->floor->type == SURFACE_SLOW)
        maxTargetSpeed = 48.0f;
    else
        maxTargetSpeed = 64.0f;

    targetSpeed = m->intendedMag * 2.0f;
    if (targetSpeed > maxTargetSpeed)
        targetSpeed = maxTargetSpeed;
    if (targetSpeed < 24.0f)
        targetSpeed = 24.0f;

    if (m->forwardVel <= 0.0f)
        m->forwardVel += 1.1f;
    else if (m->forwardVel <= targetSpeed)
        m->forwardVel += 1.1f - m->forwardVel / 58.0f;
    else if (m->floor->normal.y >= 0.95f)
        m->forwardVel -= 1.0f;

    //! No backward speed cap (shell hyperspeed)
    if (m->forwardVel > 64.0f)
        m->forwardVel = 64.0f;

    m->faceAngle[1] = m->intendedYaw -
        approach_s32((s16)(m->intendedYaw - m->faceAngle[1]), 0, 0x800, 0x800);

    apply_slope_accel(m);
}

static s32 apply_slope_decel(struct MarioState *m, f32 decelCoef)
{
    f32 decel;
    s32 stopped = FALSE;

    switch (mario_get_floor_class(m))
    {
    case SURFACE_CLASS_SLIDE:        decel = decelCoef * 0.2f; break;
    case SURFACE_CLASS_SLIPPERY:     decel = decelCoef * 0.7f; break;
    default:                         decel = decelCoef * 2.0f; break;
    case SURFACE_CLASS_NOT_SLIPPERY: decel = decelCoef * 3.0f; break;
    }

    if ((m->forwardVel = approach_f32(m->forwardVel, 0.0f, decel, decel)) == 0.0f)
        stopped = TRUE;

    apply_slope_accel(m);
    return stopped;
}

static s32 update_decelerating_speed(struct MarioState *m)
{
    s32 stopped = FALSE;

    if ((m->forwardVel = approach_f32(m->forwardVel, 0.0f, 1.0f, 1.0f)) == 0.0f)
        stopped = TRUE;

    mario_set_forward_vel(m, m->forwardVel);
    mario_update_moving_sand(m);
    mario_update_windy_ground(m);

    return stopped;
}

static void update_walking_speed(struct MarioState *m)
{
    f32 maxTargetSpeed;
    f32 targetSpeed;

    if (m->floor != NULL && m->floor->type == SURFACE_SLOW)
        maxTargetSpeed = 24.0f;
    else
        maxTargetSpeed = 32.0f;

    targetSpeed = m->intendedMag < maxTargetSpeed ? m->intendedMag : maxTargetSpeed;

    if (m->quicksandDepth > 10.0f)
        targetSpeed *= 6.25 / m->quicksandDepth;

    if (m->forwardVel <= 0.0f)
        m->forwardVel += 1.1f;
    else if (m->forwardVel <= targetSpeed)
        m->forwardVel += 1.1f - m->forwardVel / 43.0f;
    else if (m->floor->normal.y >= 0.95f)
        m->forwardVel -= 1.0f;

    if (m->forwardVel > 48.0f)
        m->forwardVel = 48.0f;

    m->faceAngle[1] = m->intendedYaw -
        approach_s32((s16)(m->intendedYaw - m->faceAngle[1]), 0, 0x800, 0x800);
    apply_slope_accel(m);
}

static s32 should_begin_sliding(struct MarioState *m)
{
    if (m->input & INPUT_ABOVE_SLIDE)
    {
        s32 slideLevel = (m->area->terrainType & 0x0007) == TERRAIN_SLIDE;
        s32 movingBackward = m->forwardVel <= -1.0f;

        if (slideLevel || movingBackward || mario_facing_downhill(m, FALSE))
            return TRUE;
    }

    return FALSE;
}

static s32 analog_stick_held_back(struct MarioState *m)
{
    s16 intendedDYaw = m->intendedYaw - m->faceAngle[1];
    return intendedDYaw < -0x471C || intendedDYaw > 0x471C;
}

static s32 check_ground_dive_or_punch(struct MarioState *m)
{
    UNUSED s32 unused;

    if (m->input & INPUT_B_PRESSED)
    {
        //! Speed kick (shoutouts to SimpleFlips)
        if (m->forwardVel >= 29.0f && m->controller->stickMag > 48.0f)
        {
            m->vel[1] = 20.0f;
            return set_mario_action(m, ACT_DIVE, 1);
        }

        return set_mario_action(m, ACT_MOVE_PUNCHING, 0);
    }

    return FALSE;
}

static s32 begin_braking_action(struct MarioState *m)
{
    mario_drop_held_object(m);

    if (m->actionState == 1)
    {
        m->faceAngle[1] = m->actionArg;
        return set_mario_action(m, ACT_STANDING_AGAINST_WALL, 0);
    }

    if (m->forwardVel >= 16.0f && m->floor->normal.y >= 0.17364818f)
        return set_mario_action(m, ACT_BRAKING, 0);

    return set_mario_action(m, ACT_DECELERATING, 0);
}

static void func_802652F0(struct MarioState *m)
{
    s32 val14;
    struct Object *marioObj = m->marioObj;
    s32 val0C = TRUE;
    s16 targetPitch = 0;
    f32 val04;
    
    val04 = m->intendedMag > m->forwardVel ? m->intendedMag : m->forwardVel;

    if (val04 < 4.0f)
        val04 = 4.0f;

    if (m->quicksandDepth > 50.0f)
    {
        val14 = (s32)(val04 / 4.0f * 0x10000);
        func_80250934(m, MARIO_ANIM_MOVE_IN_QUICKSAND, val14);
        func_80263AD4(m, 0x0013, 0x005D);
        m->actionTimer = 0;
    }
    else
    {
        while (val0C)
        {
            switch (m->actionTimer)
            {
            case 0:
                if (val04 > 8.0f)
                {
                    m->actionTimer = 2;
                }
                else
                {
                    if ((val14 = (s32)(val04 / 4.0f * 0x10000)) < 0x1000)
                        val14 = 0x1000;
                    func_80250934(m, MARIO_ANIM_START_TIPTOE, val14);
                    func_80263AD4(m, 0x0007, 0x0016);
                    if (func_80250B68(m, 0x0017))
                        m->actionTimer = 2;

                    val0C = FALSE;
                }
                break;

            case 1:
                if (val04 > 8.0f)
                {
                    m->actionTimer = 2;
                }
                else
                {
                    if ((val14 = (s32)(val04 * 0x10000)) < 0x1000)
                        val14 = 0x1000;
                    func_80250934(m, MARIO_ANIM_TIPTOE, val14);
                    func_80263AD4(m, 0x000E, 0x0048);

                    val0C = FALSE;
                }
                break;

            case 2:
                if (val04 < 5.0f)
                {
                    m->actionTimer = 1;
                }
                else if (val04 > 22.0f)
                {
                    m->actionTimer = 3;
                }
                else
                {
                    val14 = (s32)(val04 / 4.0f * 0x10000);
                    func_80250934(m, MARIO_ANIM_WALKING, val14);
                    func_80263AD4(m, 0x000A, 0x0031);

                    val0C = FALSE;
                }
                break;

            case 3:
                if (val04 < 18.0f)
                {
                    m->actionTimer = 2;
                }
                else
                {
                    val14 = (s32)(val04 / 4.0f * 0x10000);
                    func_80250934(m, MARIO_ANIM_RUNNING, val14);
                    func_80263AD4(m, 0x0009, 0x002D);
                    targetPitch = func_80263A50(m);

                    val0C = FALSE;
                }
                break;
            }
        }
    }

    marioObj->oMarioWalkingPitch =
        (s16)approach_s32(marioObj->oMarioWalkingPitch, targetPitch, 0x800, 0x800);
    marioObj->header.gfx.angle[0] = marioObj->oMarioWalkingPitch;
}

static void func_8026570C(struct MarioState *m)
{
    s32 val0C;
    s32 val08 = TRUE;
    f32 val04;

    val04 = m->intendedMag > m->forwardVel ? m->intendedMag : m->forwardVel;

    if (val04 < 2.0f)
        val04 = 2.0f;

    while (val08)
    {
        switch (m->actionTimer)
        {
        case 0:
            if (val04 > 6.0f)
            {
                m->actionTimer = 1;
            }
            else
            {
                val0C = (s32)(val04 * 0x10000);
                func_80250934(m, MARIO_ANIM_SLOW_WALK_WITH_LIGHT_OBJ, val0C);
                func_80263AD4(m, 0x000C, 0x003E);

                val08 = FALSE;
            }
            break;

        case 1:
            if (val04 < 3.0f)
            {
                m->actionTimer = 0;
            }
            else if (val04 > 11.0f)
            {
                m->actionTimer = 2;
            }
            else
            {
                val0C = (s32)(val04 * 0x10000);
                func_80250934(m, MARIO_ANIM_WALK_WITH_LIGHT_OBJ, val0C);
                func_80263AD4(m, 0x000C, 0x003E);

                val08 = FALSE;
            }
            break;

        case 2:
            if (val04 < 8.0f)
            {
                m->actionTimer = 1;
            }
            else
            {
                val0C = (s32)(val04 / 2.0f * 0x10000);
                func_80250934(m, MARIO_ANIM_RUN_WITH_LIGHT_OBJ, val0C);
                func_80263AD4(m, 0x000A, 0x0031);

                val08 = FALSE;
            }
            break;
        }
    }
}

static void func_80265980(struct MarioState *m)
{
    s32 val04 = (s32)(m->intendedMag * 0x10000);
    func_80250934(m, MARIO_ANIM_WALK_WITH_HEAVY_OBJ, val04);
    func_80263AD4(m, 0x001A, 0x004F);
}

static void func_802659E8(struct MarioState *m, Vec3f startPos)
{
    s16 wallAngle;
    s16 dWallAngle;
    f32 dx = m->pos[0] - startPos[0];
    f32 dz = m->pos[2] - startPos[2];
    f32 movedDistance = sqrtf(dx*dx + dz*dz);
    s32 val04 = (s32)(movedDistance * 2.0f * 0x10000);

    if (m->forwardVel > 6.0f)
        mario_set_forward_vel(m, 6.0f);

    if (m->wall != NULL)
    {
        wallAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
        dWallAngle = wallAngle - m->faceAngle[1];
    }

    if (m->wall == NULL || dWallAngle <= -0x71C8 || dWallAngle >= 0x71C8)
    {
        m->flags |= MARIO_UNKNOWN_31;
        set_mario_animation(m, MARIO_ANIM_PUSHING);
        func_80263AD4(m, 0x0006, 0x0012);
    }
    else
    {
        if (dWallAngle < 0)
            func_80250934(m, MARIO_ANIM_SIDESTEP_RIGHT, val04);
        else
            func_80250934(m, MARIO_ANIM_SIDESTEP_LEFT, val04);

        if (m->marioObj->header.gfx.unk38.animFrame < 20)
        {
            SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);
            m->particleFlags |= PARTICLE_DUST;
        }

        m->actionState = 1;
        m->actionArg = wallAngle + 0x8000;
        m->marioObj->header.gfx.angle[1] = wallAngle + 0x8000;
        m->marioObj->header.gfx.angle[2] = func_80251DD4(m, 0x4000);
    }
}

static void func_80265C28(struct MarioState *m, s16 startYaw)
{
    struct MarioBodyState *val0C = m->unk98;
    UNUSED struct Object *marioObj = m->marioObj;
    s16 val06 = m->marioObj->header.gfx.unk38.animID;
    s16 dYaw;
    s16 val02;
    s16 val00;

    if (val06 == 0x0048 || val06 == 0x0072)
    {
        dYaw = m->faceAngle[1] - startYaw;
        val02 = -(s16)(dYaw * m->forwardVel / 12.0f);
        val00 = (s16)(m->forwardVel * 170.0f);

        if (val02 > 0x1555) val02 = 0x1555;
        if (val02 < -0x1555) val02 = -0x1555;

        if (val00 > 0x1555) val00 = 0x1555;
        if (val00 < 0) val00 = 0;

        val0C->unkC[2] = approach_s32(val0C->unkC[2], val02, 0x400, 0x400);
        val0C->unkC[0] = approach_s32(val0C->unkC[0], val00, 0x400, 0x400);;
    }
    else
    {
        val0C->unkC[2] = 0;
        val0C->unkC[0] = 0;
    }
}

static void func_80265DBC(struct MarioState *m, s16 startYaw)
{
    struct MarioBodyState *val0C = m->unk98;
    struct Object *marioObj = m->marioObj;
    s16 dYaw = m->faceAngle[1] - startYaw;
    s16 val04 = -(s16)(dYaw * m->forwardVel / 12.0f);
    s16 val02 = (s16)(m->forwardVel * 170.0f);

    if (val04 > 0x1800) val04 = 0x1800;
    if (val04 < -0x1800) val04 = -0x1800;

    if (val02 > 0x1000) val02 = 0x1000;
    if (val02 < 0) val02 = 0;

    val0C->unkC[2] = approach_s32(val0C->unkC[2], val04, 0x200, 0x200);
    val0C->unkC[0] = approach_s32(val0C->unkC[0], val02, 0x200, 0x200);
    val0C->unk12[2] = -val0C->unkC[2];

    marioObj->header.gfx.angle[2] = val0C->unkC[2];
    marioObj->header.gfx.pos[1] += 45.0f;
}

static s32 act_walking(struct MarioState *m)
{
    Vec3f startPos;
    s16 startYaw = m->faceAngle[1];

    mario_drop_held_object(m);

    if (should_begin_sliding(m))
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    if (m->input & INPUT_FIRST_PERSON)
        return begin_braking_action(m);

    if (m->input & INPUT_A_PRESSED)
        return func_80252C30(m);

    if (check_ground_dive_or_punch(m))
        return TRUE;

    if (m->input & INPUT_UNKNOWN_5)
        return begin_braking_action(m);

    if (analog_stick_held_back(m) && m->forwardVel >= 16.0f)
        return set_mario_action(m, ACT_TURNING_AROUND, 0);

    if (m->input & INPUT_Z_PRESSED)
        return set_mario_action(m, ACT_CROUCH_SLIDE, 0);

    m->actionState = 0;

    vec3f_copy(startPos, m->pos);
    update_walking_speed(m);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 0);
        set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
        break;

    case GROUND_STEP_NONE:
        func_802652F0(m);
        if (m->intendedMag - m->forwardVel > 16.0f)
            m->particleFlags |= PARTICLE_DUST;
        break;

    case GROUND_STEP_HIT_WALL:
        func_802659E8(m, startPos);
        m->actionTimer = 0;
        break;
    }

    check_ledge_climb_down(m);
    func_80265C28(m, startYaw);
    return FALSE;
}

static s32 act_move_punching(struct MarioState *m)
{
    if (should_begin_sliding(m))
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    if (m->actionState == 0 && (m->input & INPUT_A_DOWN))
        return set_mario_action(m, ACT_JUMP_KICK, 0);

    m->actionState = 1;

    mario_update_punch_sequence(m);

    if (m->forwardVel >= 0.0f)
    {
        apply_slope_decel(m, 0.5f);
    }
    else
    {
        if ((m->forwardVel += 8.0f) >= 0.0f)
            m->forwardVel = 0.0f;
        apply_slope_accel(m);
    }

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 0);
        break;

    case GROUND_STEP_NONE:
        m->particleFlags |= PARTICLE_DUST;
        break;
    }

    return FALSE;
}

static s32 act_hold_walking(struct MarioState *m)
{
    if (m->heldObj->behavior == segmented_to_virtual(bhvJumpingBox))
        return set_mario_action(m, ACT_CRAZY_BOX_BOUNCE, 0);

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_WALKING, 0);

    if (should_begin_sliding(m))
        return set_mario_action(m, ACT_HOLD_BEGIN_SLIDING, 0);

    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_THROWING, 0);

    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_HOLD_JUMP, 0);

    if (m->input & INPUT_UNKNOWN_5)
        return set_mario_action(m, ACT_HOLD_DECELERATING, 0);

    if (m->input & INPUT_Z_PRESSED)
        return drop_and_set_mario_action(m, ACT_CROUCH_SLIDE, 0);

    m->intendedMag *= 0.4f;

    update_walking_speed(m);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_HOLD_FREEFALL, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        if (m->forwardVel > 16.0f)
            mario_set_forward_vel(m, 16.0f);
        break;
    }

    func_8026570C(m);

    if (0.4f * m->intendedMag - m->forwardVel > 10.0f)
        m->particleFlags |= PARTICLE_DUST;

    return FALSE;
}

static s32 act_hold_heavy_walking(struct MarioState *m)
{
    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_HEAVY_THROW, 0);

    if (should_begin_sliding(m))
        return drop_and_set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    if (m->input & INPUT_UNKNOWN_5)
        return set_mario_action(m, ACT_UNKNOWN_008, 0);

    m->intendedMag *= 0.1f;

    update_walking_speed(m);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        drop_and_set_mario_action(m, ACT_FREEFALL, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        if (m->forwardVel > 10.0f)
            mario_set_forward_vel(m, 10.0f);
        break;
    }

    func_80265980(m);
    return FALSE;
}

static s32 act_turning_around(struct MarioState *m)
{
    if (m->input & INPUT_ABOVE_SLIDE)
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_SIDE_FLIP, 0);

    if (m->input & INPUT_UNKNOWN_5)
        return set_mario_action(m, ACT_BRAKING, 0);

    if (!analog_stick_held_back(m))
        return set_mario_action(m, ACT_WALKING, 0);

    if (apply_slope_decel(m, 2.0f))
        return begin_walking_action(m, 8.0f, ACT_FINISH_TURNING_AROUND, 0);

    SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);

    func_80251048(m);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 0);
        break;

    case GROUND_STEP_NONE:
        m->particleFlags |= PARTICLE_DUST;
        break;
    }

    if (m->forwardVel >= 18.0f)
    {
        set_mario_animation(m, MARIO_ANIM_TURNING_PART1);
    }
    else
    {
        set_mario_animation(m, MARIO_ANIM_TURNING_PART2);
        if (func_80250770(m))
        {
            if (m->forwardVel > 0.0f)
                begin_walking_action(m, -m->forwardVel, ACT_WALKING, 0);
            else
                begin_walking_action(m, 8.0f, ACT_WALKING, 0);
        }
    }

    return FALSE;
}

static s32 act_finish_turning_around(struct MarioState *m)
{
    if (m->input & INPUT_ABOVE_SLIDE)
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_SIDE_FLIP, 0);

    update_walking_speed(m);
    set_mario_animation(m, MARIO_ANIM_TURNING_PART2);

    if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND)
        set_mario_action(m, ACT_FREEFALL, 0);

    if (func_80250770(m))
        set_mario_action(m, ACT_WALKING, 0);

    m->marioObj->header.gfx.angle[1] += 0x8000;
    return FALSE;
}

static s32 act_braking(struct MarioState *m)
{
    if (!(m->input & INPUT_FIRST_PERSON) &&
        (m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE)))
    {
        return func_80252FEC(m);
    }

    if (apply_slope_decel(m, 2.0f))
        return set_mario_action(m, ACT_BRAKING_STOP, 0);

    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_MOVE_PUNCHING, 0);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 0);
        break;

    case GROUND_STEP_NONE:
        m->particleFlags |= PARTICLE_DUST;
        break;

    case GROUND_STEP_HIT_WALL:
        slide_bonk(m, ACT_BACKWARD_GROUND_KB, ACT_BRAKING_STOP);
        break;
    }

    SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);
    func_80251048(m);
    set_mario_animation(m, MARIO_ANIM_SKID_ON_GROUND);
    return FALSE;
}

static s32 act_decelerating(struct MarioState *m)
{
    s32 val0C;
    s16 slopeClass = mario_get_floor_class(m);

    if (!(m->input & INPUT_FIRST_PERSON))
    {
        if (should_begin_sliding(m))
            return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

        if (m->input & INPUT_A_PRESSED)
            return func_80252C30(m);

        if (check_ground_dive_or_punch(m))
            return TRUE;

        if (m->input & INPUT_NONZERO_ANALOG)
            return set_mario_action(m, ACT_WALKING, 0);

        if (m->input & INPUT_Z_PRESSED)
            return set_mario_action(m, ACT_CROUCH_SLIDE, 0);
    }

    if (update_decelerating_speed(m))
        return set_mario_action(m, ACT_IDLE, 0);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        if (slopeClass == SURFACE_CLASS_SLIDE)
            mario_bonk_reflection(m, TRUE);
        else
            mario_set_forward_vel(m, 0.0f);
        break;
    }

    if (slopeClass == SURFACE_CLASS_SLIDE)
    {
        set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_LEFT);
        SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);
        func_80251048(m);
        m->particleFlags |= PARTICLE_DUST;
    }
    else
    {
        if ((val0C = (s32)(m->forwardVel / 4.0f * 0x10000)) < 0x1000)
            val0C = 0x1000;
        
        func_80250934(m, MARIO_ANIM_WALKING, val0C);
        func_80263AD4(m, 0x000A, 0x0031);
    }

    return FALSE;
}

static s32 act_hold_decelerating(struct MarioState *m)
{
    s32 val0C;
    s16 slopeClass = mario_get_floor_class(m);

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_WALKING, 0);

    if (should_begin_sliding(m))
        return set_mario_action(m, ACT_HOLD_BEGIN_SLIDING, 0);

    if (m->input & INPUT_B_PRESSED)
        return set_mario_action(m, ACT_THROWING, 0);

    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_HOLD_JUMP, 0);

    if (m->input & INPUT_Z_PRESSED)
        return drop_and_set_mario_action(m, ACT_CROUCH_SLIDE, 0);

    if (m->input & INPUT_NONZERO_ANALOG)
        return set_mario_action(m, ACT_HOLD_WALKING, 0);

    if (update_decelerating_speed(m))
        return set_mario_action(m, ACT_UNKNOWN_007, 0);

    m->intendedMag *= 0.4f;

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_HOLD_FREEFALL, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        if (slopeClass == SURFACE_CLASS_SLIDE)
            mario_bonk_reflection(m, TRUE);
        else
            mario_set_forward_vel(m, 0.0f);
        break;
    }

    if (slopeClass == SURFACE_CLASS_SLIDE)
    {
        set_mario_animation(m, MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
        SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);
        func_80251048(m);
        m->particleFlags |= PARTICLE_DUST;
    }
    else
    {
        if ((val0C = (s32)(m->forwardVel * 0x10000)) < 0x1000)
            val0C = 0x1000;
        
        func_80250934(m, MARIO_ANIM_WALK_WITH_LIGHT_OBJ, val0C);
        func_80263AD4(m, 0x000C, 0x003E);
    }

    return FALSE;
}

static s32 act_riding_shell_ground(struct MarioState *m)
{
    /*06*/ s16 startYaw = m->faceAngle[1];

    if (m->input & INPUT_A_PRESSED)
        return set_mario_action(m, ACT_RIDING_SHELL_JUMP, 0);

    if (m->input & INPUT_Z_PRESSED)
    {
        mario_stop_riding_object(m);
        if (m->forwardVel < 24.0f)
            mario_set_forward_vel(m, 24.0f);
        return set_mario_action(m, ACT_CROUCH_SLIDE, 0);
    }

    update_shell_speed(m);
    set_mario_animation(m, m->actionArg == 0 ? MARIO_ANIM_START_RIDING_SHELL : MARIO_ANIM_RIDING_SHELL);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_RIDING_SHELL_FALL, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        mario_stop_riding_object(m);
        SetSound(m->flags & MARIO_METAL_CAP ? SOUND_ACTION_UNKNOWN442 : SOUND_ACTION_UNKNOWN445, m->marioObj->header.gfx.cameraToObject);
        m->particleFlags |= PARTICLE_1;
        set_mario_action(m, ACT_BACKWARD_GROUND_KB, 0);
        break;
    }

    func_80265DBC(m, startYaw);
    if (m->floor->type == SURFACE_LAVA)
        SetSound(SOUND_UNKNOWN_UNK1428, m->marioObj->header.gfx.cameraToObject);
    else
        SetSound(SOUND_UNKNOWN_UNK1420 + m->unk14, m->marioObj->header.gfx.cameraToObject);

    func_80251048(m);
    return FALSE;
}

static s32 act_crawling(struct MarioState *m)
{
    s32 val04;

    if (should_begin_sliding(m))
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    if (m->input & INPUT_FIRST_PERSON)
        return set_mario_action(m, ACT_UNKNOWN_024, 0);

    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_JUMP, 0);

    if (check_ground_dive_or_punch(m))
        return TRUE;

    if (m->input & INPUT_UNKNOWN_5)
        return set_mario_action(m, ACT_UNKNOWN_024, 0);

    if (!(m->input & INPUT_Z_DOWN))
        return set_mario_action(m, ACT_UNKNOWN_024, 0);

    m->intendedMag *= 0.1f;

    update_walking_speed(m);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        if (m->forwardVel > 10.0f)
            mario_set_forward_vel(m, 10.0f);
        //! Possibly unintended missing break

    case GROUND_STEP_NONE:
        func_80263C14(m);
        break;
    }

    val04 = (s32)(m->intendedMag * 2.0f * 0x10000);
    func_80250934(m, MARIO_ANIM_CRAWLING, val04);
    func_80263AD4(m, 0x001A, 0x004F);
    return FALSE;
}

static s32 act_burning_ground(struct MarioState *m)
{
    if (m->input & INPUT_A_PRESSED)
        return set_mario_action(m, ACT_BURNING_JUMP, 0);

    m->marioObj->oMarioBurnTimer += 2;
    if (m->marioObj->oMarioBurnTimer > 160)
        return set_mario_action(m, ACT_WALKING, 0);

    if (m->waterLevel - m->floorHeight > 50.0f)
    {
        SetSound(SOUND_GENERAL_FLAMEOUT, m->marioObj->header.gfx.cameraToObject);
        return set_mario_action(m, ACT_WALKING, 0);
    }

    if (m->forwardVel < 8.0f) m->forwardVel = 8.0f;
    if (m->forwardVel > 48.0f) m->forwardVel = 48.0f;

    m->forwardVel = approach_f32(m->forwardVel, 32.0f, 4.0f, 1.0f);

    if (m->input & INPUT_NONZERO_ANALOG)
    {
        m->faceAngle[1] = m->intendedYaw -
            approach_s32((s16)(m->intendedYaw - m->faceAngle[1]), 0, 0x600, 0x600);
    }

    apply_slope_accel(m);

    if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND)
        set_mario_action(m, ACT_BURNING_FALL, 0);

    func_80250934(m, MARIO_ANIM_RUNNING, (s32)(m->forwardVel / 2.0f * 0x10000));
    func_80263AD4(m, 0x0009, 0x002D);

    m->particleFlags |= PARTICLE_11;
    SetSound(SOUND_UNKNOWN_UNK1410, m->marioObj->header.gfx.cameraToObject);

    m->health -= 10;
    if (m->health < 0x100)
        set_mario_action(m, ACT_STANDING_DEATH, 0);

    m->unk98->eyeState = MARIO_EYES_DEAD;
    return FALSE;
}

static void func_80267814(struct MarioState *m)
{
    s16 intendedDYaw = m->intendedYaw - m->faceAngle[1];
    m->unk98->unkC[0] = (s32)(5461.3335f * m->intendedMag / 32.0f * coss(intendedDYaw));
    m->unk98->unkC[2] = (s32)(-(5461.3335f * m->intendedMag / 32.0f * sins(intendedDYaw)));
}

static void common_slide_action(
    struct MarioState *m, u32 endAction, u32 airAction, s32 animation)
{
    Vec3f val14;

    vec3f_copy(val14, m->pos);
    SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);

    func_80251048(m);

    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, airAction, 0);
        if (m->forwardVel < -50.0f || 50.0f < m->forwardVel)
            SetSound(SOUND_MARIO_HOOHOO, m->marioObj->header.gfx.cameraToObject);
        break;

    case GROUND_STEP_NONE:
        set_mario_animation(m, animation);
        func_80263C14(m);
        m->particleFlags |= PARTICLE_DUST;
        break;

    case GROUND_STEP_HIT_WALL:
        if (!func_802519A8(m))
        {
#ifdef VERSION_JP
            m->particleFlags |= PARTICLE_1;
#else
            if (m->forwardVel > 16.0f)
                m->particleFlags |= PARTICLE_1;
#endif
            slide_bonk(m, ACT_GROUND_BONK, endAction);
        }
        else if (m->wall != NULL)
        {
            s16 wallAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
            f32 slideSpeed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

            if ((slideSpeed *= 0.9) < 4.0f)
                slideSpeed = 4.0f;

            m->slideYaw = wallAngle - (s16)(m->slideYaw - wallAngle) + 0x8000;

            m->vel[0] = m->slideVelX = slideSpeed * sins(m->slideYaw);
            m->vel[2] = m->slideVelZ = slideSpeed * coss(m->slideYaw);
        }

        func_80263C14(m);
        break;
    }
}

static s32 common_slide_action_with_jump(
    struct MarioState *m, u32 stopAction, u32 jumpAction, u32 airAction, s32 animation)
{
    if (m->actionTimer == 5)
    {
        if (m->input & INPUT_A_PRESSED)
            return func_80252E74(m, jumpAction, 0);
    }
    else
    {
        m->actionTimer++;
    }

    if (update_sliding(m, 4.0f))
        return set_mario_action(m, stopAction, 0);

    common_slide_action(m, stopAction, airAction, animation);
    return FALSE;
}

static s32 act_butt_slide(struct MarioState *m)
{
    s32 cancel = common_slide_action_with_jump(
        m, ACT_BUTT_SLIDE_STOP, ACT_JUMP, ACT_BUTT_SLIDE_AIR, MARIO_ANIM_SLIDE);
    func_80267814(m);
    return cancel;
}

static s32 act_hold_butt_slide(struct MarioState *m)
{
    s32 cancel;

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_BUTT_SLIDE, 0);

    cancel = common_slide_action_with_jump(
        m, ACT_UNKNOWN_03F, ACT_HOLD_JUMP, ACT_HOLD_BUTT_SLIDE_AIR, MARIO_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ);
    func_80267814(m);
    return cancel;
}

static s32 act_crouch_slide(struct MarioState *m)
{
    s32 cancel;

    if (m->input & INPUT_ABOVE_SLIDE)
        return set_mario_action(m, ACT_BUTT_SLIDE, 0);

    if (m->actionTimer < 30)
    {
        m->actionTimer++;
        if (m->input & INPUT_A_PRESSED)
        {
            if (m->forwardVel > 10.0f)
                return func_80252E74(m, ACT_LONG_JUMP, 0);
        }
    }

    if (m->input & INPUT_B_PRESSED)
    {
        if (m->forwardVel >= 10.0f)
            return set_mario_action(m, ACT_SLIDE_KICK, 0);
        else
            return set_mario_action(m, ACT_MOVE_PUNCHING, 0x0009);
    }

    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_JUMP, 0);

    if (m->input & INPUT_FIRST_PERSON)
        return set_mario_action(m, ACT_BRAKING, 0);

    cancel = common_slide_action_with_jump(
        m, ACT_CROUCHING, ACT_JUMP, ACT_FREEFALL, MARIO_ANIM_START_CROUCHING);
    return cancel;
}

static s32 act_slide_kick_slide(struct MarioState *m)
{
    if (m->input & INPUT_A_PRESSED)
        return func_80252E74(m, ACT_FORWARD_ROLLOUT, 0);

    set_mario_animation(m, MARIO_ANIM_SLIDE_KICK);
    if (func_80250770(m) && m->forwardVel < 1.0f)
        return set_mario_action(m, ACT_SLIDE_KICK_SLIDE_STOP, 0);

    update_sliding(m, 1.0f);
    switch (perform_ground_step(m))
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, ACT_FREEFALL, 2);
        break;

    case GROUND_STEP_HIT_WALL:
        mario_bonk_reflection(m, TRUE);
        m->particleFlags |= PARTICLE_1;
        set_mario_action(m, ACT_BACKWARD_GROUND_KB, 0);
        break;
    }

    SetSound(SOUND_UNKNOWN_UNK1400 + m->unk14, m->marioObj->header.gfx.cameraToObject);
    m->particleFlags |= PARTICLE_DUST;
    return FALSE;
}

static s32 stomach_slide_action(
    struct MarioState *m, u32 stopAction, u32 airAction, s32 animation)
{
    if (m->actionTimer == 5)
    {
        if (!(m->input & INPUT_ABOVE_SLIDE) &&
            (m->input & (INPUT_A_PRESSED | INPUT_B_PRESSED)))
        {
            return drop_and_set_mario_action(
                m, m->forwardVel >= 0.0f ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0);
        }
    }
    else
    {
        m->actionTimer++;
    }

    if (update_sliding(m, 4.0f))
        return set_mario_action(m, stopAction, 0);

    common_slide_action(m, stopAction, airAction, animation);
    return FALSE;
}

static s32 act_stomach_slide(struct MarioState *m)
{
    s32 cancel = stomach_slide_action(m, ACT_STOMACH_SLIDE_STOP, ACT_FREEFALL, MARIO_ANIM_SLIDE_DIVE);
    return cancel;
}

static s32 act_hold_stomach_slide(struct MarioState *m)
{
    s32 cancel;

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_STOMACH_SLIDE, 0);

    cancel = stomach_slide_action(m, ACT_DIVE_PICKING_UP, ACT_HOLD_FREEFALL, MARIO_ANIM_SLIDE_DIVE);
    return cancel;
}

static s32 act_dive_slide(struct MarioState *m)
{
    if (!(m->input & INPUT_ABOVE_SLIDE) &&
        (m->input & (INPUT_A_PRESSED | INPUT_B_PRESSED)))
    {
        return set_mario_action(
            m, m->forwardVel > 0.0f ? ACT_FORWARD_ROLLOUT : ACT_BACKWARD_ROLLOUT, 0);
    }

    func_802512E4(m, SOUND_TERRAIN_4);

    //! If the dive slide ends on the same frame that we pick up on object,
    // mario will not be in the dive slide action for the call to
    // mario_check_object_grab, and so will end up in the regular picking action,
    // rather than the picking up after dive action.

    if (update_sliding(m, 8.0f) && func_80250770(m))
    {
        mario_set_forward_vel(m, 0.0f);
        set_mario_action(m, ACT_STOMACH_SLIDE_STOP, 0);
    }

    if (mario_check_object_grab(m))
    {
        mario_grab_used_object(m);
        m->unk98->unk0A = 1;
        return TRUE;
    }

    common_slide_action(m, ACT_STOMACH_SLIDE_STOP, ACT_FREEFALL, MARIO_ANIM_DIVE);
    return FALSE;
}

static s32 common_ground_knockback_action(
    struct MarioState *m, s32 animation, s32 arg2, s32 arg3, s32 arg4)
{
    s32 val04;

    if (arg3)
        func_802513AC(m, SOUND_TERRAIN_4);

    if (arg4 > 0)
        func_80250F50(m, SOUND_MARIO_ATTACKED, MARIO_UNKNOWN_17);
    else
#ifdef VERSION_JP
        func_80250F50(m, SOUND_MARIO_OOOF, MARIO_UNKNOWN_17);
#else
        func_80250F50(m, SOUND_MARIO_OOOF2, MARIO_UNKNOWN_17);
#endif

    if (m->forwardVel > 32.0f) m->forwardVel = 32.0f;
    if (m->forwardVel < -32.0f) m->forwardVel = -32.0f;

    val04 = set_mario_animation(m, animation);
    if (val04 < arg2)
        apply_landing_accel(m, 0.9f);
    else if (m->forwardVel >= 0.0f)
        mario_set_forward_vel(m, 0.1f);
    else
        mario_set_forward_vel(m, -0.1f);

    if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND)
    {
        if (m->forwardVel >= 0.0f)
            set_mario_action(m, ACT_FORWARD_AIR_KB, arg4);
        else
            set_mario_action(m, ACT_BACKWARD_AIR_KB, arg4);
    }
    else if (func_80250770(m))
    {
        if (m->health < 0x100)
        {
            set_mario_action(m, ACT_STANDING_DEATH, 0);
        }
        else
        {
            if (arg4 > 0)
                m->invincTimer = 30;
            set_mario_action(m, ACT_IDLE, 0);
        }
    }

    return val04;
}

static s32 act_hard_backward_ground_kb(struct MarioState *m)
{
    s32 val04 = common_ground_knockback_action(m, MARIO_ANIM_FALL_OVER_BACKWARDS, 0x2B, TRUE, m->actionArg);
    if (val04 == 0x2B && m->health < 0x100)
        set_mario_action(m, ACT_DEATH_ON_BACK, 0);

#if VERSION_US
    if (val04 == 0x36 && m->prevAction == ACT_SPECIAL_DEATH_EXIT)
        SetSound(SOUND_MARIO_MAMAMIA, m->marioObj->header.gfx.cameraToObject);
#endif

    if (val04 == 0x45)
        func_802512E4(m, SOUND_TERRAIN_2);

    return FALSE;
}

static s32 act_hard_forward_ground_kb(struct MarioState *m)
{
    s32 val04 = common_ground_knockback_action(m, MARIO_ANIM_LAND_ON_STOMACH, 0x15, TRUE, m->actionArg);
    if (val04 == 0x17 && m->health < 0x100)
        set_mario_action(m, ACT_DEATH_ON_STOMACH, 0);

    return FALSE;
}

static s32 act_backward_ground_kb(struct MarioState *m)
{
    common_ground_knockback_action(m, MARIO_ANIM_BACKWARD_KB, 0x16, TRUE, m->actionArg);
    return FALSE;
}

static s32 act_forward_ground_kb(struct MarioState *m)
{
    common_ground_knockback_action(m, MARIO_ANIM_FORWARD_KB, 0x14, TRUE, m->actionArg);
    return FALSE;
}

static s32 act_soft_backward_ground_kb(struct MarioState *m)
{
    common_ground_knockback_action(m, MARIO_ANIM_SOFT_BACK_KB, 0x64, FALSE, m->actionArg);
    return FALSE;
}

static s32 act_soft_forward_ground_kb(struct MarioState *m)
{
    common_ground_knockback_action(m, MARIO_ANIM_SOFT_FRONT_KB, 0x64, FALSE, m->actionArg);
    return FALSE;
}

static s32 act_ground_bonk(struct MarioState *m)
{
    s32 val04 = common_ground_knockback_action(m, MARIO_ANIM_GROUND_BONK, 0x20, TRUE, m->actionArg);
    if (val04 == 0x20)
        func_80251280(m, SOUND_TERRAIN_2);
    return FALSE;
}

static s32 act_death_exit_land(struct MarioState *m)
{
    s32 val04;

    apply_landing_accel(m, 0.9f);
    func_802513AC(m, SOUND_TERRAIN_4);

    val04 = set_mario_animation(m, MARIO_ANIM_FALL_OVER_BACKWARDS);

    if (val04 == 0x36)
        SetSound(SOUND_MARIO_MAMAMIA, m->marioObj->header.gfx.cameraToObject);
    if (val04 == 0x44)
        func_80251280(m, SOUND_TERRAIN_2);

    if (func_80250770(m))
        set_mario_action(m, ACT_IDLE, 0);

    return FALSE;
}

static u32 common_landing_action(struct MarioState *m, s16 animation, u32 airAction)
{
    u32 stepResult;

    if (m->input & INPUT_NONZERO_ANALOG)
        apply_landing_accel(m, 0.98f);
    else if (m->forwardVel >= 16.0f)
        apply_slope_decel(m, 2.0f);
    else
        m->vel[1] = 0.0f;

    stepResult = perform_ground_step(m);
    switch (stepResult)
    {
    case GROUND_STEP_LEFT_GROUND:
        set_mario_action(m, airAction, 0);
        break;

    case GROUND_STEP_HIT_WALL:
        set_mario_animation(m, MARIO_ANIM_PUSHING);
        break;
    }

    if (m->forwardVel > 16.0f)
        m->particleFlags |= PARTICLE_DUST;

    set_mario_animation(m, animation);
    func_802512E4(m, SOUND_TERRAIN_2);

    if (m->floor->type >= SURFACE_SHALLOW_QUICKSAND && m->floor->type <= SURFACE_MOVING_QUICKSAND)
        m->quicksandDepth += (4 - m->actionTimer) * 3.5f - 0.5f;

    return stepResult;
}

static s32 common_landing_cancels(
    struct MarioState *m,
    struct LandingAction *landingAction,
    s32 (*setAPressAction)(struct MarioState *, u32, u32))
{
    //! Everything here, incuding floor steepness, is checked before checking
    // if mario is actually on the floor. This leads to e.g. remote sliding.

    if (m->floor->normal.y < 0.2923717f)
        return mario_push_off_steep_floor(m, landingAction->verySteepAction, 0);

    m->doubleJumpTimer = landingAction->unk02;

    if (should_begin_sliding(m))
        return set_mario_action(m, landingAction->slideAction, 0);

    if (m->input & INPUT_FIRST_PERSON)
        return set_mario_action(m, landingAction->endAction, 0);

    if (++m->actionTimer >= landingAction->numFrames)
        return set_mario_action(m, landingAction->endAction, 0);

    if (m->input & INPUT_A_PRESSED)
        return setAPressAction(m, landingAction->aPressedAction, 0);

    if (m->input & INPUT_OFF_FLOOR)
        return set_mario_action(m, landingAction->offFloorAction, 0);

    return FALSE;
}

static s32 act_jump_land(struct MarioState *m)
{
    if (common_landing_cancels(m, &sJumpLandAction, func_80252E74))
        return TRUE;

    common_landing_action(m, MARIO_ANIM_LAND_FROM_SINGLE_JUMP, ACT_FREEFALL);
    return FALSE;
}

static s32 act_freefall_land(struct MarioState *m)
{
    if (common_landing_cancels(m, &sFreefallLandAction, func_80252E74))
        return TRUE;

    common_landing_action(m, MARIO_ANIM_GENERAL_LAND, ACT_FREEFALL);
    return FALSE;
}

static s32 act_side_flip_land(struct MarioState *m)
{
    if (common_landing_cancels(m, &sSideFlipLandAction, func_80252E74))
        return TRUE;

    if (common_landing_action(m, MARIO_ANIM_SLIDEFLIP_LAND, ACT_FREEFALL) != GROUND_STEP_HIT_WALL)
        m->marioObj->header.gfx.angle[1] += 0x8000;
    return FALSE;
}

static s32 act_hold_jump_land(struct MarioState *m)
{
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_JUMP_LAND_STOP, 0);

    if (common_landing_cancels(m, &sHoldJumpLandAction, func_80252E74))
        return TRUE;

    common_landing_action(m, MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ, ACT_HOLD_FREEFALL);
    return FALSE;
}

static s32 act_hold_freefall_land(struct MarioState *m)
{
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT)
        return drop_and_set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);

    if (common_landing_cancels(m, &sHoldFreefallLandAction, func_80252E74))
        return TRUE;

    common_landing_action(m, MARIO_ANIM_FALL_LAND_WITH_LIGHT_OBJ, ACT_HOLD_FREEFALL);
    return FALSE;
}

static s32 act_long_jump_land(struct MarioState *m)
{
    if (!(m->input & INPUT_Z_DOWN))
        m->input &= ~INPUT_A_PRESSED;

    if (common_landing_cancels(m, &sLongJumpLandAction, func_80252E74))
        return TRUE;

    if (!(m->input & INPUT_NONZERO_ANALOG))
        func_80250F50(m, SOUND_MARIO_UH2_2, MARIO_UNKNOWN_17);

    common_landing_action(m, !m->marioObj->oMarioLongJumpIsSlow ? MARIO_ANIM_CROUCH_FROM_FAST_LONGJUMP : MARIO_ANIM_CROUCH_FROM_SLOW_LONGJUMP, ACT_FREEFALL);
    return FALSE;
}

static s32 act_double_jump_land(struct MarioState *m)
{
    if (common_landing_cancels(m, &sDoubleJumpLandAction, set_triple_jump_action))
        return TRUE;
    common_landing_action(m, MARIO_ANIM_LAND_FROM_DOUBLE_JUMP, ACT_FREEFALL);
    return FALSE;
}

static s32 act_triple_jump_land(struct MarioState *m)
{
    m->input &= ~INPUT_A_PRESSED;

    if (common_landing_cancels(m, &sTripleJumpLandAction, func_80252E74))
        return TRUE;

    if (!(m->input & INPUT_NONZERO_ANALOG))
        func_80250F50(m, SOUND_MARIO_HAHA, MARIO_UNKNOWN_17);

    common_landing_action(m, MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_FREEFALL);
    return FALSE;
}

static s32 act_backflip_land(struct MarioState *m)
{
    if (!(m->input & INPUT_Z_DOWN))
        m->input &= ~INPUT_A_PRESSED;

    if (common_landing_cancels(m, &sBackflipLandAction, func_80252E74))
        return TRUE;

    if (!(m->input & INPUT_NONZERO_ANALOG))
        func_80250F50(m, SOUND_MARIO_HAHA, MARIO_UNKNOWN_17);

    common_landing_action(m, MARIO_ANIM_TRIPLE_JUMP_LAND, ACT_FREEFALL);
    return FALSE;
}

static s32 quicksand_jump_land_action(
    struct MarioState *m, s32 animation1, s32 animation2, u32 endAction, u32 airAction)
{
    if (m->actionTimer++ < 6)
    {
        m->quicksandDepth -= (7 - m->actionTimer) * 0.8f;
        if (m->quicksandDepth < 1.0f)
            m->quicksandDepth = 1.1f;

        func_80250FBC(m);
        set_mario_animation(m, animation1);
    }
    else
    {
        if (m->actionTimer >= 13)
            return set_mario_action(m, endAction, 0);

        set_mario_animation(m, animation2);
    }

    apply_landing_accel(m, 0.95f);
    if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND)
        set_mario_action(m, airAction, 0);

    return FALSE;
}

static s32 act_quicksand_jump_land(struct MarioState *m)
{
    s32 cancel = quicksand_jump_land_action(
        m, MARIO_ANIM_SINGLE_JUMP, MARIO_ANIM_LAND_FROM_SINGLE_JUMP, ACT_JUMP_LAND_STOP, ACT_FREEFALL);
    return cancel;
}

static s32 act_hold_quicksand_jump_land(struct MarioState *m)
{
    s32 cancel = quicksand_jump_land_action(
        m, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ, MARIO_ANIM_JUMP_LAND_WITH_LIGHT_OBJ, ACT_UNKNOWN_034, ACT_HOLD_FREEFALL);
    return cancel;
}

static s32 check_common_moving_cancels(struct MarioState *m)
{
    if (m->pos[1] < m->waterLevel - 100)
        return func_8025325C(m);

    if (!(m->action & ACT_FLAG_INVULNERABLE) && (m->input & INPUT_UNKNOWN_10))
        return drop_and_set_mario_action(m, ACT_UNKNOWN_026, 0);

    if (m->input & INPUT_SQUISHED)
        return drop_and_set_mario_action(m, ACT_SQUISHED, 0);

    if (!(m->action & ACT_FLAG_INVULNERABLE))
    {
        if (m->health < 0x100)
            return drop_and_set_mario_action(m, ACT_STANDING_DEATH, 0);
    }

    return FALSE;
}

s32 mario_execute_moving_action(struct MarioState *m)
{
    s32 cancel;

    if (check_common_moving_cancels(m))
        return TRUE;

    if (mario_update_quicksand(m, 0.25f))
        return TRUE;

    switch (m->action)
    {
    case ACT_WALKING:                  cancel = act_walking(m);                  break;
    case ACT_HOLD_WALKING:             cancel = act_hold_walking(m);             break;
    case ACT_HOLD_HEAVY_WALKING:       cancel = act_hold_heavy_walking(m);       break;
    case ACT_TURNING_AROUND:           cancel = act_turning_around(m);           break;
    case ACT_FINISH_TURNING_AROUND:    cancel = act_finish_turning_around(m);    break;
    case ACT_BRAKING:                  cancel = act_braking(m);                  break;
    case ACT_RIDING_SHELL_GROUND:      cancel = act_riding_shell_ground(m);      break;
    case ACT_CRAWLING:                 cancel = act_crawling(m);                 break;
    case ACT_BURNING_GROUND:           cancel = act_burning_ground(m);           break;
    case ACT_DECELERATING:             cancel = act_decelerating(m);             break;
    case ACT_HOLD_DECELERATING:        cancel = act_hold_decelerating(m);        break;
    case ACT_BUTT_SLIDE:               cancel = act_butt_slide(m);               break;
    case ACT_STOMACH_SLIDE:            cancel = act_stomach_slide(m);            break;
    case ACT_HOLD_BUTT_SLIDE:          cancel = act_hold_butt_slide(m);          break;
    case ACT_HOLD_STOMACH_SLIDE:       cancel = act_hold_stomach_slide(m);       break;
    case ACT_DIVE_SLIDE:               cancel = act_dive_slide(m);               break;
    case ACT_MOVE_PUNCHING:            cancel = act_move_punching(m);            break;
    case ACT_CROUCH_SLIDE:             cancel = act_crouch_slide(m);             break;
    case ACT_SLIDE_KICK_SLIDE:         cancel = act_slide_kick_slide(m);         break;
    case ACT_HARD_BACKWARD_GROUND_KB:  cancel = act_hard_backward_ground_kb(m);  break;
    case ACT_HARD_FORWARD_GROUND_KB:   cancel = act_hard_forward_ground_kb(m);   break;
    case ACT_BACKWARD_GROUND_KB:       cancel = act_backward_ground_kb(m);       break;
    case ACT_FORWARD_GROUND_KB:        cancel = act_forward_ground_kb(m);        break;
    case ACT_SOFT_BACKWARD_GROUND_KB:  cancel = act_soft_backward_ground_kb(m);  break;
    case ACT_SOFT_FORWARD_GROUND_KB:   cancel = act_soft_forward_ground_kb(m);   break;
    case ACT_GROUND_BONK:              cancel = act_ground_bonk(m);              break;
    case ACT_DEATH_EXIT_LAND:          cancel = act_death_exit_land(m);          break;
    case ACT_JUMP_LAND:                cancel = act_jump_land(m);                break;
    case ACT_FREEFALL_LAND:            cancel = act_freefall_land(m);            break;
    case ACT_DOUBLE_JUMP_LAND:         cancel = act_double_jump_land(m);         break;
    case ACT_SIDE_FLIP_LAND:           cancel = act_side_flip_land(m);           break;
    case ACT_HOLD_JUMP_LAND:           cancel = act_hold_jump_land(m);           break;
    case ACT_HOLD_FREEFALL_LAND:       cancel = act_hold_freefall_land(m);       break;
    case ACT_TRIPLE_JUMP_LAND:         cancel = act_triple_jump_land(m);         break;
    case ACT_BACKFLIP_LAND:            cancel = act_backflip_land(m);            break;
    case ACT_QUICKSAND_JUMP_LAND:      cancel = act_quicksand_jump_land(m);      break;
    case ACT_HOLD_QUICKSAND_JUMP_LAND: cancel = act_hold_quicksand_jump_land(m); break;
    case ACT_LONG_JUMP_LAND:           cancel = act_long_jump_land(m);           break;
    }

    if (!cancel && (m->input & INPUT_IN_WATER))
    {
        m->particleFlags |= PARTICLE_10;
        m->particleFlags &= ~PARTICLE_DUST;
    }

    return cancel;
}

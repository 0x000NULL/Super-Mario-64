#include <ultra64.h>

#include "sm64.h"
#include "mario.h"
#include "area.h"
#include "audio/interface_2.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "mario_misc.h"
#include "display.h"
#include "game.h"
#include "engine/graph_node.h"
#include "interaction.h"
#include "level_update.h"
#include "memory.h"
#include "main.h"
#include "mario_actions_object.h"
#include "mario_actions_automatic.h"
#include "mario_actions_cutscene.h"
#include "mario_actions_submerged.h"
#include "mario_actions_airborne.h"
#include "mario_actions_moving.h"
#include "mario_actions_stationary.h"
#include "mario_step.h"
#include "engine/math_util.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"
#include "engine/surface_collision.h"

u32 D_80339F10;
s8 filler80339F1C[0x80339F30 - 0x80339F1C];

// Sound terrain types. See audio_defines.h.
s8 D_8032CB40[7][6] = {
    {0, 3, 1, 1, 1, 0},
    {3, 3, 3, 3, 1, 1},
    {5, 6, 5, 6, 3, 3},
    {7, 3, 7, 7, 3, 3},
    {4, 4, 4, 4, 3, 3},
    {0, 3, 1, 6, 3, 6},
    {3, 3, 3, 3, 6, 6}
};

u8 D_8032CB7B[16] = {
    0x46, 0x32, 0x32, 0x3C,
    0x46, 0x50, 0x50, 0x3C,
    0x28, 0x14, 0x14, 0x1E,
    0x32, 0x3C, 0x3C, 0x28
};

// Equals [1000]^5 . [100]^8 . [10]^9 . [1] in binary
u64 D_8032CB80 = 0x4444449249255555;

s32 func_80250770(struct MarioState *m)
{
    struct Object *o = m->marioObj;
    return (o->header.gfx.unk38.animFrame + 1) == o->header.gfx.unk38.curAnim->unk08;
}

s32 func_802507AC(struct MarioState *m)
{
    struct Object *o = m->marioObj;
    return o->header.gfx.unk38.animFrame >= (o->header.gfx.unk38.curAnim->unk08 - 2);
}

s16 set_mario_animation(struct MarioState *m, s32 targetAnimID)
{
    struct Object *o = m->marioObj;
    struct Animation *targetAnim = m->animation->targetAnim;

    if (func_80278AD4(m->animation, targetAnimID))
    {
        targetAnim->values = (void *) VIRTUAL_TO_PHYSICAL((s8 *) targetAnim + (s32) targetAnim->values);
        targetAnim->index = (void *) VIRTUAL_TO_PHYSICAL((s8 *) targetAnim + (s32) targetAnim->index);
    }
    if (o->header.gfx.unk38.animID != targetAnimID)
    {
        o->header.gfx.unk38.animID = targetAnimID;
        o->header.gfx.unk38.curAnim = targetAnim;
        o->header.gfx.unk38.animAccel = 0;
        o->header.gfx.unk38.animYTrans = m->unkB0;
        if (targetAnim->flags & 4)
            o->header.gfx.unk38.animFrame = targetAnim->unk04;
        else
        {
            if (targetAnim->flags & 2)
                o->header.gfx.unk38.animFrame = targetAnim->unk04 + 1;
            else
                o->header.gfx.unk38.animFrame = targetAnim->unk04 - 1;
        }
    }
    return o->header.gfx.unk38.animFrame;
}

s16 func_80250934(struct MarioState *m, s32 targetAnimID, s32 arg2)
{
    struct Object *o = m->marioObj;
    struct Animation *targetAnim = m->animation->targetAnim;

    if (func_80278AD4(m->animation, targetAnimID))
    {
        targetAnim->values = (void *) VIRTUAL_TO_PHYSICAL((s8 *) targetAnim + (s32) targetAnim->values);
        targetAnim->index = (void *) VIRTUAL_TO_PHYSICAL((s8 *) targetAnim + (s32) targetAnim->index);
    }
    if (o->header.gfx.unk38.animID != targetAnimID)
    {
        o->header.gfx.unk38.animID = targetAnimID;
        o->header.gfx.unk38.curAnim = targetAnim;
        o->header.gfx.unk38.animYTrans = m->unkB0;
        if (targetAnim->flags & 4)
            o->header.gfx.unk38.animFrameAccelAssist = (targetAnim->unk04 << 0x10);
        else
        {
            if (targetAnim->flags & 2)
                o->header.gfx.unk38.animFrameAccelAssist = (targetAnim->unk04 << 0x10) + arg2;
            else
                o->header.gfx.unk38.animFrameAccelAssist = (targetAnim->unk04 << 0x10) - arg2;
        }
        o->header.gfx.unk38.animFrame = (o->header.gfx.unk38.animFrameAccelAssist >> 0x10);
    }
    o->header.gfx.unk38.animAccel = arg2;
    return o->header.gfx.unk38.animFrame;
}

void func_80250AAC(struct MarioState *m, s16 arg1)
{
    struct GraphNodeObject_sub *animInfo = &m->marioObj->header.gfx.unk38;
    struct Animation *curAnim = animInfo->curAnim;

    if (animInfo->animAccel)
    {
        if (curAnim->flags & 2)
            animInfo->animFrameAccelAssist = (arg1 << 0x10) + animInfo->animAccel;
        else
            animInfo->animFrameAccelAssist = (arg1 << 0x10) - animInfo->animAccel;
    }
    else
    {
        if (curAnim->flags & 2)
            animInfo->animFrame = arg1 + 1;
        else
            animInfo->animFrame = arg1 - 1;
    }
}

s32 func_80250B68(struct MarioState *m, s16 arg1)
{
    s32 sp14;
    s32 sp10 = arg1 << 0x10;
    struct GraphNodeObject_sub *spC = &m->marioObj->header.gfx.unk38;
    struct Animation *sp8 = spC->curAnim;

    if (spC->animAccel)
    {
        if (sp8->flags & 2)
            sp14 = (spC->animFrameAccelAssist > sp10) &&
                   (sp10 >= (spC->animFrameAccelAssist - spC->animAccel));
        else
            sp14 = (spC->animFrameAccelAssist < sp10) &&
                   (sp10 <= (spC->animFrameAccelAssist + spC->animAccel));
    }
    else
    {
        if (sp8->flags & 2)
            sp14 = (spC->animFrame == (arg1 + 1));
        else
            sp14 = ((spC->animFrame + 1) == arg1);
    }
    return sp14;
}

s16 func_80250C84(struct Object *obj, s32 arg1, Vec3s arg2)
{
    f32 sp34;
    f32 sp30;
    struct Animation *curAnim = (void *) obj->header.gfx.unk38.curAnim;
    s16 sp2A = func_8037C844(&obj->header.gfx.unk38, NULL);
    u16 *animIndex = (u16 *) segmented_to_virtual(curAnim->index);
    s16 *animValues = (s16 *) segmented_to_virtual(curAnim->values);
    f32 sp1C = (f32) sins(arg1);
    f32 sp18 = (f32) coss(arg1);

    sp34 = *(animValues + (func_8037C7D8(sp2A, &animIndex))) / 4.0f;
    arg2[1] = *(animValues + (func_8037C7D8(sp2A, &animIndex))) / 4.0f;
    sp30 = *(animValues + (func_8037C7D8(sp2A, &animIndex))) / 4.0f;
    arg2[0] = (sp34 * sp18) + (sp30 * sp1C);
    arg2[2] = (-sp34 * sp1C) + (sp30 * sp18);
    return curAnim->flags;
}

void func_80250E50(struct MarioState *m)
{
    Vec3s sp20;
    s16 sp30;

    sp30 = func_80250C84(m->marioObj, m->faceAngle[1], sp20);
    if (sp30 & 0x48)
    {
        m->pos[0] += (f32) sp20[0];
        m->pos[2] += (f32) sp20[2];
    }
    if (sp30 & 0x50)
        m->pos[1] += (f32) sp20[1];
}

s16 func_80250F0C(struct MarioState *m)
{
    Vec3s sp18;
    func_80250C84(m->marioObj, 0, sp18);
    return sp18[1];
}

void func_80250F50(struct MarioState *m, u32 arg1, u32 arg2)
{
    if ((m->flags & arg2) == 0)
    {
        SetSound(arg1, m->marioObj->header.gfx.cameraToObject);
        m->flags |= arg2;
    }
}

void func_80250FBC(struct MarioState *m)
{
    if (!(m->flags & MARIO_UNKNOWN_17))
    {
#ifdef VERSION_US
        if (m->action == ACT_TRIPLE_JUMP)
            SetSound(SOUND_MARIO_YAHOO2 + ((D_80226EB8 % 5) << 0x10),
                m->marioObj->header.gfx.cameraToObject);
        else
#endif
            SetSound(SOUND_MARIO_YAH + ((D_80226EB8 % 3) << 0x10),
                m->marioObj->header.gfx.cameraToObject);
        m->flags |= MARIO_UNKNOWN_17;
    }
}

void func_80251048(struct MarioState *m) {
    s32 absForwardVel = (m->forwardVel > 0.0f) ? m->forwardVel : -m->forwardVel;
    func_80320A4C(1, (absForwardVel >= 101) ? 100 : absForwardVel);
}

void func_802510E4(struct MarioState *m, u32 arg1, u32 arg2)
{
    if (m->unk14 == 0x20000)
    {
        if (arg2 != 0)
            m->particleFlags |= PARTICLE_12;
        else
            m->particleFlags |= PARTICLE_8;
    }
    else
    {
        if (m->unk14 == 0x70000)
            m->particleFlags |= PARTICLE_15;
        else if (m->unk14 == 0x50000)
            m->particleFlags |= PARTICLE_14;
    }
    if ((m->flags & MARIO_METAL_CAP) || arg1 == SOUND_ACTION_UNKNOWN443 || arg1 == SOUND_MARIO_HOO6)
        SetSound(arg1, m->marioObj->header.gfx.cameraToObject);
    else
        SetSound((m->unk14 + arg1), m->marioObj->header.gfx.cameraToObject);
}

void func_80251218(struct MarioState *m, u32 arg1, u32 arg2) {
    if ((m->flags & MARIO_UNKNOWN_16) == 0)
    {
        func_802510E4(m, arg1, arg2);
        m->flags |= MARIO_UNKNOWN_16;
    }
}

void func_80251280(struct MarioState *m, u32 arg1)
{
    func_802510E4(m, (m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_UNKNOWN429 : arg1, 1);
}

void func_802512E4(struct MarioState *m, u32 arg1)
{
    func_80251218(m, (m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_UNKNOWN429 : arg1, 1);
}

void func_80251348(struct MarioState *m, u32 arg1)
{
    func_802510E4(m, (m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_UNKNOWN42B : arg1, 1);
}

void func_802513AC(struct MarioState *m, u32 arg1)
{
    func_80251218(m, (m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_UNKNOWN42B : arg1, 1);
}

void func_80251410(struct MarioState *m, s32 arg1, s32 arg2)
{
    if (arg1 == SOUND_TERRAIN_1)
        func_80251218(m, (m->flags & MARIO_METAL_CAP) ? SOUND_ACTION_UNKNOWN428 : SOUND_TERRAIN_1, 1);
    else
        func_80250F50(m, arg1, MARIO_UNKNOWN_16);

    if (arg2 == 0)
        func_80250FBC(m);

    if (arg2 != -1)
        func_80250F50(m, arg2, MARIO_UNKNOWN_17);
}

void mario_set_forward_vel(struct MarioState *m, f32 forwardVel)
{
    m->forwardVel = forwardVel;
    m->slideVelX = sins(m->faceAngle[1]) * m->forwardVel;
    m->slideVelZ = coss(m->faceAngle[1]) * m->forwardVel;
    m->vel[0] = (f32) m->slideVelX;
    m->vel[2] = (f32) m->slideVelZ;
}

extern s32 mario_get_floor_class(struct MarioState *m)
{
    s32 floorClass;

    if ((m->area->terrainType & 7) == TERRAIN_SLIDE)
        floorClass = SURFACE_CLASS_SLIDE;
    else
        floorClass = SURFACE_CLASS_DEFAULT;
    if (m->floor)
    {
        switch (m->floor->type)
        {
            case SURFACE_NOT_SLIPPERY:
            case SURFACE_HARD_NOT_SLIPPERY:
            case SURFACE_SWITCH:
                floorClass = SURFACE_CLASS_NOT_SLIPPERY;
                break;
            case SURFACE_SLIPPERY:
            case SURFACE_002A:   //Slippery with noise
            case SURFACE_HARD_SLIPPERY:
            case SURFACE_0079:
                floorClass = SURFACE_CLASS_SLIPPERY;
                break;
            case SURFACE_SLIDE:
            case SURFACE_ICE:
            case SURFACE_HARD_SLIDE:
            case SURFACE_0073:    //Slide with noise, unused
            case SURFACE_0074:    //Slide with noise, unused
            case SURFACE_0075:    //Slide with noise
            case SURFACE_0078:
                floorClass = SURFACE_CLASS_SLIDE;
                break;
        }
    }
    if (m->action == ACT_CRAWLING && m->floor->normal.y > 0.5f && floorClass == SURFACE_CLASS_DEFAULT)
        floorClass = SURFACE_CLASS_NOT_SLIPPERY;
    return floorClass;
}

u32 func_8025167C(struct MarioState *m)
{
    s16 spE;
    s16 terrainType = m->area->terrainType & 7;
    s32 sp8 = 0;
    s32 floorType;

    if (m->floor)
    {
        floorType = m->floor->type;
        if ((gCurrLevelNum != LEVEL_LLL) && (m->floorHeight < (m->waterLevel - 10)))
            sp8 = 0x20000;
        else if (SURFACE_IS_QUICKSAND(floorType))
            sp8 = 0x70000;
        else
        {
            switch (floorType)
            {
                default:
                    spE = 0;
                    break;
                case SURFACE_NOT_SLIPPERY:
                case SURFACE_HARD:
                case SURFACE_HARD_NOT_SLIPPERY:
                case SURFACE_SWITCH:
                    spE = 1;
                    break;
                case SURFACE_SLIPPERY:
                case SURFACE_HARD_SLIPPERY:
                case SURFACE_0079:
                    spE = 2;
                    break;
                case SURFACE_SLIDE:
                case SURFACE_ICE:
                case SURFACE_HARD_SLIDE:
                case SURFACE_0073:
                case SURFACE_0074:
                case SURFACE_0075:
                case SURFACE_0078:
                    spE = 3;
                    break;
                case SURFACE_0029:
                    spE = 4;
                    break;
                case SURFACE_002A:
                    spE = 5;
                    break;
            }
            sp8 = D_8032CB40[terrainType][spE] << 0x10;
        }
    }
    return sp8;
}

struct Surface *func_8025181C(Vec3f arg0, f32 arg1, f32 arg2)
{
    struct WallCollisionData collisionData;
    struct Surface *wall = NULL;

    collisionData.x = arg0[0];
    collisionData.y = arg0[1];
    collisionData.z = arg0[2];
    collisionData.radius = arg2;
    collisionData.offsetY = arg1;

    if (find_wall_collisions(&collisionData))
        wall = collisionData.walls[collisionData.numWalls - 1];

    arg0[0] = collisionData.x;
    arg0[1] = collisionData.y;
    arg0[2] = collisionData.z;

    return wall;
}


f32 func_802518D0(Vec3f arg0, f32 arg1, struct Surface **ceil)
{
    f32 tmp UNUSED;  // why...
    return find_ceil(arg0[0], arg1 + 80.0f, arg0[2], ceil);
}

s32 mario_facing_downhill(struct MarioState *m, s32 arg1)
{
    s16 faceAngleYaw = m->faceAngle[1];
    if (arg1 && m->forwardVel < 0.0f)
        faceAngleYaw += 0x8000;

    faceAngleYaw = m->floorAngle - faceAngleYaw;
    return (-0x4000 < faceAngleYaw) && (faceAngleYaw < 0x4000);
}

u32 func_802519A8(struct MarioState *m)
{
    f32 sp24;

    if ((m->area->terrainType & 7) == TERRAIN_SLIDE && m->floor->normal.y < 0.9998477f)
        return TRUE;

    switch (mario_get_floor_class(m))
    {
        case SURFACE_SLIDE:
            sp24 = 0.9848077f;
            break;
        case SURFACE_SLIPPERY:
            sp24 = 0.9396926f;
            break;
        default:
            sp24 = 0.7880108f;
            break;
        case SURFACE_NOT_SLIPPERY:
            sp24 = 0.0f;
            break;
    }
    return m->floor->normal.y <= sp24;
}

s32 mario_floor_is_slope(struct MarioState *m)
{
    f32 tmp;

    if ((m->area->terrainType & 0x0007) == TERRAIN_SLIDE && m->floor->normal.y < 0.9998477f)
        return TRUE;

    switch (mario_get_floor_class(m))
    {
        case SURFACE_SLIDE:
            tmp = 0.9961947f;
            break;
        case SURFACE_SLIPPERY:
            tmp = 0.9848077f;
            break;
        default:
            tmp = 0.9659258f;
            break;
        case SURFACE_NOT_SLIPPERY:
            tmp = 0.9396926f;
            break;
    }
    return m->floor->normal.y <= tmp;
}

s32 func_80251BF8(struct MarioState *m)
{
    f32 tmp;
    s32 result = FALSE;
    if (!mario_facing_downhill(m, FALSE))
    {
        switch (mario_get_floor_class(m))
        {
            case SURFACE_SLIDE:
                tmp = 0.9659258f;
                break;
            case SURFACE_SLIPPERY:
                tmp = 0.9396926f;
                break;
            default:
                tmp = 0.8660254f;
                break;
            case SURFACE_NOT_SLIPPERY:
                tmp = 0.8660254f;
                break;
        }
        result = m->floor->normal.y <= tmp;
    }
    return result;
}

f32 find_floor_height_relative_polar(struct MarioState *m, s32 angleFromMario, f32 distFromMario)
{
    struct Surface *floor;
    f32 floorY;
    f32 y = sins(m->faceAngle[1] + (s16) angleFromMario) * distFromMario;
    f32 x = coss(m->faceAngle[1] + (s16) angleFromMario) * distFromMario;

    floorY = find_floor(m->pos[0] + y, m->pos[1] + 100.0f, m->pos[2] + x, &floor);
    return floorY;
}

s16 func_80251DD4(struct MarioState *m, s16 unk1)
{
    struct Surface *floor;
    f32 forwardFloorY, backwardFloorY;
    f32 forwardYDelta, backwardYDelta;
    s16 result;
    f32 y = sins(m->faceAngle[1] + unk1) * 5.0f;
    f32 x = coss(m->faceAngle[1] + unk1) * 5.0f;

    forwardFloorY = find_floor(m->pos[0] + y, m->pos[1] + 100.0f, m->pos[2] + x, &floor);
    backwardFloorY = find_floor(m->pos[0] - y, m->pos[1] + 100.0f, m->pos[2] - x, &floor);
    forwardYDelta = forwardFloorY - m->pos[1];
    backwardYDelta = m->pos[1] - backwardFloorY;
    if (forwardYDelta * forwardYDelta < backwardYDelta * backwardYDelta)
        result = atan2s(5.0f, forwardYDelta);
    else
        result = atan2s(5.0f, backwardYDelta);

    return result;
}

void func_80251F74(struct MarioState *m)
{
    u32 action = m->action;
    s32 unk0 = m->area->camera->currPreset;

    if (action == ACT_FIRST_PERSON)
    {
        func_80248CB8(2);
        gCameraMovementFlags &= ~CAM_MOVE_C_UP_MODE;
        func_80285BD8(m->area->camera, -1, 1);
    }
    else if (action == ACT_SLEEPING)
        func_80248CB8(2);

    if (!(action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER)))
    {
        if (unk0 == CAMERA_PRESET_BEHIND_MARIO || unk0 == CAMERA_PRESET_WATER_SURFACE)
            func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
    }
}

void func_80252070(struct MarioState *m)
{
    m->marioObj->oMarioSteepJumpYaw = m->faceAngle[1];
    if (m->forwardVel > 0.0f)
    {
        //! ((s16)0x8000) has undefined behavior. Therefore, this downcast has
        // undefined behavior if m->floorAngle >= 0.
        s16 angleTemp = m->floorAngle + 0x8000;
        s16 faceAngleTemp = m->faceAngle[1] - angleTemp;
        f32 y = sins(faceAngleTemp) * m->forwardVel;
        f32 x = coss(faceAngleTemp) * m->forwardVel * 0.75f;
        m->forwardVel = sqrtf(y * y + x * x);
        m->faceAngle[1] = atan2s(x, y) + angleTemp;
    }
    drop_and_set_mario_action(m, ACT_STEEP_JUMP, 0);
}

static void func_8025219C(struct MarioState *m, f32 unk1, f32 unk2)
{
    m->vel[1] = unk1 + zero_80254E20() + m->forwardVel * unk2;
    if (m->squishTimer != 0 || m->quicksandDepth > 1.0f)
        m->vel[1] *= 0.5f;
}

static u32 set_mario_action_airborne(struct MarioState *m, u32 action, u32 actionArg)
{
    f32 tmp;

    if (m->squishTimer != 0 || m->quicksandDepth >= 1.0f)
    {
        if (action == ACT_DOUBLE_JUMP || action == ACT_TWIRLING)
            action = ACT_JUMP;
    }

    switch (action)
    {
        case ACT_DOUBLE_JUMP:
            func_8025219C(m, 52.0f, 0.25f);
            m->forwardVel *= 0.8f;
            break;

        case ACT_BACKFLIP:
            m->marioObj->header.gfx.unk38.animID = -1;
            m->forwardVel = -16.0f;
            func_8025219C(m, 62.0f, 0.0f);
            break;

        case ACT_TRIPLE_JUMP:
            func_8025219C(m, 69.0f, 0.0f);
            m->forwardVel *= 0.8f;
            break;

        case ACT_FLYING_TRIPLE_JUMP:
            func_8025219C(m, 82.0f, 0.0f);
            break;

        case ACT_WATER_JUMP:
        case ACT_HOLD_WATER_JUMP:
             if (actionArg == 0)
                 func_8025219C(m, 42.0f, 0.0f);
             break;

        case ACT_BURNING_JUMP:
             m->vel[1] = 31.5f;
             m->forwardVel = 8.0f;
             break;

        case ACT_RIDING_SHELL_JUMP:
            func_8025219C(m, 42.0f, 0.25f);
            break;

        case ACT_JUMP:
        case ACT_HOLD_JUMP:
            m->marioObj->header.gfx.unk38.animID = -1;
            func_8025219C(m, 42.0f, 0.25f);
            m->forwardVel *= 0.8f;
            break;

        case ACT_WALL_KICK_AIR:
        case ACT_TOP_OF_POLE_JUMP:
            func_8025219C(m, 62.0f, 0.0f);
            if (m->forwardVel < 24.0f)
                m->forwardVel = 24.0f;
            m->wallKickTimer = 0;
            break;

        case ACT_SIDE_FLIP:
            func_8025219C(m, 62.0f, 0.0f);
            m->forwardVel = 8.0f;
            m->faceAngle[1] = m->intendedYaw;
            break;

        case ACT_STEEP_JUMP:
            m->marioObj->header.gfx.unk38.animID = -1;
            func_8025219C(m, 42.0f, 0.25f);
            m->faceAngle[0] = -0x2000;
            break;

        case ACT_LAVA_BOOST:
            m->vel[1] = 84.0f;
            if (actionArg == 0) {
                m->forwardVel = 0.0f;
            }
            break;

        case ACT_DIVE:
            if ((tmp = m->forwardVel + 15.0f) > 48.0f)
                tmp = 48.0f;
            mario_set_forward_vel(m, tmp);
            break;

        case ACT_LONG_JUMP:
            m->marioObj->header.gfx.unk38.animID = -1;
            func_8025219C(m, 30.0f, 0.0f);
            m->marioObj->oMarioLongJumpIsSlow = m->forwardVel > 16.0f ? FALSE : TRUE;
            if ((m->forwardVel *= 1.5f) > 48.0f)
                m->forwardVel = 48.0f;
            break;

        case ACT_SLIDE_KICK:
            m->vel[1] = 12.0f;
            if (m->forwardVel < 32.0f)
                m->forwardVel = 32.0f;
            break;

        case ACT_JUMP_KICK:
            m->vel[1] = 20.0f;
            break;
    }

    m->peakHeight = m->pos[1];
    m->flags |= MARIO_UNKNOWN_08;
    return action;
}

static u32 set_mario_action_moving(struct MarioState *m, u32 action, UNUSED u32 actionArg)
{
    s16 floorClass = mario_get_floor_class(m);
    f32 forwardVel = m->forwardVel;
    f32 mag = min(m->intendedMag, 8.0f);

    switch (action) {
        case ACT_WALKING:
            if (floorClass != SURFACE_CLASS_SLIDE)
            {
                if (0.0f <= forwardVel && forwardVel < mag)
                    m->forwardVel = mag;
            }
            m->marioObj->OBJECT_FIELD_S32(0x22) = 0;
            break;

        case ACT_HOLD_WALKING:
            if (0.0f <= forwardVel && forwardVel < mag / 2.0f)
                m->forwardVel = mag / 2.0f;
            break;

        case ACT_BEGIN_SLIDING:
            if (mario_facing_downhill(m, 0))
                action = ACT_BUTT_SLIDE;
            else
                action = ACT_STOMACH_SLIDE;
            break;

        case ACT_HOLD_BEGIN_SLIDING:
            if (mario_facing_downhill(m, 0))
                action = ACT_HOLD_BUTT_SLIDE;
            else
                action = ACT_HOLD_STOMACH_SLIDE;
            break;
    }

    return action;
}

static u32 set_mario_action_submerged(struct MarioState *m, u32 action, UNUSED u32 actionArg)
{
    if (action == ACT_METAL_WATER_JUMP || action == ACT_HOLD_METAL_WATER_JUMP)
        m->vel[1] = 32.0f;

    return action;
}

static u32 set_mario_action_cutscene(struct MarioState *m, u32 action, UNUSED u32 actionArg)
{
    switch (action)
    {
        case ACT_EMERGE_FROM_PIPE:
            m->vel[1] = 52.0f;
            break;

        case ACT_FALL_AFTER_STAR_GRAB:
            mario_set_forward_vel(m, 0.0f);
            break;

        case ACT_SPAWN_SPIN_AIRBORNE:
            mario_set_forward_vel(m, 2.0f);
            break;

        case ACT_SPECIAL_EXIT_AIRBORNE:
        case ACT_SPECIAL_DEATH_EXIT:
            m->vel[1] = 64.0f;
            break;
    }

    return action;
}

u32 set_mario_action(struct MarioState *m, u32 action, u32 actionArg)
{
    switch (action & ACT_GROUP_MASK)
    {
        case ACT_GROUP_MOVING:
            action = set_mario_action_moving(m, action, actionArg);
            break;

        case ACT_GROUP_AIRBORNE:
            action = set_mario_action_airborne(m, action, actionArg);
            break;

        case ACT_GROUP_SUBMERGED:
            action = set_mario_action_submerged(m, action, actionArg);
            break;

        case ACT_GROUP_CUTSCENE:
            action = set_mario_action_cutscene(m, action, actionArg);
            break;
    }

    m->flags &= ~(MARIO_UNKNOWN_16 | MARIO_UNKNOWN_17);
    if (!(m->action & ACT_FLAG_AIR))
        m->flags &= ~MARIO_UNKNOWN_18;

    m->prevAction = m->action;
    m->action = action;
    m->actionArg = actionArg;
    m->actionState = 0;
    m->actionTimer = 0;
    return TRUE;
}

s32 func_80252C30(struct MarioState *m)
{
    if (m->quicksandDepth >= 11.0f)
    {
        if (m->heldObj == NULL)
            return set_mario_action(m, ACT_QUICKSAND_JUMP_LAND, 0);
        else
            return set_mario_action(m, ACT_HOLD_QUICKSAND_JUMP_LAND, 0);
    }

    if (func_80251BF8(m))
        func_80252070(m);
    else
    {
        if ((m->doubleJumpTimer == 0) || (m->squishTimer != 0))
            set_mario_action(m, ACT_JUMP, 0);
        else
        {
            switch (m->prevAction)
            {
                case ACT_JUMP_LAND:
                    set_mario_action(m, ACT_DOUBLE_JUMP, 0);
                    break;
                case ACT_FREEFALL_LAND:
                    set_mario_action(m, ACT_DOUBLE_JUMP, 0);
                    break;
                case ACT_SIDE_FLIP_LAND_STOP:
                    set_mario_action(m, ACT_DOUBLE_JUMP, 0);
                    break;
                case ACT_DOUBLE_JUMP_LAND:
                    if (m->flags & MARIO_WING_CAP)
                        set_mario_action(m, ACT_FLYING_TRIPLE_JUMP, 0);
                    else if (m->forwardVel > 20.0f)
                        set_mario_action(m, ACT_TRIPLE_JUMP, 0);
                    else
                        set_mario_action(m, ACT_JUMP, 0);
                    break;
                default:
                    set_mario_action(m, ACT_JUMP, 0);
                    break;
            }
        }
    }
    m->doubleJumpTimer = 0;
    return TRUE;
}

s32 func_80252E74(struct MarioState *m, u32 arg1, u32 arg2)
{
    u32 action UNUSED = m->action;

    if (m->quicksandDepth >= 11.0f)
    {
        if (m->heldObj == NULL)
            return set_mario_action(m, ACT_QUICKSAND_JUMP_LAND, 0);
        else
            return set_mario_action(m, ACT_HOLD_QUICKSAND_JUMP_LAND, 0);
    }

    if (func_80251BF8(m))
        func_80252070(m);
    else
        set_mario_action(m, arg1, arg2);
    return TRUE;
}

s32 drop_and_set_mario_action(struct MarioState *m, u32 arg1, u32 arg2)
{
    mario_stop_riding_and_holding(m);
    return set_mario_action(m, arg1, arg2);
}

s32 func_80252F98(struct MarioState *m, u32 arg1, u32 arg2, s16 hurtCounter)
{
    m->hurtCounter = hurtCounter;
    return set_mario_action(m, arg1, arg2);
}

s32 func_80252FEC(struct MarioState *m)
{
    if (m->input & INPUT_A_PRESSED)
        return set_mario_action(m, ACT_JUMP, 0);
    if (m->input & INPUT_OFF_FLOOR)
        return set_mario_action(m, ACT_FREEFALL, 0);
    if (m->input & INPUT_NONZERO_ANALOG)
        return set_mario_action(m, ACT_WALKING, 0);
    if (m->input & INPUT_ABOVE_SLIDE)
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    return 0;
}

s32 func_802530D4(struct MarioState *m)
{
    if (m->input & INPUT_A_PRESSED)
        return set_mario_action(m, ACT_HOLD_JUMP, 0);
    if (m->input & INPUT_OFF_FLOOR)
        return set_mario_action(m, ACT_HOLD_FREEFALL, 0);
    if (m->input & INPUT_NONZERO_ANALOG)
        return set_mario_action(m, ACT_HOLD_WALKING, 0);
    if (m->input & INPUT_ABOVE_SLIDE)
        return set_mario_action(m, ACT_HOLD_BEGIN_SLIDING, 0);
    return 0;
}

s32 func_802531B8(struct MarioState *m)
{
    func_80285BD8(m->area->camera, m->area->camera->defPreset, 1);
    vec3s_set(m->angleVel, 0, 0, 0);
    if (m->heldObj == NULL)
        return set_mario_action(m, ACT_WALKING, 0);
    else
        return set_mario_action(m, ACT_HOLD_WALKING, 0);
}

s32 func_8025325C(struct MarioState *m)
{
    m->forwardVel = m->forwardVel / 4.0f;
    m->vel[1] = m->vel[1] / 2.0f;
    m->pos[1] = m->waterLevel - 100;
    m->faceAngle[2] = 0;
    vec3s_set(m->angleVel, 0, 0, 0);

    if ((m->action & ACT_FLAG_DIVING) == 0)
        m->faceAngle[0] = 0;

    if (m->area->camera->currPreset != CAMERA_PRESET_WATER_SURFACE)
        func_80285BD8(m->area->camera, 8, 1);

    return set_mario_action(m, ACT_WATER_PLUNGE, 0);
}

void func_8025335C(struct MarioState *m)
{
    if (m->squishTimer != 0xFF)
    {
        if (m->squishTimer == 0)
            vec3f_set(m->marioObj->header.gfx.scale, 1.0f, 1.0f, 1.0f);
        else if (m->squishTimer <= 16)
        {
            m->squishTimer--;
            m->marioObj->header.gfx.scale[1] =
                1.0f - ((D_8032CB7B[15 - m->squishTimer] * 0.6f) / 100.0f);
            m->marioObj->header.gfx.scale[0] =
                ((D_8032CB7B[15 - m->squishTimer] * 0.4f) / 100.0f) + 1.0f;
            m->marioObj->header.gfx.scale[2] = m->marioObj->header.gfx.scale[0];
        }
        else
        {
            m->squishTimer--;
            vec3f_set(m->marioObj->header.gfx.scale, 1.4f, 0.4f, 1.4f);
        }
    }
}

void func_802534F4(struct MarioState *m)
{
    f32 steepness;
    f32 floor_nY;

    if (gShowDebugText)
    {
        steepness = sqrtf(((m->floor->normal.x * m->floor->normal.x) +
                           (m->floor->normal.z * m->floor->normal.z)));
        floor_nY = m->floor->normal.y;
        print_text_fmt_int(210, 88, "ANG %d", (atan2s(floor_nY, steepness) * 180.0f) / 32768.0f);
        print_text_fmt_int(210, 72, "SPD %d", m->forwardVel);
        print_text_fmt_int(210, 56, "STA %x", (m->action & ACT_ID_MASK));
    }
}

void func_8025360C(struct MarioState *m)
{
    if (m->controller->buttonPressed & A_BUTTON)
        m->input |= INPUT_A_PRESSED;

    if (m->controller->buttonDown & A_BUTTON)
        m->input |= INPUT_A_DOWN;

    if (m->squishTimer == 0)
    {
        if (m->controller->buttonPressed & B_BUTTON)
            m->input |= INPUT_B_PRESSED;

        if (m->controller->buttonDown & Z_TRIG)
            m->input |= INPUT_Z_DOWN;

        if (m->controller->buttonPressed & Z_TRIG)
            m->input |= INPUT_Z_PRESSED;
    }

    if (m->input & INPUT_A_PRESSED)
        m->framesSinceA = 0;
    else if (m->framesSinceA < 0xFF)
        m->framesSinceA += 1;

    if (m->input & INPUT_B_PRESSED)
        m->framesSinceB = 0;
    else if (m->framesSinceB < 0xff)
        m->framesSinceB += 1;
}

void func_80253730(struct MarioState *m)
{
    struct Controller *controller = m->controller;
    f32 sp18 = ((controller->stickMag / 64.0f) *
                (controller->stickMag / 64.0f)) * 64.0f;

    if (m->squishTimer == 0)
        m->intendedMag = sp18 / 2.0f;
    else
        m->intendedMag = sp18 / 8.0f;

    if (m->intendedMag > 0.0f)
    {
        m->intendedYaw =
            atan2s(-controller->stickY, controller->stickX) + m->area->camera->trueYaw;
        m->input |= INPUT_NONZERO_ANALOG;
    }
    else
        m->intendedYaw = m->faceAngle[1];
}

void func_80253834(struct MarioState *m)
{
    f32 sp24;
    f32 ceilToFloorDist;

    resolve_wall_collisions(&m->pos[0], &m->pos[1], &m->pos[2], 60.0f, 50.0f);
    resolve_wall_collisions(&m->pos[0], &m->pos[1], &m->pos[2], 30.0f, 24.0f);

    m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
    if (m->floor == 0)
    {
        vec3f_copy(m->pos, m->marioObj->header.gfx.pos);
        m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
    }

    m->ceilHeight = func_802518D0(&m->pos[0], m->floorHeight, &m->ceil);
    sp24 = func_80381D3C(m->pos[0], m->pos[2]);
    m->waterLevel = find_water_level(m->pos[0], m->pos[2]);

    if (m->floor)
    {
        m->floorAngle = atan2s(m->floor->normal.z, m->floor->normal.x);
        m->unk14 = func_8025167C(m);

        if ((m->pos[1] > m->waterLevel - 0x28) && func_802519A8(m))
                m->input |= INPUT_ABOVE_SLIDE;

        if ((m->floor->flags & 1) || (m->ceil && m->ceil->flags & 1))
        {
            ceilToFloorDist = m->ceilHeight - m->floorHeight;
            if ((0.0f <= ceilToFloorDist) && (ceilToFloorDist <= 150.0f))
                m->input |= INPUT_SQUISHED;
        }

        if (m->pos[1] > m->floorHeight + 100.0f)
            m->input |= INPUT_OFF_FLOOR;

        if (m->pos[1] < (m->waterLevel - 0xa))
            m->input |= INPUT_IN_WATER;

        if (m->pos[1] < (sp24 - 100.0f))
            m->input |= INPUT_UNKNOWN_8;

    }
    else
        level_trigger_warp(m, WARP_OP_DEATH);
}

void func_80253B2C(struct MarioState *m)
{
    m->particleFlags = 0;
    m->input = 0;
    m->collidedObjInteractTypes = m->marioObj->collidedObjInteractTypes;
    m->flags &= 0xFFFFFF;

    func_8025360C(m);
    func_80253730(m);
    func_80253834(m);
    func_802534F4(m);
    if (gCameraMovementFlags & CAM_MOVE_C_UP_MODE)
    {
        if (m->action & 0x4000000)
            m->input |= INPUT_FIRST_PERSON;
        else
            gCameraMovementFlags &= ~CAM_MOVE_C_UP_MODE;
    }

    if (!(m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED)))
        m->input |= INPUT_UNKNOWN_5;

    if (m->marioObj->oInteractStatus & 0x13)
        m->input |= INPUT_UNKNOWN_10;

    nop_80254E3C(m);
    if (m->wallKickTimer > 0)
        m->wallKickTimer--;

    if (m->doubleJumpTimer > 0)
        m->doubleJumpTimer--;
}

void func_80253C94(struct MarioState *m)
{
    f32 sp1C;
    s16 sp1A;

    if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED)
    {
        sp1C = (f32) (m->waterLevel - 80) - m->pos[1];
        sp1A = m->area->camera->currPreset;
        if ((m->action & ACT_FLAG_METAL_WATER))
        {
            if (sp1A != CAMERA_PRESET_CLOSE)
                func_80285BD8(m->area->camera, 4, 1);
        }
        else
        {
            if ((sp1C > 800.0f) && (sp1A != CAMERA_PRESET_BEHIND_MARIO))
                func_80285BD8(m->area->camera, 3, 1);

            if ((sp1C < 400.0f) && (sp1A != CAMERA_PRESET_WATER_SURFACE))
                func_80285BD8(m->area->camera, 8, 1);

            if ((m->action & ACT_FLAG_INTANGIBLE) == 0)
            {
                if ((m->pos[1] < (f32) (m->waterLevel - 160)) ||
                        (m->faceAngle[0] < -0x800))
                    m->particleFlags |= PARTICLE_5;
            }
        }
    }
}

void func_80253E34(struct MarioState *m)
{
    s32 terrainIsSnow;

    if (m->health >= 0x100)
    {
        if ((m->healCounter | m->hurtCounter) == 0)
        {
            if ((m->input & INPUT_UNKNOWN_8) && ((m->action & ACT_FLAG_INTANGIBLE) == 0))
            {
                if (((m->flags & MARIO_METAL_CAP) == 0) && (gDebugLevelSelect == 0))
                    m->health -= 4;
            }
            else
            {
                if ((m->action & 0x2000) && ((m->action & 0x1000) == 0))
                {
                    terrainIsSnow = (m->area->terrainType & 0x7) == TERRAIN_SNOW;

                    if ((m->pos[1] >= (m->waterLevel - 140)) && !terrainIsSnow)
                        m->health += 0x1A;
                    else if (gDebugLevelSelect == 0)
                        m->health -= (terrainIsSnow ? 3 : 1);
                }
            }
        }
        if (m->healCounter > 0)
        {
            m->health += 0x40;
            m->healCounter--;
        }
        if (m->hurtCounter > 0)
        {
            m->health -= 0x40;
            m->hurtCounter--;
        }
        if (m->health >= 0x881)
            m->health = 0x880;

        if (m->health < 0x100)
            m->health = 0xFF;

        if (((m->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED) && (m->health < 0x300))
            SetSound(SOUND_UNKNOWN_UNK1C18, D_803320E0);
    }
}

void func_80254088(struct MarioState *m)
{
    m->unk98->action = m->action;
    m->unk94->action = m->action;

    vec3s_copy(m->unk94->faceAngle, m->faceAngle);

    if ((m->flags & MARIO_UNKNOWN_25) == 0)
        vec3f_copy(m->unk94->pos, m->pos);
}

void func_8025410C(struct MarioState *m)
{
    struct MarioBodyState *sp4 = m->unk98;
    sp4->capState = MARIO_HAS_DEFAULT_CAP_OFF;
    sp4->eyeState = MARIO_EYES_BLINK;
    sp4->handState = MARIO_HAND_FISTS;
    sp4->unk08 = 0;
    sp4->unk07 = 0;
    m->flags &= ~0x40;
}

void func_80254164(struct MarioState *m)
{
    struct Object *o = m->marioObj;

    if (o->header.gfx.throwMatrix)
    {
        // TODO: throwMatrix should probably be an actual matrix pointer
        *(f32 *)((u8 *) o->header.gfx.throwMatrix + 0x34) -= m->quicksandDepth;
    }

    o->header.gfx.pos[1] -= m->quicksandDepth;
}

s32 func_802541BC(struct MarioState *m)
{
    s32 flags = m->flags;
    s32 action;

    if (m->capTimer > 0)
    {
        action = (s32) m->action;

        if (
            (m->capTimer < 0x3D) ||
            (
                (action != ACT_READING_AUTOMATIC_DIALOGUE) &&
                    (action != ACT_READING_NPC_DIALOGUE) &&
                    (action != ACT_READING_SIGN) &&
                    (action != ACT_IN_CANNON)
            )
        )
        {
            m->capTimer--;
        }

        if (m->capTimer == 0)
        {
            func_80249418();
            m->flags &= ~(MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP);
            if ((m->flags &
                    (MARIO_UNKNOWN_00 | MARIO_VANISH_CAP |
                     MARIO_METAL_CAP | MARIO_WING_CAP)) == 0)
                m->flags &= ~MARIO_CAP_ON_HEAD;
        }
        if (m->capTimer == 0x3C)
            func_802493D4();

        if ((m->capTimer < 0x40) && ((1LL << m->capTimer) & D_8032CB80))
        {
            flags &= ~(MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP);
            if ((flags &
                    (MARIO_UNKNOWN_00 | MARIO_VANISH_CAP |
                     MARIO_METAL_CAP | MARIO_WING_CAP)) == 0)
                flags &= ~MARIO_CAP_ON_HEAD;
        }

    }
    return flags;
}

void func_8025435C(struct MarioState *m)
{
    struct MarioBodyState *sp1C = m->unk98;
    s32 sp18 = func_802541BC(m);

    if (sp18 & MARIO_VANISH_CAP)
        sp1C->unk08 = 0x180;
    if (sp18 & MARIO_METAL_CAP)
        sp1C->unk08 |= 0x200;
    if (sp18 & MARIO_UNKNOWN_06)
        sp1C->unk08 |= 0x200;

    if (m->invincTimer >= 3)
    {
        if (gGlobalTimer & 1)
            gMarioState->marioObj->header.gfx.node.flags |= 0x10;
    }
    if (sp18 & MARIO_CAP_IN_HAND)
    {
        if (sp18 & MARIO_WING_CAP)
            sp1C->handState = MARIO_HAND_HOLDING_WING_CAP;
        else
            sp1C->handState = MARIO_HAND_HOLDING_CAP;
    }
    if (sp18 & MARIO_CAP_ON_HEAD)
    {
        if (sp18 & MARIO_WING_CAP)
            sp1C->capState = MARIO_HAS_WING_CAP_ON;
        else
            sp1C->capState = MARIO_HAS_DEFAULT_CAP_ON;
    }
    if (m->action & ACT_FLAG_SHORT_HITBOX)
        m->marioObj->hitboxHeight = 100.0f;
    else
        m->marioObj->hitboxHeight = 160.0f;
    if ((m->flags & MARIO_TELEPORTING) && (m->fadeWarpOpacity != 0xFF))
    {
        sp1C->unk08 &= ~0xFF;
        sp1C->unk08 |= (m->fadeWarpOpacity | 0x100);
    }
}

void Unknown8025453C(u16 arg0, s32 arg1, u16 arg2, u16 arg3)
{
    if ((gPlayer1Controller->buttonDown & 0x2000) &&
            (gPlayer1Controller->buttonPressed & arg0) &&
            ((gMarioState->flags & arg1) == 0))
    {
        gMarioState->flags |= (arg1 + MARIO_CAP_ON_HEAD);

        if (arg2 > gMarioState->capTimer)
            gMarioState->capTimer = arg2;

        func_80249368(arg3);
    }
}

s32 func_80254604(UNUSED struct Object *arg0)
{
    s32 sp24 = TRUE;

    if (gMarioState->action)
    {
        gMarioState->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_10;
        func_8025410C(gMarioState);
        func_80253B2C(gMarioState);
        mario_handle_special_floors(gMarioState);
        mario_process_interactions(gMarioState);

        if (gMarioState->floor == NULL)
            return 0;

        while (sp24)
        {
            switch (gMarioState->action & 0x1c0)
            {
                case ACT_GROUP_STATIONARY:
                    sp24 = mario_execute_stationary_action(gMarioState);
                    break;
                case ACT_GROUP_MOVING:
                    sp24 = mario_execute_moving_action(gMarioState);
                    break;
                case ACT_GROUP_AIRBORNE:
                    sp24 = mario_execute_airborne_action(gMarioState);
                    break;
                case ACT_GROUP_SUBMERGED:
                    sp24 = mario_execute_submerged_action(gMarioState);
                    break;
                case ACT_GROUP_CUTSCENE:
                    sp24 = mario_execute_cutscene_action(gMarioState);
                    break;
                case ACT_GROUP_AUTOMATIC:
                    sp24 = mario_execute_automatic_action(gMarioState);
                    break;
                case ACT_GROUP_OBJECT:
                    sp24 = mario_execute_object_action(gMarioState);
                    break;
            }
        }

        func_80254164(gMarioState);
        func_8025335C(gMarioState);
        func_80253C94(gMarioState);
        func_80253E34(gMarioState);
        func_80254088(gMarioState);
        func_8025435C(gMarioState);

        if (gMarioState->floor->type == SURFACE_HORIZONTAL_WIND)
        {
            func_802ADC20(0, (gMarioState->floor->force << 8));
            #if VERSION_US
                SetSound(SOUND_ENVIRONMENT_WIND2, gMarioState->marioObj->header.gfx.cameraToObject);
            #endif
        }

        if (gMarioState->floor->type == SURFACE_VERTICAL_WIND)
        {
            func_802ADC20(1, 0);
            #if VERSION_US
                SetSound(SOUND_ENVIRONMENT_WIND2, gMarioState->marioObj->header.gfx.cameraToObject);
            #endif
        }

        func_80249040();
        gMarioState->marioObj->oInteractStatus = 0;
        return gMarioState->particleFlags;
    }
    return 0;
}

void func_802548BC(void)
{
    Vec3s capPos;
    struct Object *capObject;

    D_80339F10 = 0;
    gMarioState->actionTimer = 0;
    gMarioState->framesSinceA = 0xFF;
    gMarioState->framesSinceB = 0xFF;
    gMarioState->invincTimer = 0;

    if (save_file_get_flags() &
            (SAVE_FLAG_CAP_ON_GROUND | SAVE_FLAG_CAP_ON_KLEPTO |
             SAVE_FLAG_CAP_ON_UKIKI | SAVE_FLAG_CAP_ON_MR_BLIZZARD))
        gMarioState->flags = 0;
    else
        gMarioState->flags = (MARIO_CAP_ON_HEAD | MARIO_UNKNOWN_00);

    gMarioState->forwardVel = 0.0f;
    gMarioState->squishTimer = 0;
    gMarioState->hurtCounter = 0;
    gMarioState->healCounter = 0;
    gMarioState->capTimer = 0;
    gMarioState->quicksandDepth = 0.0f;
    gMarioState->heldObj = NULL;
    gMarioState->riddenObj = NULL;
    gMarioState->usedObj = NULL;
    gMarioState->waterLevel = find_water_level(
        gMarioSpawnInfo->startPos[0], gMarioSpawnInfo->startPos[2]
    );
    gMarioState->area = gCurrentArea;
    gMarioState->marioObj = gMarioObject;
    gMarioState->marioObj->header.gfx.unk38.animID = -1;
    vec3s_copy(gMarioState->faceAngle, gMarioSpawnInfo->startAngle);
    vec3s_set(gMarioState->angleVel, 0, 0, 0);
    vec3s_to_vec3f(gMarioState->pos, gMarioSpawnInfo->startPos);
    vec3f_set(gMarioState->vel, 0, 0, 0);
    gMarioState->floorHeight = find_floor(
        gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2],
        &gMarioState->floor
    );

    if (gMarioState->pos[1] < gMarioState->floorHeight)
        gMarioState->pos[1] = gMarioState->floorHeight;

    gMarioState->marioObj->header.gfx.pos[1] = gMarioState->pos[1];

    gMarioState->action = (gMarioState->pos[1] <= (gMarioState->waterLevel - 100))
                        ? ACT_WATER_IDLE
                        : ACT_IDLE;

    func_8025410C(gMarioState);
    func_80254088(gMarioState);
    gMarioState->unk98->unk0B = 0;
    gMarioState->marioObj->oPosX = gMarioState->pos[0];
    gMarioState->marioObj->oPosY = gMarioState->pos[1];
    gMarioState->marioObj->oPosZ = gMarioState->pos[2];
    gMarioState->marioObj->oMoveAnglePitch = gMarioState->faceAngle[0];
    gMarioState->marioObj->oMoveAngleYaw = gMarioState->faceAngle[1];
    gMarioState->marioObj->oMoveAngleRoll = gMarioState->faceAngle[2];
    vec3f_copy(gMarioState->marioObj->header.gfx.pos, gMarioState->pos);
    vec3s_set(
        gMarioState->marioObj->header.gfx.angle,
        0, gMarioState->faceAngle[1], 0
    );

    if (save_file_get_cap_pos(capPos))
    {
        capObject = spawn_object(gMarioState->marioObj, MODEL_MARIOS_CAP, bhvNormalCap);
        capObject->oPosX = capPos[0];
        capObject->oPosY = capPos[1];
        capObject->oPosZ = capPos[2];
        capObject->oForwardVel2 = 0;
        capObject->oMoveAngleYaw = 0;
    }
}

void func_80254CE0(void)
{
    gMarioState->unk00 = 0;
    gMarioState->flags = 0;
    gMarioState->action = 0;
    gMarioState->spawnInfo = &gPlayerSpawnInfos[0];
    gMarioState->unk94 = &gPlayerStatusForCamera[0];
    gMarioState->unk98 = &D_8033A040[0];
    gMarioState->controller = &gControllers[0];
    gMarioState->animation = &D_80339D10;
    gMarioState->numCoins = 0;
    gMarioState->numStars = save_file_get_total_star_count(
        gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1
    );
    gMarioState->numKeys = 0;
    gMarioState->numLives = 4;
    gMarioState->health = 0x880;
    gMarioState->unkB8 = gMarioState->numStars;
    gMarioState->unkB0 = 0xBD;
    gDisplayedCoins = 0;
    gDisplayedHealthWedges = 8;
}

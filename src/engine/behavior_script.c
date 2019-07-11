#include <ultra64.h>

#include "sm64.h"
#include "behavior_script.h"
#include "game/memory.h"
#include "graph_node.h"
#include "surface_collision.h"
#include "game/object_helpers.h"
#include "game/object_helpers2.h"
#include "game/mario.h"
#include "game/display.h"
#include "game/obj_behaviors_2.h"
#include "behavior_data.h"
#include "game/object_list_processor.h"

static u16 gRandomSeed16;

// unused
static void func_80383B70(u32 segptr)
{
    gBehCommand = segmented_to_virtual((void *)segptr);
    gCurrentObject->stackIndex = 0;
}

u16 RandomU16(void)
{
    u16 temp1, temp2;

    if (gRandomSeed16 == 22026)
        gRandomSeed16 = 0;

    temp1 = (gRandomSeed16 & 0x00FF) << 8;
    temp1 = temp1 ^ gRandomSeed16;

    gRandomSeed16 = ((temp1 & 0x00FF) << 8) + ((temp1 & 0xFF00) >> 8);

    temp1 = ((temp1 & 0x00FF) << 1) ^ gRandomSeed16;
    temp2 = (temp1 >> 1) ^ 0xFF80;

    if ((temp1 & 1) == 0)
    {
        if (temp2 == 43605)
            gRandomSeed16 = 0;
        else
            gRandomSeed16 = temp2 ^ 0x1FF4;
    }
    else
    {
        gRandomSeed16 = temp2 ^ 0x8180;
    }

    return gRandomSeed16;
}

f32 RandomFloat(void)
{
    f32 rnd = RandomU16();
    return rnd / (double)0x10000;
}

s32 RandomSign(void)
{
    if (RandomU16() >= 0x7FFF)
        return 1;
    else
        return -1;
}

void func_80383D68(struct Object *object)
{
    object->header.gfx.pos[0] = object->oPosX;
    object->header.gfx.pos[1] = object->oPosY + object->oGraphYOffset;
    object->header.gfx.pos[2] = object->oPosZ;

    object->header.gfx.angle[0] = object->oFaceAnglePitch & 0xFFFF;
    object->header.gfx.angle[1] = object->oFaceAngleYaw & 0xFFFF;
    object->header.gfx.angle[2] = object->oFaceAngleRoll & 0xFFFF;
}

static void cur_object_stack_push(u32 value)
{
    gCurrentObject->stack[gCurrentObject->stackIndex] = value;
    gCurrentObject->stackIndex++;
}

static u32 cur_object_stack_pop(void)
{
    u32 value;
    gCurrentObject->stackIndex--;
    value = gCurrentObject->stack[gCurrentObject->stackIndex];
    return value;
}

static void Unknown80383E44(void) // ?
{
    for (;;);
}

static s32 beh_cmd_unhide(void)
{
    obj_hide();
    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_graph_clear(void)
{
    gCurrentObject->header.gfx.node.flags &= ~GRAPH_RENDER_01;
    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_billboard(void)
{
    gCurrentObject->header.gfx.node.flags |= GRAPH_RENDER_BILLBOARD;
    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_graph_node(void)
{
    s32 index = (s16)(gBehCommand[0] & 0xFFFF);
    gCurrentObject->header.gfx.asGraphNode = gLoadedGraphNodes[index];
    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_load_chill(void)
{
    u32 model = gBehCommand[1];
    void *arg1 = (void *) gBehCommand[2];
    
    struct Object *object = spawn_object_at_origin(gCurrentObject, 0, model, arg1);
    
    copy_object_pos_and_angle(object, gCurrentObject);
    
    gBehCommand += 3;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_spawn(void)
{
    u32 model = gBehCommand[1];
    void *arg1 = (void *) gBehCommand[2];
    
    struct Object *object = spawn_object_at_origin(gCurrentObject, 0, model, arg1);
    
    copy_object_pos_and_angle(object, gCurrentObject);
    
    gCurrentObject->prevObj = object;
    
    gBehCommand += 3;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_load_chill_param(void)
{
    u32 behParam = (s16)(gBehCommand[0] & 0xFFFF);
    u32 model = gBehCommand[1];
    void *arg2 = (void *) gBehCommand[2];
    
    struct Object *object = spawn_object_at_origin(gCurrentObject, 0, model, arg2);
    
    copy_object_pos_and_angle(object, gCurrentObject);
    
    object->oBehParams2ndByte = behParam;
    
    gBehCommand += 3;
    return BEH_CONTINUE;
}

static s32 beh_cmd_deactivate(void)
{
    gCurrentObject->activeFlags = 0;
    return BEH_BREAK;
}

static s32 beh_cmd_break(void)
{
    return BEH_BREAK;
}

// unused
static s32 beh_cmd_break2(void)
{
    return BEH_BREAK;
}

static s32 beh_cmd_call(void)
{
    u32* jumpAddress;

    gBehCommand++;
    cur_object_stack_push((u32)(gBehCommand + 1));
    jumpAddress = (u32 *)segmented_to_virtual((void *)gBehCommand[0]);
    gBehCommand = jumpAddress;

    return BEH_CONTINUE;
}

static s32 beh_cmd_return(void)
{
    gBehCommand = (u32 *)cur_object_stack_pop();
    return BEH_CONTINUE;
}

static s32 beh_cmd_delay(void)
{
    s16 arg0 = gBehCommand[0] & 0xFFFF;

    if (gCurrentObject->unk1F4 < arg0 - 1)
    {
        gCurrentObject->unk1F4++;
    }
    else
    {
        gCurrentObject->unk1F4 = 0;
        gBehCommand++;
    }

    return BEH_BREAK;
}

static s32 beh_cmd_delay_var(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 arg0 = cur_object_get_int(objectOffset);

    if (gCurrentObject->unk1F4 < (arg0 - 1))
    {
        gCurrentObject->unk1F4++;
    }
    else
    {
        gCurrentObject->unk1F4 = 0;
        gBehCommand++;
    }

    return BEH_BREAK;
}

static s32 beh_cmd_goto(void)
{
    gBehCommand++;
    gBehCommand = (u32 *)segmented_to_virtual((void *)gBehCommand[0]);
    return BEH_CONTINUE;
}

// unused
static s32 Behavior26(void)
{
    s32 value = (u8)(gBehCommand[0] >> 16) & 0xFF;
    
    cur_object_stack_push((u32)(gBehCommand + 1));
    cur_object_stack_push(value);
    
    gBehCommand++;
    
    return BEH_CONTINUE;
}

static s32 beh_cmd_begin_repeat(void)
{
    s32 count = (s16)(gBehCommand[0] & 0xFFFF);
    
    cur_object_stack_push((u32)(gBehCommand + 1));
    cur_object_stack_push(count);
    
    gBehCommand++;
    
    return BEH_CONTINUE;
}

static s32 beh_cmd_end_repeat(void)
{
    u32 count = cur_object_stack_pop();

    count--;
    if (count != 0)
    {
        gBehCommand = (u32 *)cur_object_stack_pop();
        cur_object_stack_push((u32)gBehCommand);
        cur_object_stack_push(count);
    }
    else
    {
        cur_object_stack_pop();
        gBehCommand++;
    }

    return BEH_BREAK;
}

static s32 beh_cmd_end_repeat_nobreak(void)
{
    u32 count = cur_object_stack_pop();

    count--;
    if (count != 0)
    {
        gBehCommand = (u32 *)cur_object_stack_pop();
        cur_object_stack_push((u32)gBehCommand);
        cur_object_stack_push(count);
    }
    else
    {
        cur_object_stack_pop();
        gBehCommand++;
    }

    return BEH_CONTINUE;
}

static s32 beh_cmd_begin_loop(void)
{
    cur_object_stack_push((u32)(gBehCommand + 1));

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_end_loop(void)
{
    gBehCommand = (u32 *)cur_object_stack_pop();
    cur_object_stack_push((u32)gBehCommand);

    return BEH_BREAK;
}

typedef void (*BehaviorCallProc)(void);

static s32 beh_cmd_callnative(void)
{
    BehaviorCallProc behavior_proc = (BehaviorCallProc)gBehCommand[1];
    
    behavior_proc();
    
    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_float(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    f32 value = (s16)(gBehCommand[0] & 0xFFFF);

    cur_object_set_float(objectOffset, value);

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_int(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s16 value = gBehCommand[0] & 0xFFFF;

    cur_object_set_int(objectOffset, value);

    gBehCommand++;
    return BEH_CONTINUE;
}

// unused
static s32 Behavior36(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    u32 value = (s16)(gBehCommand[1] & 0xFFFF);

    cur_object_set_int(objectOffset, value);

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_float_rand(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    f32 min = (s16)(gBehCommand[0] & 0xFFFF);
    f32 max = (s16)(gBehCommand[1] >> 16);

    cur_object_set_float(objectOffset, (max * RandomFloat()) + min);

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_int_rand(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 min = (s16)(gBehCommand[0] & 0xFFFF);
    s32 max = (s16)(gBehCommand[1] >> 16);

    cur_object_set_int(objectOffset, (s32)(max * RandomFloat()) + min);

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_int_rand_rshift(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 min = (s16)(gBehCommand[0] & 0xFFFF);
    s32 rshift = (s16)(gBehCommand[1] >> 16);

    cur_object_set_int(objectOffset, (RandomU16() >> rshift) + min);

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_add_float_rand(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    f32 min = (s16)(gBehCommand[0] & 0xFFFF);
    f32 max = (s16)(gBehCommand[1] >> 16);

    cur_object_set_float(objectOffset,
        (cur_object_get_float(objectOffset) + min) + (max * RandomFloat()));

    gBehCommand += 2;
    return BEH_CONTINUE;
}

// unused
static s32 beh_cmd_obj_add_int_rand_rshift(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 min = (s16)(gBehCommand[0] & 0xFFFF);
    s32 rshift = (s16)(gBehCommand[1] >> 16);
    s32 rnd = RandomU16();

    cur_object_set_int(objectOffset,
        (cur_object_get_int(objectOffset) + min) + (rnd >> rshift));

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_add_float(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    f32 value = (s16)(gBehCommand[0] & 0xFFFF);
    
    cur_object_add_float(objectOffset, value);
    
    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_add_int(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s16 value = gBehCommand[0] & 0xFFFF;

    cur_object_add_int(objectOffset, value);

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_or_int(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 value = (s16)(gBehCommand[0] & 0xFFFF);

    value &= 0xFFFF;
    cur_object_or_int(objectOffset, value);
    
    gBehCommand++;
    return BEH_CONTINUE;
}

// unused
static s32 beh_cmd_obj_bit_clear_int(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 value = (s16)(gBehCommand[0] & 0xFFFF);

    value = (value & 0xFFFF) ^ 0xFFFF;
    cur_object_and_int(objectOffset, value);

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_int32(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;

    cur_object_set_int(objectOffset, gBehCommand[1]);

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_animate(void)
{    
    s32 animIndex = (u8)((gBehCommand[0] >> 16) & 0xFF);
    u32* animations = gCurrentObject->oAnimations;

    func_8037C658((struct GraphNodeObject *) gCurrentObject, &animations[animIndex]);

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_drop_floor(void)
{
    f32 x = gCurrentObject->oPosX;
    f32 y = gCurrentObject->oPosY;
    f32 z = gCurrentObject->oPosZ;
    f32 floor = find_floor_height(x, y + 200.0f, z);

    gCurrentObject->oPosY = floor;
    gCurrentObject->oMoveFlags |= OBJ_MOVE_ON_GROUND;

    gBehCommand++;
    return BEH_CONTINUE;
}

// unused
static s32 Behavior18(void)
{
    /* no operation */
    UNUSED u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;

    gBehCommand++;
    return BEH_CONTINUE;
}

// unused
static s32 Behavior1A(void)
{
    /* no operation */
    UNUSED u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;

    gBehCommand++;
    return BEH_CONTINUE;
}

// unused
static s32 Behavior19(void)
{
    /* no operation */
    UNUSED u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_sum_float(void)
{
    u32 objectOffsetDst = (u8)((gBehCommand[0] >> 16) & 0xFF);
    u32 objectOffsetSrc1 = (u8)((gBehCommand[0] >> 8) & 0xFF);
    u32 objectOffsetSrc2 = (u8)((gBehCommand[0]) & 0xFF);

    cur_object_set_float(objectOffsetDst,
        cur_object_get_float(objectOffsetSrc1) + cur_object_get_float(objectOffsetSrc2));

    gBehCommand++;
    return BEH_CONTINUE;
}

// unused
static s32 beh_cmd_obj_sum_int(void)
{
    u32 objectOffsetDst = (u8)((gBehCommand[0] >> 16) & 0xFF);
    u32 objectOffsetSrc1 = (u8)((gBehCommand[0] >> 8) & 0xFF);
    u32 objectOffsetSrc2 = (u8)((gBehCommand[0]) & 0xFF);

    cur_object_set_int(objectOffsetDst,
        cur_object_get_int(objectOffsetSrc1) + cur_object_get_int(objectOffsetSrc2));

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_set_hitbox(void)
{
    s16 colSphereX = gBehCommand[1] >> 16;
    s16 colSphereY = gBehCommand[1] & 0xFFFF;

    gCurrentObject->hitboxRadius = colSphereX;
    gCurrentObject->hitboxHeight = colSphereY;

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_float2(void)
{
    s16 arg0 = gBehCommand[1] >> 16;
    s16 arg1 = gBehCommand[1] & 0xFFFF;

    gCurrentObject->hurtboxRadius = arg0;
    gCurrentObject->hurtboxHeight = arg1;

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_collision_sphere(void)
{
    s16 colSphereX = gBehCommand[1] >> 16;
    s16 colSphereY = gBehCommand[1] & 0xFFFF;
    s16 unknown = gBehCommand[2] >> 16;

    gCurrentObject->hitboxRadius = colSphereX;
    gCurrentObject->hitboxHeight = colSphereY;
    gCurrentObject->hitboxDownOffset = unknown;

    gBehCommand += 3;
    return BEH_CONTINUE;
}

// unused
static s32 Behavior24(void)
{
    /* no operation */
    UNUSED s16 arg0 = (u8)((gBehCommand[0] >> 16) & 0xFF);
    UNUSED s16 arg1 = gBehCommand[0] & 0xFFFF;

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_begin(void)
{
    if (obj_has_behavior(bhvHauntedChair))
        bhv_init_room();
    if (obj_has_behavior(bhvMadPiano))
        bhv_init_room();
    if (obj_has_behavior(bhvMessagePanel))
        gCurrentObject->oCollisionDistance = 150.0f;
    gBehCommand++;
    return BEH_CONTINUE;
}

static void Unknown8038556C(s32 lastIndex)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    u32 table[16];
    s32 i;

    for (i = 0; i <= lastIndex / 2; i += 2)
    {
        table[i] = (s16)(gBehCommand[i + 1] >> 16);
        table[i + 1] = (s16)(gBehCommand[i + 1] & 0xFFFF);
    }

    cur_object_set_int(objectOffset, table[(s32)(lastIndex * RandomFloat())]);
}

static s32 beh_cmd_collision_data(void)
{
    u32* collisionData = segmented_to_virtual((void *)gBehCommand[1]);
    gCurrentObject->collisionData = collisionData;
    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_pos(void)
{
    gCurrentObject->oHomeX = gCurrentObject->oPosX;
    gCurrentObject->oHomeY = gCurrentObject->oPosY;
    gCurrentObject->oHomeZ = gCurrentObject->oPosZ;
    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_interact_type(void)
{
    gCurrentObject->oInteractType = gBehCommand[1];

    gBehCommand += 2;
    return BEH_CONTINUE;
}

// unused
static s32 Behavior31(void)
{
    gCurrentObject->oUnk190 = gBehCommand[1];

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_scale(void)
{
    UNUSED u8 sp1f = (gBehCommand[0] >> 16) & 0xFF;
    s16 sp1c = gBehCommand[0] & 0xFFFF;

    obj_scale((f32)sp1c / 100.0f);

    gBehCommand++;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_set_gravity(void)
{
    UNUSED f32 sp04, sp00;

    gCurrentObject->oWallHitboxRadius = (f32)(s16)(gBehCommand[1] >> 16);
    gCurrentObject->oGravity = (f32)(s16)(gBehCommand[1] & 0xFFFF) / 100.0f;
    gCurrentObject->oBounce = (f32)(s16)(gBehCommand[2] >> 16) / 100.0f;
    gCurrentObject->oDragStrength = (f32)(s16)(gBehCommand[2] & 0xFFFF) / 100.0f;
    gCurrentObject->oFriction = (f32)(s16)(gBehCommand[3] >> 16) / 100.0f;
    gCurrentObject->oBuoyancy = (f32)(s16)(gBehCommand[3] & 0xFFFF) / 100.0f;

    // unused parameters
    sp04 = (f32)(s16)(gBehCommand[4] >> 16) / 100.0f;
    sp00 = (f32)(s16)(gBehCommand[4] & 0xFFFF) / 100.0f;

    gBehCommand += 5;
    return BEH_CONTINUE;
}

static s32 beh_cmd_obj_bit_clear_int32(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s32 flags = gBehCommand[1];

    flags = flags ^ 0xFFFFFFFF;

    object_and_int(gCurrentObject->parentObj, objectOffset, flags);

    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_spawn_addr(void)
{
    struct WaterSplashParams *arg0 = (struct WaterSplashParams *) gBehCommand[1];
    spawn_water_splash(gCurrentObject, arg0);
    gBehCommand += 2;
    return BEH_CONTINUE;
}

static s32 beh_cmd_text_anim_rate(void)
{
    u8 objectOffset = (gBehCommand[0] >> 16) & 0xFF;
    s16 arg1 = (gBehCommand[0] & 0xFFFF);

    if ((gGlobalTimer % arg1) == 0)
        cur_object_add_int(objectOffset, 1);

    gBehCommand++;
    return BEH_CONTINUE;
}

void stub_80385BF0(void)
{
    // (empty function)
}

typedef s32 (*BehCommandProc)(void);
static BehCommandProc BehaviorJumpTable[] =
{
    beh_cmd_begin,
    beh_cmd_delay,
    beh_cmd_call,
    beh_cmd_return,
    beh_cmd_goto,
    beh_cmd_begin_repeat,
    beh_cmd_end_repeat,
    beh_cmd_end_repeat_nobreak,
    beh_cmd_begin_loop,
    beh_cmd_end_loop,
    beh_cmd_break,
    beh_cmd_break2,
    beh_cmd_callnative,
    beh_cmd_obj_add_float,
    beh_cmd_obj_set_float,
    beh_cmd_obj_add_int,
    beh_cmd_obj_set_int,
    beh_cmd_obj_or_int,
    beh_cmd_obj_bit_clear_int,
    beh_cmd_obj_set_int_rand_rshift,
    beh_cmd_obj_set_float_rand,
    beh_cmd_obj_set_int_rand,
    beh_cmd_obj_add_float_rand,
    beh_cmd_obj_add_int_rand_rshift,
    Behavior18,
    Behavior19,
    Behavior1A,
    beh_cmd_graph_node,
    beh_cmd_obj_load_chill,
    beh_cmd_deactivate,
    beh_cmd_obj_drop_floor,
    beh_cmd_obj_sum_float,
    beh_cmd_obj_sum_int,
    beh_cmd_billboard,
    beh_cmd_unhide,
    beh_cmd_set_hitbox,
    Behavior24,
    beh_cmd_delay_var,
    Behavior26,
    beh_cmd_obj_set_int32,
    beh_cmd_obj_animate,
    beh_cmd_obj_load_chill_param,
    beh_cmd_collision_data,
    beh_cmd_collision_sphere,
    beh_cmd_obj_spawn,
    beh_cmd_obj_set_pos,
    beh_cmd_obj_set_float2,
    beh_cmd_interact_type,
    beh_cmd_obj_set_gravity,
    Behavior31,
    beh_cmd_scale,
    beh_cmd_obj_bit_clear_int32,
    beh_cmd_text_anim_rate,
    beh_cmd_graph_clear,
    Behavior36,
    beh_cmd_spawn_addr,
};

void cur_object_exec_behavior(void)
{
    UNUSED u32 unused;

    s16 flagsLo = gCurrentObject->oFlags;
    f32 distanceFromMario;
    BehCommandProc behCmdFunc;
    s32 behProcResult;

    if (flagsLo & OBJ_FLAG_COMPUTE_DIST_TO_MARIO)
    {
        gCurrentObject->oDistanceToMario = dist_between_objects(gCurrentObject, gMarioObject);
        distanceFromMario = gCurrentObject->oDistanceToMario;
    }
    else
    {
        distanceFromMario = 0.0f;
    }

    if (flagsLo & OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO)
        gCurrentObject->oAngleToMario = angle_to_object(gCurrentObject, gMarioObject);

    if (gCurrentObject->oAction != gCurrentObject->oPrevAction)
    {
        (void) (gCurrentObject->oTimer = 0,
        gCurrentObject->oSubAction = 0,
        gCurrentObject->oPrevAction = gCurrentObject->oAction);
    }

    gBehCommand = gCurrentObject->behScript;

    do
    {
        behCmdFunc = BehaviorJumpTable[*gBehCommand >> 24];
        behProcResult = behCmdFunc();
    } while (behProcResult == BEH_CONTINUE);

    gCurrentObject->behScript = gBehCommand;

    if (gCurrentObject->oTimer < 0x3FFFFFFF)
        gCurrentObject->oTimer++;

    if (gCurrentObject->oAction != gCurrentObject->oPrevAction) 
    {
        (void) (gCurrentObject->oTimer = 0,
        gCurrentObject->oSubAction = 0,
        gCurrentObject->oPrevAction = gCurrentObject->oAction);
    }

    flagsLo = (s16)gCurrentObject->oFlags;

    if (flagsLo & OBJ_FLAG_0010)
        obj_set_facing_to_move_angles(gCurrentObject);

    if (flagsLo & OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW)
        gCurrentObject->oFaceAngleYaw = gCurrentObject->oMoveAngleYaw;

    if (flagsLo & OBJ_FLAG_MOVE_XZ_USING_FVEL)
        obj_move_xz_using_fvel();

    if (flagsLo & OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL)
        obj_move_y_with_terminal_vel();

    if (flagsLo & OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT)
        build_object_transform_relative_to_parent(gCurrentObject);

    if (flagsLo & OBJ_FLAG_0800)
        func_802A2270(gCurrentObject);

    if (flagsLo & OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)
        func_80383D68(gCurrentObject);

    if (gCurrentObject->oRoom != -1)
    {
        obj_enable_rendering_if_mario_in_room();
    }
    else if ((flagsLo & OBJ_FLAG_COMPUTE_DIST_TO_MARIO) &&
        gCurrentObject->collisionData == NULL)
    {
        if (!(flagsLo & OBJ_FLAG_ACTIVE_FROM_AFAR))
        {
            if (distanceFromMario > gCurrentObject->oDrawingDistance)
            {
                gCurrentObject->header.gfx.node.flags &= ~GRAPH_RENDER_01;
                gCurrentObject->activeFlags |= ACTIVE_FLAG_FAR_AWAY;
            }
            else if (gCurrentObject->oHeldState == HELD_FREE)
            {
                gCurrentObject->header.gfx.node.flags |= GRAPH_RENDER_01;
                gCurrentObject->activeFlags &= ~ACTIVE_FLAG_FAR_AWAY;
            }
        }
    }
}

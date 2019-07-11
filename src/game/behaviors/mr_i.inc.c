// mr_i.c.inc

// this is actually the MrI particle loop function. piranha
// plant code later on reuses this function.
void bhv_piranha_particle_loop(void)
{
    if(o->oTimer == 0)
    {
        o->oVelY = 20.0f + 20.0f * RandomFloat();
        o->oForwardVel = 20.0f + 20.0f * RandomFloat();
        o->oMoveAngleYaw = RandomU16();
    }
    obj_move_using_fvel_and_gravity();
}

void ActionMrIParticle0(void)
{
    obj_scale(3.0f);
    o->oForwardVel = 20.0f;
    obj_update_floor_and_walls();
    if(0x8000 & o->oInteractStatus)
        o->oAction = 1;
    else if((o->oTimer >= 101) || (0x200 & o->oMoveFlags) || (8 & (s16)o->activeFlags))
    {
        mark_object_for_deletion(o);
        func_802A3004();
    }
}

void ActionMrIParticle1(void)
{
    s32 i;
    mark_object_for_deletion(o);
    for(i = 0;i<10;i++)
        spawn_object(o,MODEL_PURPLE_MARBLE,bhvGiantPiranhaParticle);
}

void (*TableMrIParticleActions[])(void) = {ActionMrIParticle0,ActionMrIParticle1};

void bhv_mr_i_particle_loop(void)
{
    obj_call_action_function(TableMrIParticleActions);
}

void func_802A525C(void)
{
    struct Object* particle;
    f32 sp18 = o->header.gfx.scale[1];
    particle = spawn_object(o,MODEL_PURPLE_MARBLE,bhvMrIParticle);
    particle->oPosY += 50.0f * sp18;
    particle->oPosX += sins(o->oMoveAngleYaw) * 90.0f * sp18;
    particle->oPosZ += coss(o->oMoveAngleYaw) * 90.0f * sp18;
    PlaySound2(SOUND_OBJECT_MRISHOOT);
}

void bhv_mr_i_body_loop(void)
{
    copy_object_pos_and_angle(o,o->parentObj);
    if(!(8 & o->activeFlags))
    {
        copy_object_scale(o,o->parentObj);
        set_object_parent_relative_pos(o,0,0,o->header.gfx.scale[1] * 100.0f);
        build_object_transform_from_pos_and_angle(o,44,15);
        translate_object_local(o,6,44);
        o->oFaceAnglePitch = o->oMoveAnglePitch;
        o->oGraphYOffset = o->header.gfx.scale[1] * 100.f;
    }
    if(o->parentObj->OBJECT_FIELD_S32(0x22) != 1)
        o->OBJECT_FIELD_S32(0x1a) = -1;
    else
    {
        o->OBJECT_FIELD_S32(0x1a)++;
        if(o->OBJECT_FIELD_S32(0x1a) == 15)
            o->parentObj->OBJECT_FIELD_S32(0x22) = 0;
    }
    if(!o->parentObj->activeFlags)
        mark_object_for_deletion(o);
}

void ActionMrI3(void)
{
    s16 sp36;
    s16 sp34;
    f32 sp30;
    f32 sp2C;
    UNUSED u8 pad[8];
    f32 sp20;
    f32 sp1C;
    if(o->oBehParams2ndByte)
        sp1C = 2.0f;
    else
        sp1C = 1.0f;
    if(o->OBJECT_FIELD_S32(0x1E) < 0)
        sp34 = 0x1000;
    else
        sp34 = -0x1000;
    sp2C = (o->oTimer + 1)/96.0f;
    if(o->oTimer < 64)
    {
        sp36 = o->oMoveAngleYaw;
        o->oMoveAngleYaw += sp34 * coss(0x4000 * sp2C);
        if(sp36 < 0 && o->oMoveAngleYaw >= 0)
            PlaySound2(SOUND_CH9_UNK6B);
        o->oMoveAnglePitch = (1.0 - coss(0x4000 * sp2C)) * -0x4000;
        obj_shake_y(4.0f);
    }
    else if(o->oTimer < 96)
    {
        if(o->oTimer == 64)
            PlaySound2(SOUND_OBJECT_MRIDEATH);
        sp30 = (f32)(o->oTimer - 63)/32;
        o->oMoveAngleYaw += sp34 * coss(0x4000 * sp2C);
        o->oMoveAnglePitch = (1.0 - coss(0x4000 * sp2C)) * - 0x4000;
        obj_shake_y((s32)((1.0f - sp30) * 4)); // trucating the f32?
        sp20 = coss(0x4000 * sp30) * 0.4 + 0.6;
        obj_scale(sp20*sp1C);
    }
    else if(o->oTimer < 104) {} // do nothing
    else if(o->oTimer < 168)
    {
        if(o->oTimer == 104)
        {
            obj_become_intangible();
            func_802A3004();
            o->oMrISize = sp1C * 0.6;
            if(o->oBehParams2ndByte)
            {
                o->oPosY += 100.0f;
                CreateStar(1370,2000.0f,-320.0f);
                mark_object_for_deletion(o);
            }
            else obj_spawn_loot_blue_coin();
        }
        o->oMrISize -= 0.2 * sp1C;
        if(o->oMrISize < 0)
            o->oMrISize = 0;
        obj_scale(o->oMrISize);
    }
    else mark_object_for_deletion(o);
}

void ActionMrI2(){
    s16 sp1E;
    s16 sp1C;
    sp1E = o->oMoveAngleYaw;
    if(o->oTimer == 0){
        if(o->oBehParams2ndByte)
            o->OBJECT_FIELD_S32(0x1B) = 200;
        else
            o->OBJECT_FIELD_S32(0x1B) = 120;
        o->OBJECT_FIELD_S32(0x1D) = 0;
        o->OBJECT_FIELD_S32(0x1E) = 0;
        o->OBJECT_FIELD_S32(0x1F) = 0;
    }
    obj_turn_toward_object(o,gMarioObject,0x10,0x800);
    obj_turn_toward_object(o,gMarioObject,0x0F,0x400);
    sp1C = sp1E - (s16)(o->oMoveAngleYaw);
    if(!sp1C)
    {
        o->OBJECT_FIELD_S32(0x1D) = 0;
        o->OBJECT_FIELD_S32(0x1E) = 0;
    }
    else if(sp1C > 0)
    {
        if(o->OBJECT_FIELD_S32(0x1E) > 0)
            o->OBJECT_FIELD_S32(0x1D) += sp1C;
        else
            o->OBJECT_FIELD_S32(0x1D) = 0;
        o->OBJECT_FIELD_S32(0x1E) = 1;
    }
    else
    {
        if(o->OBJECT_FIELD_S32(0x1E) < 0)
            o->OBJECT_FIELD_S32(0x1D) -= sp1C;
        else
            o->OBJECT_FIELD_S32(0x1D) = 0;
        o->OBJECT_FIELD_S32(0x1E) = -1;
    }
    if(!o->OBJECT_FIELD_S32(0x1D))
        o->OBJECT_FIELD_S32(0x1B) = 120;
    if(o->OBJECT_FIELD_S32(0x1D) > 1<<16)
        o->oAction = 3;
    o->OBJECT_FIELD_S32(0x1B) -= 1;
    if(!o->OBJECT_FIELD_S32(0x1B))
    {
        o->OBJECT_FIELD_S32(0x1B) = 120;
        o->OBJECT_FIELD_S32(0x1D) = 0;
    }
    if(o->OBJECT_FIELD_S32(0x1D) < 5000)
    {
        if(o->OBJECT_FIELD_S32(0x1F) == o->OBJECT_FIELD_S32(0x20))
            o->OBJECT_FIELD_S32(0x22) = 1;
        if(o->OBJECT_FIELD_S32(0x1F) == o->OBJECT_FIELD_S32(0x20) + 20)
        {
            func_802A525C();
            o->OBJECT_FIELD_S32(0x1F) = 0;
            o->OBJECT_FIELD_S32(0x20) = (s32)(RandomFloat() * 50.0f + 50.0f);
        }
        o->OBJECT_FIELD_S32(0x1F)++;
    }
    else
    {
        o->OBJECT_FIELD_S32(0x1F) = 0;
        o->OBJECT_FIELD_S32(0x20) = (s32)(RandomFloat() * 50.0f + 50.0f);
    }
    if(o->oDistanceToMario > 800.0f)
        o->oAction = 1;
}

void ActionMrI1(void)
{
    s16 sp1E;
    s16 sp1C;
    s16 sp1A;
    sp1E = angle_to_object(o,gMarioObject);
    sp1C = abs_angle_diff(o->oMoveAngleYaw,sp1E);
    sp1A = abs_angle_diff(o->oMoveAngleYaw,gMarioObject->oFaceAngleYaw);
    if(o->oTimer == 0)
    {
        obj_become_tangible();
        o->oMoveAnglePitch = 0;
        o->OBJECT_FIELD_S32(0x1F) = 30;
        o->OBJECT_FIELD_S32(0x20) = RandomFloat() * 20.0f;
        if(o->OBJECT_FIELD_S32(0x20) & 1)
            o->oAngleVelYaw = -256;
        else
            o->oAngleVelYaw = 256;
    }
    if(sp1C < 1024 && sp1A > 0x4000)
    {
        if(o->oDistanceToMario < 700.0f)
            o->oAction = 2;
        else
            o->OBJECT_FIELD_S32(0x1F)++;
    }
    else
    {
        o->oMoveAngleYaw += o->oAngleVelYaw;
        o->OBJECT_FIELD_S32(0x1F) = 30;
    }
    if(o->OBJECT_FIELD_S32(0x1F) == o->OBJECT_FIELD_S32(0x20) + 60)
        o->OBJECT_FIELD_S32(0x22) = 1;
    if(o->OBJECT_FIELD_S32(0x20) + 80 < o->OBJECT_FIELD_S32(0x1F))
    {
        o->OBJECT_FIELD_S32(0x1F) = 0;
        o->OBJECT_FIELD_S32(0x20) = RandomFloat() * 80.0f;
        func_802A525C();
    }
}

void ActionMrI0(void)
{
#ifndef VERSION_JP
    set_object_angle(o, 0, 0, 0);
#else
    o->oMoveAnglePitch = 0;
    o->oMoveAngleYaw     = 0;
    o->oMoveAngleRoll    = 0;
#endif
    obj_scale(o->oBehParams2ndByte + 1);
    if(o->oTimer == 0)
        obj_set_pos_to_home();
    if(o->oDistanceToMario < 1500.0f)
        o->oAction = 1;
}

void (*TableMrIActions[])(void) = {ActionMrI0,ActionMrI1,ActionMrI2,ActionMrI3};

struct ObjectHitbox sMrIHitbox =
{
    /* interactType: */      INTERACT_DAMAGE,
    /* downOffset: */        0,
    /* damageOrCoinValue: */ 2,
    /* health: */            2,
    /* numLootCoins: */      5,
    /* radius: */            80,
    /* height: */            150,
    /* hurtboxRadius: */     0,
    /* hurtboxHeight: */     0,
};

void bhv_mr_i_loop(void)
{
    set_object_hitbox(o,&sMrIHitbox);
    obj_call_action_function(TableMrIActions);
    if(o->oAction != 3)
        if(o->oDistanceToMario > 3000.0f || o->activeFlags & 8)
            o->oAction = 0;
    o->oInteractStatus = 0;
}

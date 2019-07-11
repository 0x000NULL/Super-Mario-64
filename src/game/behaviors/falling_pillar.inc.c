// falling_pillar.c.inc


static struct ObjectHitbox sFallingPillarHitbox = 
{
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 150,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 300,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_falling_pillar_init(void) {
    o->oGravity = 0.5f;
    o->oFriction = 0.91f;
    o->oBuoyancy = 1.3f;
}

void func_802F6308(void) {
    s32 sp2C;

    for (sp2C = 0; sp2C < 4; sp2C++) {
        spawn_object_relative(sp2C, 0, sp2C * 400 + 300, 0, o, MODEL_NONE, &bhvSomeSubojbjectOfFallingPillar);
    }
}

s16 func_802F6388(void) {
    f32 sp1C;
    f32 sp18;

    sp1C = sins(gMarioObject->header.gfx.angle[1]) * 500.0f + gMarioObject->header.gfx.pos[0];
    sp18 = coss(gMarioObject->header.gfx.angle[1]) * 500.0f + gMarioObject->header.gfx.pos[2];

    return atan2s(sp18 - o->oPosZ, sp1C - o->oPosX);
}

void bhv_falling_pillar_loop(void) {
    s16 sp26;
    switch(o->oAction) {
        case 0:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1300)) {
                o->oMoveAngleYaw = o->oAngleToMario;
                o->oForwardVel = 1.0f;
                func_802F6308();
                o->oAction = 1;
                PlaySound2(SOUND_GENERAL_POUNDROCK);
            }
            break;

        case 1:
            func_802E4204();
            sp26 = func_802F6388();
            o->oFaceAngleYaw = approach_s16_symmetric(o->oFaceAngleYaw, sp26, 0x400);
            if (o->oTimer > 10) 
                o->oAction = 2;
            break;

        case 2:
            func_802E4204();
            o->oFallingPillarUnkF4 += 4.0f;
            o->oAngleVelPitch += o->oFallingPillarUnkF4;
            o->oFaceAnglePitch += o->oAngleVelPitch;
            if (o->oFaceAnglePitch > 0x3900) {
                o->oPosX += sins(o->oFaceAngleYaw) * 500.0f;
                o->oPosZ += coss(o->oFaceAngleYaw) * 500.0f;
                func_8027F440(2, o->oPosX, o->oPosY, o->oPosZ);
                func_802AA618(0, 0, 92.0f);
                o->activeFlags = 0;
                create_sound_spawner(SOUND_GENERAL_BIGPOUND);
            }
            break;
    }
}

void bhv_some_subobject_falling_pillar_loop(void) {
    s32 sp2C = o->parentObj->oFaceAnglePitch;
    s32 sp28 = o->parentObj->oFaceAngleYaw;
    f32 sp24 = o->parentObj->oPosX;
    f32 sp20 = o->parentObj->oPosY;
    f32 sp1C = o->parentObj->oPosZ;
    f32 sp18 = o->oBehParams2ndByte * 400 + 300;

    o->oPosX = sins(sp2C) * sins(sp28) * sp18 + sp24;
    o->oPosY = coss(sp2C) * sp18 + sp20;
    o->oPosZ = sins(sp2C) * coss(sp28) * sp18 + sp1C;

    set_object_hitbox(o, &sFallingPillarHitbox);
    if (o->parentObj->activeFlags == 0)
        o->activeFlags = 0;
}

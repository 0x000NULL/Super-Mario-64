// spawn_star.c.inc

static struct ObjectHitbox sCollectStarHitbox =
{
    /* interactType:      */ INTERACT_STAR_OR_KEY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_collect_star_init(void) {
    s8 sp1F;
    u8 sp1E;

    sp1F = (o->oBehParams >> 24) & 0xFF;
    sp1E = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    if (sp1E & (1 << sp1F)) {
        o->header.gfx.asGraphNode = gLoadedGraphNodes[MODEL_TRANSPARENT_STAR];
    } 
    else {
        o->header.gfx.asGraphNode = gLoadedGraphNodes[MODEL_STAR];
    }

    set_object_hitbox(o, &sCollectStarHitbox);
}

void bhv_collect_star_loop(void) {
    o->oFaceAngleYaw += 0x800;
    
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        mark_obj_for_deletion(o);
        o->oInteractStatus = 0;
    }

}

void bhv_star_spawn_init(void) {
    o->oMoveAngleYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
    o->oStarSpawnDisFromHome = sqrtf(sqr(o->oHomeX - o->oPosX) + sqr(o->oHomeZ - o->oPosZ));
    o->oVelY = (o->oHomeY - o->oPosY) / 30.0f;
    o->oForwardVel = o->oStarSpawnDisFromHome / 30.0f;
    o->oStarSpawnUnkFC = o->oPosY;
    if (o->oBehParams2ndByte == 0 || gCurrCourseNum == 5)
        func_8028F9E8(173, o);
    else 
        func_8028F9E8(176, o);

    set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
    o->activeFlags |= 0x20;
    obj_become_intangible();
}

void bhv_star_spawn_loop(void) {
    switch (o->oAction) {
        case 0:
            o->oFaceAngleYaw += 0x1000;
            if (o->oTimer > 20)
                o->oAction = 1;
            break;

        case 1:
            func_802E4250(o);
            o->oStarSpawnUnkFC += o->oVelY;
            o->oPosY = o->oStarSpawnUnkFC + sins((o->oTimer * 0x8000) / 30) * 400.0f;
            o->oFaceAngleYaw += 0x1000;
            spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);
            PlaySound(SOUND_ENVIRONMENT_STAR);
            if (o->oTimer == 30) {
                o->oAction = 2;
                o->oForwardVel = 0;
                play_power_star_jingle(1);
            }
            break;

        case 2:
            if (o->oTimer < 20)
                o->oVelY = 20 - o->oTimer;
            else    
                o->oVelY = -10.0f; 

            spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);
            func_802E4250(o);
            o->oFaceAngleYaw = o->oFaceAngleYaw - o->oTimer * 0x10 + 0x1000;
            PlaySound(SOUND_ENVIRONMENT_STAR);

            if (o->oPosY < o->oHomeY) {
                PlaySound2(SOUND_GENERAL_STARAPPEARS);
                obj_become_tangible();
                o->oPosY = o->oHomeY;
                o->oAction = 3;
            }
            break;

        case 3:
            o->oFaceAngleYaw += 0x800;
            if (o->oTimer == 20) {
                gCutsceneActive = 1;
                clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                o->activeFlags &= ~0x20;
            }

            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                mark_obj_for_deletion(o);
                o->oInteractStatus = 0;
            }
            break;
    }
}

struct Object *func_802F1A50(struct Object *sp30, f32 sp34, f32 sp38, f32 sp3C) {
    sp30 = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStarSpawnCoordinates, o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
    sp30->oBehParams = o->oBehParams;
    sp30->oHomeX = sp34;
    sp30->oHomeY = sp38;
    sp30->oHomeZ = sp3C;
    sp30->oFaceAnglePitch = 0;
    sp30->oFaceAngleRoll = 0;
    return sp30;
}

void CreateStar(f32 sp20, f32 sp24, f32 sp28) {
    struct Object *sp1C;
    sp1C = func_802F1A50(sp1C, sp20, sp24, sp28);
    sp1C->oBehParams2ndByte = 0;
}

void func_802F1B84(f32 sp20, f32 sp24, f32 sp28) {
    struct Object *sp1C;
    sp1C = func_802F1A50(sp1C, sp20, sp24, sp28);
    sp1C->oBehParams2ndByte = 1;
}

void func_802F1BD4(f32 sp20, f32 sp24, f32 sp28) {
    struct Object *sp1C;
    sp1C = func_802F1A50(sp1C, sp20, sp24, sp28);
    sp1C->oBehParams2ndByte = 1;
    sp1C->oUnk190 |= 0x400;
}

void bhv_hidden_red_coin_star_init(void) {
    s16 sp36;
    struct Object *sp30;

    if (gCurrCourseNum != 3)
        spawn_object(o, MODEL_TRANSPARENT_STAR, bhvRedCoinStarMarker);

    sp36 = count_objects_with_behavior(bhvRedCoin);
    if (sp36 == 0) {
        sp30 = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
        sp30->oBehParams = o->oBehParams;
        o->activeFlags = 0;
    }

    o->oHiddenRedCoinStarCoinsCollected = 8 - sp36;
}

void bhv_hidden_red_coin_star_loop(void) {
    D_8036008E = o->oHiddenRedCoinStarCoinsCollected;
    switch (o->oAction) {
        case 0:
            if (o->oHiddenRedCoinStarCoinsCollected == 8) 
                o->oAction = 1;
            break;

        case 1:
            if (o->oTimer > 2) {
                func_802F1B84(o->oPosX, o->oPosY, o->oPosZ);
                func_802A3004();
                o->activeFlags = 0;
            }
            break;
    }
}

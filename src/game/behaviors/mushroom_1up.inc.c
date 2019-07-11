// mushroom_1up.c.inc

void bhv_1up_interact(void) {
    UNUSED s32 sp1C;

    if (are_objects_collided(o, gMarioObject) == 1) {
        SetSound(SOUND_GENERAL_1UP, D_803320E0);
        gMarioState->numLives++;
        o->activeFlags = 0;
    }
}

void bhv_1up_common_init(void) {
    o->oMoveAnglePitch = -0x4000;
    o->oGravity = 3.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.0f;
}

void bhv_1up_init(void) {
    bhv_1up_common_init();
    if (o->oBehParams2ndByte == 1) {
        if ((save_file_get_flags() & 0x50) == 0)
            o->activeFlags = 0;
    } 
    else if (o->oBehParams2ndByte == 2) {
        if ((save_file_get_flags() & 0xa0) == 0)
            o->activeFlags = 0;
    }
}

void func_802F2D40(void) {
    if (o->oTimer < 5) {
        o->oVelY = 40.0f;
    }
    else {
        o->oAngleVelPitch = -0x1000;
        o->oMoveAnglePitch += o->oAngleVelPitch;
        o->oVelY = coss(o->oMoveAnglePitch) * 30.0f + 2.0f;
        o->oForwardVel = -sins(o->oMoveAnglePitch) * 30.0f;
    }
}

void func_802F2E18(void) {
    f32 sp34 = gMarioObject->header.gfx.pos[0] - o->oPosX;
    f32 sp30 = gMarioObject->header.gfx.pos[1] + 120.0f - o->oPosY;
    f32 sp2C = gMarioObject->header.gfx.pos[2] - o->oPosZ;
    s16 sp2A = atan2s(sqrtf(sqr(sp34) + sqr(sp2C)), sp30);

    obj_turn_toward_object(o, gMarioObject, 16, 0x1000);
    o->oMoveAnglePitch = approach_s16_symmetric(o->oMoveAnglePitch, sp2A, 0x1000);
    o->oVelY = sins(o->oMoveAnglePitch) * 30.0f;
    o->oForwardVel = coss(o->oMoveAnglePitch) * 30.0f;
    bhv_1up_interact();
}

void func_802F2F8C(s16 sp1A) {
    o->oForwardVel = 8.0f;
    o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
    bhv_1up_interact();
    if (sp1A & 0x02) 
        o->oAction = 2;

    if (!IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 3000))
        o->oAction = 2;
}

void bhv_1up_walking_loop(void) {
    ObjectStep();

    switch(o->oAction) {
        case 0:
            if (o->oTimer >= 18) 
                spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);

            if (o->oTimer == 0)
                SetSound(SOUND_CH8_UNK63, D_803320E0);

            func_802F2D40();

            if (o->oTimer == 37) {
                obj_become_tangible();
                o->oAction = 1;
                o->oForwardVel = 2.0f;
            }
            break;

        case 1:
            if (o->oTimer > 300)
                o->oAction = 2;

            bhv_1up_interact();
            break;

        case 2:
            ObjFlickerAndDisappear(o, 30);
            bhv_1up_interact();
            break;
    }

    SetObjectVisibility(o, 3000);
}

void bhv_1up_running_away_loop(void) {
    s16 sp26;

    sp26 = ObjectStep();
    switch (o->oAction) {
        case 0:
            if (o->oTimer >= 18) 
                spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);

            if (o->oTimer == 0)
                SetSound(SOUND_CH8_UNK63, D_803320E0);

            func_802F2D40();

            if (o->oTimer == 37) {
                obj_become_tangible();
                o->oAction = 1;
                o->oForwardVel = 8.0f;
            }
            break;

        case 1:
            spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);
            func_802F2F8C(sp26);
            break;

        case 2:
            ObjFlickerAndDisappear(o, 30);
            bhv_1up_interact();
            break;
    }

    SetObjectVisibility(o, 3000);
}

void func_802F3328(void) {
    s16 sp1E;

    sp1E = ObjectStep();
    if (sp1E & 0x01) {
        o->oForwardVel += 25.0f;
        o->oVelY = 0;
    } else {
        o->oForwardVel *= 0.98;
    }

    if (o->oForwardVel > 40.0)
        o->oForwardVel = 40.0f;

    if (!IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 5000)) 
        o->oAction = 2;
}

void bhv_1up_sliding_loop(void) {
    switch (o->oAction) {
        case 0:
            SetObjectVisibility(o, 3000);
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1000)) 
                o->oAction = 1;
            break;

        case 1:
            func_802F3328();
            break;

        case 2:
            ObjFlickerAndDisappear(o, 30);
            bhv_1up_interact();
            break;
    }

    bhv_1up_interact();
    spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);
}

void bhv_1up_loop(void) {
    bhv_1up_interact();
    SetObjectVisibility(o, 3000);
}

void bhv_1up_jump_on_approach_loop(void) {
    s16 sp26;

    switch (o->oAction) {
        case 0:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1000)) {
                o->oVelY = 40.0f;
                o->oAction = 1;
            }
            break;

        case 1:
            sp26 = ObjectStep();
            func_802F2F8C(sp26);
            spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);
            break;

        case 2:
            sp26 = ObjectStep();
            bhv_1up_interact();
            ObjFlickerAndDisappear(o, 30);
            break;
    }

    SetObjectVisibility(o, 3000);
}

void bhv_1up_hidden_loop(void) {
    s16 sp26;
    switch (o->oAction) {
        case 0:
            o->header.gfx.node.flags |= 0x10;
            if (o->o1UpHiddenUnkF4 == o->oBehParams2ndByte)  {
                o->oVelY = 40.0f; 
                o->oAction = 3;
                o->header.gfx.node.flags &= ~0x10;
                SetSound(SOUND_CH8_UNK63, D_803320E0);
            }
            break;

        case 1:
            sp26 = ObjectStep();
            func_802F2F8C(sp26);
            spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);
            break;

        case 2:
            sp26 = ObjectStep();
            bhv_1up_interact();
            ObjFlickerAndDisappear(o, 30);
            break;

        case 3:
            sp26 = ObjectStep();
            if (o->oTimer >= 18) 
                spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);

            func_802F2D40();

            if (o->oTimer == 37) {
                obj_become_tangible();
                o->oAction = 1;
                o->oForwardVel = 8.0f;
            }
            break;
    }
}

void bhv_1up_hidden_trigger_loop(void) {
    struct Object *sp1C;
    if (are_objects_collided(o, gMarioObject) == 1) {
        sp1C = obj_nearest_object_with_behavior(bhvHidden1up);
        if (sp1C != NULL) 
            sp1C->o1UpHiddenUnkF4++;

        o->activeFlags = 0;
    }
}

void bhv_1up_hidden_in_pole_loop(void) {
    UNUSED s16 sp26;
    switch (o->oAction) {
        case 0:
            o->header.gfx.node.flags |= 0x10;
            if (o->o1UpHiddenUnkF4 == o->oBehParams2ndByte)  {
                o->oVelY = 40.0f; 
                o->oAction = 3;
                o->header.gfx.node.flags &= ~0x10;
                SetSound(SOUND_CH8_UNK63, D_803320E0);
            }
            break;

        case 1:
            func_802F2E18();
            sp26 = ObjectStep();
            break;

        case 3:
            sp26 = ObjectStep();
            if (o->oTimer >= 18) 
                spawn_object(o, MODEL_NONE, bhvPowerupSparkles2);

            func_802F2D40();

            if (o->oTimer == 37) {
                obj_become_tangible();
                o->oAction = 1;
                o->oForwardVel = 10.0f;
            }
            break;
    }
}

void bhv_1up_hidden_in_pole_trigger_loop(void) {
    struct Object *sp1C;

    if (are_objects_collided(o, gMarioObject) == 1) {
        sp1C = obj_nearest_object_with_behavior(bhvHidden1upInPole);
        if (sp1C != NULL) {
            sp1C->o1UpHiddenUnkF4++;;
        }

        o->activeFlags = 0;
    }
}

void bhv_1up_hidden_in_pole_spawner_loop(void) {
    s8 sp2F;

    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 700)) {
        spawn_object_relative(2, 0, 50, 0, o, MODEL_1UP, bhvHidden1upInPole);
        for (sp2F = 0; sp2F < 2; sp2F++) {
            spawn_object_relative(0, 0, sp2F * -200, 0, o, MODEL_NONE, bhvHidden1upInPoleTrigger);
        }

        o->activeFlags = 0;
    }
}

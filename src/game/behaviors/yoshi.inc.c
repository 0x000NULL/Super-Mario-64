// yoshi.c.inc

// X/Z coordinates of Yoshi's homes that he switches between.
// Note that this doesn't contain the Y coordinate since the castle roof is flat,
// so o->oHomeY is never updated.
static s16 sYoshiHomeLocations[] = {
     0,     -5625, 
    -1364,  -5912, 
    -1403,  -4609, 
    -1004,  -5308
};

void bhv_yoshi_init(void) {
    o->oGravity = 2.0f;
    o->oFriction = 0.9f;
    o->oBuoyancy = 1.3f;
    o->oUnk190 = 0x4000;

    if (save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 24) < 120 
        || D_80331508 == 1) {
        o->activeFlags = 0;
    }
}

void yoshi_walk_loop(void) {
    UNUSED s16 sp26;
    s16 sp24 = o->header.gfx.unk38.animFrame;

    o->oForwardVel = 10.0f;
    sp26 = ObjectStep();
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oYoshiTargetYaw, 0x500);
    if (IsPointCloseToObject(o, o->oHomeX, 3174.0f, o->oHomeZ, 200))
        o->oAction = YOSHI_ACT_IDLE;

    SetObjAnimation(1);
    if (sp24 == 0 || sp24 == 15) 
        PlaySound2(SOUND_GENERAL_YOSHIWALK);

    if (o->oInteractStatus == INT_STATUS_INTERACTED)
        o->oAction = YOSHI_ACT_TALK;

    if (o->oPosY < 2100.0f) {
        create_respawner(MODEL_YOSHI, bhvYoshi, 3000);
        o->activeFlags = 0;
    }
}

void yoshi_idle_loop(void) {
    s16 chosenHome;
    UNUSED s16 sp1C = o->header.gfx.unk38.animFrame;

    if (o->oTimer > 90) {
        chosenHome = RandomFloat() * 3.99;
        
        if (o->oYoshiChosenHome == chosenHome) {
            return;
        }
        else {
            o->oYoshiChosenHome = chosenHome;
        }

        o->oHomeX = sYoshiHomeLocations[o->oYoshiChosenHome * 2];
        o->oHomeZ = sYoshiHomeLocations[o->oYoshiChosenHome * 2 + 1];
        o->oYoshiTargetYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
        o->oAction = YOSHI_ACT_WALK; 
    }

    SetObjAnimation(0);
    if (o->oInteractStatus == INT_STATUS_INTERACTED) 
        o->oAction = YOSHI_ACT_TALK;

    // Credits; Yoshi appears at this position overlooking the castle near the end of the credits
    if (gPlayerStatusForCamera->unk1C[1] == 11 || gPlayerStatusForCamera->unk1C[1] == 12) {
        o->oAction = YOSHI_ACT_CREDITS;
        o->oPosX = -1798.0f;
        o->oPosY = 3174.0f;
        o->oPosZ = -3644.0f;
    }
}

void yoshi_talk_loop(void) {
    if ((s16) o->oMoveAngleYaw == (s16)o->oAngleToMario) {
        SetObjAnimation(0);
        if (set_mario_npc_dialogue(1) == 2) {
            o->activeFlags |= 0x20;
            if (func_8028F8E0(162, o, 161)) {
                o->activeFlags &= ~0x20;
                o->oInteractStatus = 0;
                o->oHomeX = sYoshiHomeLocations[2];
                o->oHomeZ = sYoshiHomeLocations[3];
                o->oYoshiTargetYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
                o->oAction = YOSHI_ACT_GIVE_PRESENT;
            }
        }
    }
    else {
        SetObjAnimation(1);
        play_puzzle_jingle();
        o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x500);
    }
}

void yoshi_walk_and_jump_off_roof_loop(void) {
    s16 sp26 = o->header.gfx.unk38.animFrame;

    o->oForwardVel = 10.0f;
    ObjectStep();
    SetObjAnimation(1);
    if (o->oTimer == 0)
        func_8028F9E8(173, o);

    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oYoshiTargetYaw, 0x500);
    if (IsPointCloseToObject(o, o->oHomeX, 3174.0f, o->oHomeZ, 200)) {
        SetObjAnimation(2);
        PlaySound2(SOUND_GENERAL_ENEMYALERT1);
        o->oForwardVel = 50.0f;
        o->oVelY = 40.0f;
        o->oMoveAngleYaw = -0x3FFF;
        o->oAction = YOSHI_ACT_FINISH_JUMPING_AND_DESPAWN;
    }

    if (sp26 == 0 || sp26 == 15) {
        PlaySound2(SOUND_GENERAL_YOSHIWALK);
    }
}

void yoshi_finish_jumping_and_despawn_loop(void) {
    func_8029F728();
    func_802E4250(o);
    o->oVelY -= 2.0;
    if (o->oPosY < 2100.0f) {
        set_mario_npc_dialogue(0);
        gCutsceneActive = 1;
        D_80331508 = 1;
        o->activeFlags = 0;
    }
}

void yoshi_give_present_loop(void) {
    s32 sp1C = gGlobalTimer;
    
    if (gDisplayedLives == 100) {
        SetSound(SOUND_GENERAL_1UP, D_803320E0);
        gSpecialTripleJump = 1;
        o->oAction = YOSHI_ACT_WALK_JUMP_OFF_ROOF;
        return;
    }

    if ((sp1C & 0x03) == 0) {
        SetSound(SOUND_MENU_YOSHIGAINLIVES, D_803320E0);
        gMarioState->numLives++;
    }
}

void BehYoshiLoop(void) {
    switch (o->oAction) {
        case YOSHI_ACT_IDLE:
            yoshi_idle_loop();
            break;

        case YOSHI_ACT_WALK:
            yoshi_walk_loop();
            break;

        case YOSHI_ACT_TALK:
            yoshi_talk_loop();
            break;

        case YOSHI_ACT_WALK_JUMP_OFF_ROOF:
            yoshi_walk_and_jump_off_roof_loop();
            break;

        case YOSHI_ACT_FINISH_JUMPING_AND_DESPAWN: 
            yoshi_finish_jumping_and_despawn_loop();
            break;

        case YOSHI_ACT_GIVE_PRESENT:
            yoshi_give_present_loop();
            break;

        case YOSHI_ACT_CREDITS:
            SetObjAnimation(0);
            break;
    }

    ObjRandomBlink(&o->oYoshiUnkF4);
}

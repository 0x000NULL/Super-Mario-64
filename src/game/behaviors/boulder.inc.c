// boulder.c.inc

void bhv_big_boulder_init(void) {
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;

    o->oGravity = 8.0f;
    o->oFriction = 0.999f;
    o->oBuoyancy = 2.0f;
}

void func_802F05DC(void) {
    s16 sp1E;

    sp1E = func_802E4204();
    if ((sp1E & 0x09) == 0x01 && o->oVelY > 10.0f) {
        PlaySound2(SOUND_GENERAL_GRINDELROLL);
        func_802A3004();
    } 

    if (o->oForwardVel > 70.0)
        o->oForwardVel = 70.0f;

    if (o->oPosY < -1000.0f)
        o->activeFlags = 0;
}

void bhv_big_boulder_loop(void) {
    obj_scale(1.5f);
    o->oGraphYOffset = 270.0f;
    switch (o->oAction) {
        case 0:
            o->oForwardVel = 40.0f;
            o->oAction = 1;
            break;

        case 1:
            func_802F05DC();
            func_802EFB84(1.5f);
            PlaySound(SOUND_ENVIRONMENT_UNKNOWN2);
            break;
    }

    func_802EFB2C();
}

void bhv_big_boulder_generator_loop(void) {
    struct Object *sp1C;
    if (o->oTimer >= 256) {
        o->oTimer = 0;
    }

    if (!func_802E49A4(4) || IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1500))
        return;

    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 6000)) {
        if ((o->oTimer & 0x3F) == 0) {
            sp1C = spawn_object(o, MODEL_HMC_ROLLING_ROCK, bhvBigBoulder);
            sp1C->oMoveAngleYaw = RandomFloat() * 4096.0f;
        }
    } 
    else {
        if ((o->oTimer & 0x7F) == 0) {
            sp1C = spawn_object(o, MODEL_HMC_ROLLING_ROCK, bhvBigBoulder);
            sp1C->oMoveAngleYaw = RandomFloat() * 4096.0f;
        }
    }
}

// mips.c.inc

void bhv_mips_init(void) {
    u8 sp1F;
    sp1F = save_file_get_star_flags(gCurrSaveFileNum - 1, -1);
    if (save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 24) >= 15 
        && (sp1F & 0x08) == 0) {
        o->oBehParams2ndByte = 0;
#ifdef VERSION_US
        o->oUnk1AC_F32 = 40.0f;
#endif
    } 
    else if (save_file_get_total_star_count(gCurrSaveFileNum-1, 0, 24) >= 50
        && (sp1F & 0x10) == 0) {
        o->oBehParams2ndByte = 1;
#ifdef VERSION_US
        o->oUnk1AC_F32 = 45.0f;
#endif
    }
    else {
        o->activeFlags = 0;
    }

    o->oUnk190 = 0x10;
#ifdef VERSION_US
    o->oGravity = 15.0f;
#else
    o->oGravity = 2.5f;
#endif
    o->oFriction = 0.89f;
    o->oBuoyancy = 1.2f;
    SetObjAnimation(0);
}

s16 func_802F7528(void) {
    s8 sp3F;
    s16 sp3C, sp3A, sp38;
    s16 sp36 = -1;
    f32 sp30 = -10000.0f;
    f32 sp2C;
    void **sp28;
    struct Waypoint *sp24;

    sp28 = segmented_to_virtual(&inside_castle_seg7_trajectory_mips);

    for (sp3F = 0; sp3F < 10; sp3F++) {
        sp24 = (struct Waypoint *)segmented_to_virtual(*(sp28 + sp3F));
        sp3C = sp24->pos[0];
        sp3A = sp24->pos[1];
        sp38 = sp24->pos[2];
        if (IsPointCloseToObject(o, sp3C, sp3A, sp38, 800)) {
            sp2C = sqr(sp3C - gMarioObject->header.gfx.pos[0]) + sqr(sp38 - gMarioObject->header.gfx.pos[2]);
            if (sp30 < sp2C) {
                sp36 = sp3F;
                sp30 = sp2C;
            }
        }
    }

    o->oMipsUnkF8 = sp36;
    return (s16)o->oMipsUnkF8;
}

void func_802F76A8(void) {
    UNUSED s16 sp1E = 0;

    o->oForwardVel = 0.0f;
    sp1E = ObjectStep();

    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 500)) {
        if (func_802F7528() == -1) {
            o->oAction = 2;
        }
        else {
            SetObjAnimation(1);
            o->oAction = 1;
        }
    }
}

void func_802F7750(void) {
    s16 sp26 = 0;
    s32 sp20;
    void **sp1C;
    struct Waypoint *sp18;

    sp1C = segmented_to_virtual(&inside_castle_seg7_trajectory_mips);
    sp18 = (struct Waypoint *)segmented_to_virtual(*(sp1C + o->oMipsUnkF8));
    o->oMipsUnkFC = sp18;
    sp20 = obj_follow_path(sp20);
#ifdef VERSION_US
    o->oForwardVel = o->oUnk1AC_F32;
#else
    o->oForwardVel = 45.0f;
#endif
    o->oMoveAngleYaw = o->oMipsUnk10C;
    sp26 = ObjectStep();
    if (sp20 == -1) {
        SetObjAnimation(0);
        o->oAction = 0;
    }
    if (func_8029F788() == 1 && (sp26 & 0x04)) {
        PlaySound2(SOUND_OBJECT_MIPSRABBITWATER);
        spawn_object(o, MODEL_NONE, bhvSurfaceWaveShrinking);
    } else if (func_8029F788() == 1) {
        PlaySound2(SOUND_OBJECT_MIPSRABBIT);
    }
}

void func_802F788C(void) {
    if (func_8029F788() == 1) {
        SetObjAnimation(0);
        o->oAction = 4;
    }
}

void func_802F78D8(void) {
    s16 sp1E = 0;

    sp1E = ObjectStep();
    o->header.gfx.unk38.animFrame = 0;
    if ((sp1E & 0x01) == 1) {
        o->oAction = 2;
        o->oFlags |= 0x08;
        o->oMoveAngleYaw = o->oFaceAngleYaw;
        if (sp1E & 0x04)
            spawn_object(o, MODEL_NONE, bhvSurfaceWaveShrinking);
    }
}

void func_802F7984(void) {
    UNUSED s16 sp1E = 0;

    o->oForwardVel = 0;
    sp1E = ObjectStep();
    if (o->oMipsUnkF4 == 1) {
        bhv_spawn_star_objects(o->oBehParams2ndByte + 3);
        o->oMipsUnkF4 = 2;
    }
}

void func_802F7A04(void) {
    switch (o->oAction) {
        case 0:
            func_802F76A8();
            break;

        case 1:
            func_802F7750();
            break;

        case 2:
            func_802F788C();
            break;

        case 3:
            func_802F78D8();
            break;

        case 4:
            func_802F7984();
            break;
    }
}

void func_802F7AA4(void) {
    s16 sp1E;

    o->header.gfx.node.flags |= 0x10;
    SetObjAnimation(4);
    obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0f);
    obj_become_intangible();
    if (o->oMipsUnkF4 == 0) {
        if (o->oBehParams2ndByte == 0) 
            sp1E = 84;
        else
            sp1E = 162;

        if (set_mario_npc_dialogue(1) == 2) {
            o->activeFlags |= 0x20;
            if (func_8028F8E0(162, o, sp1E)) {
                o->oUnk190 |= 0x40;
                o->activeFlags &= ~0x20;
                o->oMipsUnkF4 = 1;
                set_mario_npc_dialogue(0);
            }
        }
    }
}

void func_802F7BC4(void) {
    obj_get_dropped();
    o->header.gfx.node.flags &= ~0x10;
    SetObjAnimation(0);
    o->oHeldState = 0;
    obj_become_tangible();
    o->oForwardVel = 3.0f;
    o->oAction = 4;
}

void func_802F7C48(void) {
    obj_enable_rendering_2();
    o->header.gfx.node.flags &= ~0x10;
    o->oHeldState = 0;
    o->oFlags &= ~0x08;
    SetObjAnimation(2);
    obj_become_tangible();
    o->oForwardVel = 25.0f;
    o->oVelY = 20.0f;
    o->oAction = 3;
}

void bhv_mips_loop(void) {
    switch (o->oHeldState) {
        case 0:
            func_802F7A04();
            break;

        case 1:
            func_802F7AA4();
            break;

        case 2:
            func_802F7C48();
            break;

        case 3:
            func_802F7BC4();
            break;
    }
}

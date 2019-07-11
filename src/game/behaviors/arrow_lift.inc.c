// arrow_lift.c.inc

s32 func_802F59B4(void) {
    s8 sp1F = 0;
    o->oMoveAngleYaw = o->oFaceAngleYaw - 0x4000;
    o->oVelY = 0;
    o->oForwardVel = 12.0f;
    o->oUnknownUnkF4_F32 += o->oForwardVel;

    if (o->oUnknownUnkF4_F32 > 384.0f) {
        o->oForwardVel = 0.0f;
        o->oUnknownUnkF4_F32 = 384.0f;
        sp1F = 1;
    }

    func_802E4250(o);
    return sp1F;
}

s8 func_802F5A9C(void) {
    s8 sp1F = 0;
    o->oMoveAngleYaw = o->oFaceAngleYaw + 0x4000;
    o->oVelY = 0;
    o->oForwardVel = 12.0f;
    o->oUnknownUnkF4_F32 -= o->oForwardVel;

    if (o->oUnknownUnkF4_F32 < 0.0f) {
        o->oForwardVel = 0.0f;
        o->oUnknownUnkF4_F32 = 0.0f;
        sp1F = 1;
    }

    func_802E4250(o);
    return sp1F;
}

void bhv_arrow_lift_loop(void) {
    switch(o->oAction) {
        case 0:
            if (o->oTimer > 60) {
                if (gMarioObject->platform == o)
                    o->oAction = 1;
            }
            break;

        case 1:
            if (func_802F59B4() == 1)
                o->oAction = 2;
            break;

        case 2:
            if (o->oTimer > 60) {
                if (func_802F5A9C() == 1)
                    o->oAction = 0;
            }
            break;
    }
}

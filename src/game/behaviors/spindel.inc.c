// spindel.c.inc

void bhv_spindel_init(void) {
    o->oHomeY = o->oPosY;
    o->oUnknownUnkF4_S32 = 0;
    o->oUnknownUnkF8_S32 = 0;
}

void bhv_spindel_loop(void) {
    f32 sp1C;
    s32 sp18;

    if (o->oUnknownUnkF4_S32 == -1) {
        if (o->oTimer == 32) {
            o->oUnknownUnkF4_S32 = 0;
            o->oTimer = 0;
        }
        else {
            o->oVelZ = 0.0f;
            o->oAngleVelPitch = 0;
            return;
        }
    }

    sp18 = 10 - o->oUnknownUnkF4_S32;

    if (sp18 < 0)
        sp18 *= -1;

    sp18 -= 6;
    if (sp18 < 0)
        sp18 = 0;

    if (o->oTimer == sp18 + 8) {
        o->oTimer = 0;
        o->oUnknownUnkF4_S32++;
        if (o->oUnknownUnkF4_S32 == 20) {
            if (o->oUnknownUnkF8_S32 == 0) {
                o->oUnknownUnkF8_S32 = 1;
            }
            else {
                o->oUnknownUnkF8_S32 = 0;
            }

            o->oUnknownUnkF4_S32 = -1;
        }
    }

    if (sp18 == 4 || sp18 == 3)
        sp18 = 4;
    else if (sp18 == 2 || sp18 == 1)
        sp18 = 2;
    else if (sp18 == 0)
        sp18 = 1;

    if (o->oTimer < sp18 * 8) {
        if (o->oUnknownUnkF8_S32 == 0) {
            o->oVelZ = 20 / sp18;
            o->oAngleVelPitch = 1024 / sp18;
        }
        else {
            o->oVelZ = -20 / sp18;
            o->oAngleVelPitch = -1024 / sp18;
        }

        o->oPosZ += o->oVelZ;
        o->oMoveAnglePitch += o->oAngleVelPitch;

        if (absf_2(o->oMoveAnglePitch & 0x1fff) < 800.0f
            && o->oAngleVelPitch != 0) {
            PlaySound2(SOUND_CH8_UNK48);
        }

        sp1C = sins(o->oMoveAnglePitch * 4) * 23.0;
        if (sp1C < 0.0f)
            sp1C *= -1.0f;

        o->oPosY = o->oHomeY + sp1C; 

        if (o->oTimer + 1 == sp18 * 8)
            func_8027F440(1, o->oPosX, o->oPosY, o->oPosZ);
    }
}

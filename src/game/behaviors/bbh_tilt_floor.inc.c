// bbh_tilt_floor.c.inc

void bhv_bbh_tilt_floor_platform_loop(void)
{
#ifndef VERSION_JP
    UNUSED s32 unused;
    if (gMarioObject->platform == o)
        o->oAction = 0;
    else
        o->oAction = 1;
    
    if (o->oAction == 0)
    {
        o->oAngleVelPitch = (s32)(o->oDistanceToMario * coss(o->oAngleToMario));
        o->oFaceAnglePitch += o->oAngleVelPitch;
    }
    else
    {
        if ((absi(o->oFaceAnglePitch) < 3000) || (o->oTimer >= 16))
        {
            o->oAngleVelPitch = 0;
            if((s16)o->oFaceAnglePitch > 0)
            {
                if(o->oFaceAnglePitch < 200)
                    o->oFaceAnglePitch = 0;
                else
                    o->oAngleVelPitch = -200;
            }
            else
            {
                if (o->oFaceAnglePitch > -200)
                    o->oFaceAnglePitch = 0;
                else
                    o->oAngleVelPitch = 200;
            }
        }
    }
    o->oFaceAnglePitch += o->oAngleVelPitch;
#else
    UNUSED s32 unused;
    if(gMarioObject->platform == o)
    {
        o->oAngleVelPitch = (s32)(o->oDistanceToMario * coss(o->oAngleToMario));
        o->oFaceAnglePitch += o->oAngleVelPitch;
    }
    else
    {
        o->oAngleVelPitch = 0;
        if((s16)o->oFaceAnglePitch > 0)
        {
            if(o->oFaceAnglePitch < 200)
                o->oFaceAnglePitch = 0;
            else
                o->oAngleVelPitch = -200;
        }
        else if(o->oFaceAnglePitch > -200)
            o->oFaceAnglePitch = 0;
        else
            o->oAngleVelPitch = 200;
    }
    o->oFaceAnglePitch += o->oAngleVelPitch;
#endif
}

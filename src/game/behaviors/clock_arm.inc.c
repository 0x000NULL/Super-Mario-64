// clock_arm.c.inc

void bhv_rotating_clock_arm_loop(void)
{
    struct Surface* sp1C;
    u16 sp1A = o->oFaceAngleRoll;
    o->oFloorHeight = find_floor(gMarioObject->oPosX,gMarioObject->oPosY,gMarioObject->oPosZ,&sp1C);
    if(o->oAction == 0)
    {
        if(sp1C->type == SURFACE_DEFAULT)
            if(o->oTimer >= 4)
                o->oAction++;
    }
    else if(o->oAction == 1)
    {
        if(sp1C != NULL && (sp1C->type == SURFACE_TTC_PAINTING_1 || sp1C->type == SURFACE_TTC_PAINTING_2 || sp1C->type == SURFACE_TTC_PAINTING_3))
        {
            if(obj_has_behavior(bhvClockMinuteHand))
            {
                if(sp1A < 0xAAA)
                    gTTCSpeedSetting = 3;
                else if(sp1A < 0x6aa4)
                    gTTCSpeedSetting = 1;
                else if(sp1A < 0x954C)
                    gTTCSpeedSetting = 2;
                else if(sp1A < 0xf546)
                    gTTCSpeedSetting = 0;
                else
                    gTTCSpeedSetting = 3;
            }
            o->oAction++;
        }
        else {}
    }
    if(o->oAction < 2)
        obj_rotate_face_angle_using_vel();
}

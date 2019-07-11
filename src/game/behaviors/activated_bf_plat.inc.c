
// Note: The filename is abbreviated to avoid compiler seg fault on long paths

void *D_80331AD4[] =
{
    bits_seg7_collision_0701AD54,
    bitfs_seg7_collision_070157E0,
    bitfs_seg7_collision_07015124,
};

void bhv_activated_back_and_forth_platform_init(void)
{
    s32 instance = ((u16)(o->oBehParams >> 16) & 0x0300) >> 8;
    
    if (instance != 0)
    {
        o->oActivatedBackAndForthPlatformFlipRotation = 0;
    }
    else
    {
        o->oActivatedBackAndForthPlatformFlipRotation = 0x8000;
    }

    o->collisionData = segmented_to_virtual(D_80331AD4[instance]);

    o->oActivatedBackAndForthPlatformMaxOffset = 50.0f * ((u16)(o->oBehParams >> 16) & 0x007F);
    if (instance == 2)
    {
        o->oActivatedBackAndForthPlatformMaxOffset -= 12.0f;
    }

    o->oActivatedBackAndForthPlatformVertical = (u16)(o->oBehParams >> 16) & 0x0080;
    o->oActivatedBackAndForthPlatformStartYaw = o->oFaceAngleYaw;
}

void bhv_activated_back_and_forth_platform_update(void)
{
    UNUSED s32 unused[3];

    if (gMarioObject->platform == o)
    {
        o->oVelY = -6.0f;
    }
    else
    {
        o->oVelY = 6.0f;
    }

    if (o->oActivatedBackAndForthPlatformVel != 0.0f)
    {
        if (o->oActivatedBackAndForthPlatformCountdown != 0)
        {
            o->oActivatedBackAndForthPlatformCountdown -= 1;
        }
        else
        {
            o->oActivatedBackAndForthPlatformOffset += o->oActivatedBackAndForthPlatformVel;

            if (clamp_f32(&o->oActivatedBackAndForthPlatformOffset, 0.0f, o->oActivatedBackAndForthPlatformMaxOffset) ||
                (o->oActivatedBackAndForthPlatformVel > 0.0f &&
                o->oDistanceToMario > 3000.0f))
            {
                o->oActivatedBackAndForthPlatformCountdown = 20;
                if (o->oVelY < 0.0f || o->oActivatedBackAndForthPlatformVel > 0.0f)
                {
                    o->oActivatedBackAndForthPlatformVel = -o->oActivatedBackAndForthPlatformVel;
                }
                else
                {
                    o->oActivatedBackAndForthPlatformVel = 0.0f;
                }

                o->oFaceAngleYaw += o->oActivatedBackAndForthPlatformFlipRotation;
            }
        }
    }
    else
    {
        if (o->oVelY < 0.0f)
        {
            o->oActivatedBackAndForthPlatformVel = 10.0f;
        }
        o->oActivatedBackAndForthPlatformCountdown = 20;
    }

    obj_perform_position_op(0);
    
    if (o->oActivatedBackAndForthPlatformVertical != 0)
    {
        o->oPosY = o->oHomeY + o->oActivatedBackAndForthPlatformOffset;
    }
    else
    {
        o->oPosY += o->oVelY;
        clamp_f32(&o->oPosY, o->oHomeY - 20.0f, o->oHomeY);
        obj_set_dist_from_home(-o->oActivatedBackAndForthPlatformOffset);
    }
 
    obj_perform_position_op(1);
}

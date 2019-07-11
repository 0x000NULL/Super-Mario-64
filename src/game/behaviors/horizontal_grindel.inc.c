
void bhv_horizontal_grindel_init(void)
{
    o->oHorizontalGrindelTargetYaw = o->oMoveAngleYaw;
}

void bhv_horizontal_grindel_update(void)
{
    if (o->oMoveFlags & 0x00000003)
    {
        if (!o->oHorizontalGrindelOnGround)
        {
            PlaySound2(SOUND_OBJECT_THWOMP);
            o->oHorizontalGrindelOnGround = TRUE;
            func_8027F440(1, o->oPosX, o->oPosY, o->oPosZ);

            o->oHorizontalGrindelDistToHome = obj_lateral_dist_to_home();
            o->oForwardVel = 0.0f;
            o->oTimer = 0;
        }

        if (obj_rotate_yaw_toward(o->oHorizontalGrindelTargetYaw, 0x400))
        {
            if (o->oTimer > 60)
            {
                if (o->oHorizontalGrindelDistToHome > 300.0f)
                {
                    o->oHorizontalGrindelTargetYaw += 0x8000;
                    o->oHorizontalGrindelDistToHome = 0.0f;
                }
                else
                {
                    PlaySound2(SOUND_OBJECT_KINGBOBOMBJUMP);
                    o->oForwardVel = 11.0f;
                    o->oVelY = 70.0f;
                    o->oGravity = -4.0f;
                    o->oMoveFlags = 0;
                }
            }
        }
        else
        {
            o->oTimer = 0;
        }
    }
    else
    {
        o->oHorizontalGrindelOnGround = FALSE;
        if (o->oVelY < 0.0f)
        {
            o->oGravity = -16.0f;
        }
    }

    o->oFaceAngleYaw = o->oMoveAngleYaw + 0x4000;
    obj_move_standard(78);
}

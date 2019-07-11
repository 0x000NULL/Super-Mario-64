
static void bird_act_init(void)
{
    s32 i;

    if (o->oBehParams2ndByte == BIRD_BP_SPAWNED || o->oDistanceToMario < 2000.0f)
    {
        if (o->oBehParams2ndByte != BIRD_BP_SPAWNED)
        {
            PlaySound2(SOUND_GENERAL_BIRDSFLYAWAY);

            for (i = 0; i < 6; i++)
            {
                spawn_object(o, MODEL_BIRDS, bBird);
            }

            o->oHomeX = -20.0f;
            o->oHomeZ = -3990.0f;
        }

        o->oAction = BIRD_ACT_FLY;
        o->oMoveAnglePitch = 5000 - (s32)(4000.0f * RandomFloat());
        o->oMoveAngleYaw = RandomU16();
        o->oBirdSpeed = 40.0f;

        obj_unhide();
    }
}

static void bird_act_fly(void)
{
    UNUSED s32 unused;
    f32 distance;

    obj_compute_vel_from_move_pitch(o->oBirdSpeed);
    if (o->parentObj->oPosY > 8000.0f)
    {
        mark_object_for_deletion(o);
    }
    else
    {
        if (o->oBehParams2ndByte != BIRD_BP_SPAWNED)
        {
            distance = obj_lateral_dist_to_home();
            o->oBirdPitch = atan2s(distance, o->oPosY - 10000.0f);
            o->oBirdYaw = obj_angle_to_home();
        }
        else
        {
            distance = lateral_dist_between_objects(o, o->parentObj);
            o->oBirdPitch = atan2s(distance, o->oPosY - o->parentObj->oPosY);
            o->oBirdYaw = angle_to_object(o, o->parentObj);

            o->oBirdSpeed = 0.04f * dist_between_objects(o, o->parentObj) + 20.0f;
        }

        obj_move_pitch_approach(o->oBirdPitch, 140);
        obj_rotate_yaw_toward(o->oBirdYaw, 800);
        obj_roll_to_match_yaw_turn(o->oBirdYaw, 0x3000, 600);
    }

    obj_move_using_fvel_and_gravity();
}

void bhv_bird_update(void)
{
    switch (o->oAction)
    {
    case BIRD_ACT_INIT: bird_act_init(); break;
    case BIRD_ACT_FLY:  bird_act_fly();  break;
    }
}


/**
 * Behavior for bhvSwoop.
 * Has a native room.
 */


/**
 * Hitbox for swoop.
 */
static struct ObjectHitbox sSwoopHitbox =
{
    /* interactType:      */ INTERACT_HIT_FROM_BELOW,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 1,
    /* radius:            */ 100,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 70,
    /* hurtboxHeight:     */ 70,
};


/**
 * If necessary, grow to full size. Wait for mario to enter range, then turn
 * toward him and enter the move action.
 */
static void swoop_act_idle(void)
{
    func_8029ED38(1);

    if (approach_f32_ptr(&o->header.gfx.scale[0], 1.0f, 0.05f) &&
        o->oDistanceToMario < 1500.0f)
    {
        if (obj_rotate_yaw_toward(o->oAngleToMario, 0x320))
        {
            PlaySound2(SOUND_CH9_UNK49);
            o->oAction = SWOOP_ACT_MOVE;
            o->oVelY = -12.0f;
        }
    }

    o->oFaceAngleRoll = 0x8000;
}

/**
 * Swoop downward toward mario. Stop moving downward and speed up after reaching
 * him. Return to home once mario is far away.
 */
static void swoop_act_move(void)
{
    func_8029ED98(0, 2.0f);
    if (func_8029F788())
    {
        PlaySound2(SOUND_OBJECT_UNKNOWN6);
    }

    if (o->oForwardVel == 0.0f)
    {
        // If we haven't started moving yet, begin swooping
        if (obj_face_roll_approach(0, 0x9C4))
        {
            o->oForwardVel = 10.0f;
            o->oVelY = -10.0f;
        }
    }
    else if (obj_mario_far_away())
    {
        // If mario far away, reset
        o->oAction = SWOOP_ACT_IDLE;
        obj_set_pos_to_home();
        o->header.gfx.scale[0] = o->oForwardVel = o->oVelY = 0.0f;
        o->oFaceAngleRoll = 0;
    }
    else
    {
        if (o->oSwoopBonkCountdown != 0)
        {
            o->oSwoopBonkCountdown -= 1;
        }
        else if (o->oVelY != 0.0f)
        {
            // If we're not done swooping, turn toward mario. When between
            // 0 and 200 units above mario, increase speed and stop swooping
            o->oSwoopTargetYaw = o->oAngleToMario;
            if (o->oPosY < gMarioObject->oPosY + 200.0f)
            {
                if (obj_y_vel_approach(0.0f, 0.5f))
                {
                    o->oForwardVel *= 2.0f;
                }
            }
            else
            {
                obj_y_vel_approach(-10.0f, 0.5f);
            }
        }
        else if (o->oMoveFlags & OBJ_MOVE_HIT_WALL)
        {
            // Bounce off walls and get stunned for a second
            o->oSwoopTargetYaw = obj_reflect_move_angle_off_wall();
            o->oSwoopBonkCountdown = 30;
        }

        // Tilt upward when approaching mario
        if ((o->oSwoopTargetPitch = obj_get_pitch_from_vel()) == 0)
        {
            o->oSwoopTargetPitch += o->oForwardVel * 0x1F4;
        }
        obj_move_pitch_approach(o->oSwoopTargetPitch, 0x8C);

        // Jitter yaw a bit
        obj_rotate_yaw_toward(
            o->oSwoopTargetYaw + (s32)(0xBB8 * coss(0xFA0 * gGlobalTimer)),
            0x4B0);
        obj_roll_to_match_yaw_turn(o->oSwoopTargetYaw, 0x3000, 0x1F4);

        // Jitter roll a bit
        o->oFaceAngleRoll += (s32)(0x3E8 * coss(0x4E20 * gGlobalTimer));
    }
}

/**
 * Update function for swoop.
 */
void bhv_swoop_update(void)
{
    // No partial update (only appears in roomed levels)

    if (!(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM))
    {
        o->oDeathSound = SOUND_OBJECT_BATDEATH;

        obj_update_floor_and_walls();

        switch (o->oAction)
        {
        case SWOOP_ACT_IDLE: swoop_act_idle(); break;
        case SWOOP_ACT_MOVE: swoop_act_move(); break;
        }

        obj_scale(o->header.gfx.scale[0]);
        obj_move_standard(78);

        obj_check_attacks(&sSwoopHitbox, o->oAction);
    }
}

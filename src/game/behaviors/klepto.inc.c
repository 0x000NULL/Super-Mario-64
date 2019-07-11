static struct ObjectHitbox sKleptoHitbox =
{
    /* interactType:      */ INTERACT_HIT_FROM_BELOW,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 160,
    /* height:            */ 250,
    /* hurtboxRadius:     */ 80,
    /* hurtboxHeight:     */ 200,
};

static Vec3f sKleptoTargetPositions[] =
{
    { 2200.0f, 1250.0f, -2820.0f },
    { -6200.0f, 1250.0f, -2800.0f },
    { -6200.0f, 1250.0f, 1150.0f },
};

static u8 sKleptoAttackHandlers[] = { 2, 2, 5, 5, 2, 2 };

static void klepto_target_mario(void)
{
    o->oKleptoDistanceToTarget = lateral_dist_between_objects(gMarioObject, o);
    o->oKleptoUnk1B0 = obj_turn_pitch_toward_mario(250.0f, 0);
    o->oKleptoYawToTarget = o->oAngleToMario;
    o->oKleptoUnk1AE = -60;
}

static s32 func_8030F158(void)
{
    if (o->oSoundStateID == 6)
    {
        func_802F932C(5);
    }
    else if (o->oSoundStateID == 5)
    {
        if (func_802F932C(0))
        {
            PlaySound2(SOUND_GENERAL_SWISHWATER);
            o->header.gfx.unk38.animFrame = 9;
        }
    }
    else
    {
        if (obj_check_anim_frame(9))
        {
            PlaySound2(SOUND_GENERAL_SWISHWATER);
        }
        return TRUE;
    }

    return FALSE;
}

static void func_8030F218(void)
{
    if (o->oKleptoUnk1AE > 0)
    {
        if (o->oKleptoUnk1B0 < -400)
        {
            o->oKleptoUnk1AE = 0;
        }
        else
        {
            if (o->oSoundStateID == 0)
            {
                if (obj_check_anim_frame(9))
                {
                    PlaySound2(SOUND_GENERAL_SWISHWATER);
                    func_8029ED38(6);
                }
            }
            else if (--o->oKleptoUnk1AE == 0)
            {
                o->oKleptoUnk1AE = -random_linear_offset(60, 60);
            }

            obj_move_pitch_approach(400, 10);
        }
    }
    else
    {
        obj_move_pitch_approach(o->oKleptoUnk1B0, 600);
        if (func_8030F158() != 0)
        {
            if (o->oKleptoUnk1AE != 0)
            {
                o->oKleptoUnk1AE += 1;
            }
            else if (o->oKleptoUnk1B0 > -100)
            {
                o->oKleptoUnk1AE = random_linear_offset(60, 60);
            }
        }
    }

    obj_face_pitch_approach(0, 1000);
}

void bhv_klepto_init(void)
{
    if (o->oBehParams2ndByte != 0)
    {
        o->oAnimState = KLEPTO_ANIM_STATE_HOLDING_STAR;
    }
    else
    {
        o->oKleptoStartPosX = o->oPosX;
        o->oKleptoStartPosY = o->oPosY;
        o->oKleptoStartPosZ = o->oPosZ;

        if (save_file_get_flags() & SAVE_FLAG_CAP_ON_KLEPTO)
        {
            o->oAnimState = KLEPTO_ANIM_STATE_HOLDING_CAP;
        }
        else
        {
            o->oAction = KLEPTO_ACT_WAIT_FOR_MARIO;
        }
    }
}

static void klepto_change_target(void)
{
    s32 newTarget = 0;
    s32 i;
    f32 dx;
    f32 dz;
    f32 targetDist;
    f32 minTargetDist;

    if (o->oDistanceToMario > 2000.0f)
    {
        minTargetDist = 99999.0f;

        for (i = 0; i < 3; i++)
        {
            dx = gMarioObject->oPosX - sKleptoTargetPositions[i][0];
            dz = gMarioObject->oPosZ - sKleptoTargetPositions[i][2];

            targetDist = sqrtf(dx*dx + dz*dz);
            if (targetDist < minTargetDist)
            {
                minTargetDist = targetDist;
                newTarget = i;
            }
        }
    }
    else
    {
        newTarget = RandomU16() % 3;
    }

    o->oKleptoUnkF8 = 400 * absi(newTarget - o->oKleptoTargetNumber);
    o->oKleptoTargetNumber = newTarget;

    o->oHomeX = sKleptoTargetPositions[o->oKleptoTargetNumber][0];
    o->oHomeY = sKleptoTargetPositions[o->oKleptoTargetNumber][1] + o->oKleptoUnkF8;
    o->oHomeZ = sKleptoTargetPositions[o->oKleptoTargetNumber][2];

    o->oKleptoUnkFC = obj_lateral_dist_to_home() / 2.0f;
}

static void klepto_circle_target(f32 radius, f32 targetSpeed)
{
    s16 turnAmount;
    f32 accel;

    if (o->oAnimState != KLEPTO_ANIM_STATE_HOLDING_NOTHING &&
        ((o->oTimer > 60 && o->oDistanceToMario > 2000.0f) ||
            o->oTimer >= o->oKleptoTimeUntilTargetChange))
    {
        klepto_change_target();
        o->oKleptoTimeUntilTargetChange = random_linear_offset(300, 300);
        o->oAction = KLEPTO_ACT_APPROACH_TARGET_HOLDING;
    }
    else
    {
        turnAmount = 0x4000 - atan2s(radius, o->oKleptoDistanceToTarget - radius);
        accel = 0.05f;
        if ((s16)(o->oMoveAngleYaw - o->oKleptoYawToTarget) < 0)
        {
            turnAmount = -turnAmount;
        }

        o->oKleptoYawToTarget += turnAmount;
        func_8030F218();

        turnAmount = (s16)(abs_angle_diff(o->oKleptoYawToTarget, o->oMoveAngleYaw) * (0.03f * o->oKleptoSpeed));
        clamp_s16(&turnAmount, 400, 700);
        obj_rotate_yaw_and_bounce_off_walls(o->oKleptoYawToTarget, turnAmount);

        if (o->oKleptoSpeed > 50.0f)
        {
            accel = 2.0f;
        }

        approach_f32_ptr(&o->oKleptoSpeed, targetSpeed, accel);
    }
}

static void klepto_approach_target(f32 targetSpeed)
{
    if (o->oKleptoDistanceToTarget < 1800.0f)
    {
        o->oAction = KLEPTO_ACT_CIRCLE_TARGET_HOLDING;
    }
    else
    {
        if (o->oKleptoUnkFC > 0.0f)
        {
            if ((o->oKleptoUnkFC -= o->oForwardVel) <= 0.0f)
            {
                o->oHomeY -= o->oKleptoUnkF8;
            }
        }

        func_8030F218();
        obj_rotate_yaw_and_bounce_off_walls(o->oKleptoYawToTarget, 400);
        approach_f32_ptr(&o->oKleptoSpeed, targetSpeed, 0.05f);
    }
}

static void klepto_act_wait_for_mario(void)
{
    if (o->oKleptoDistanceToTarget < 1000.0f)
    {
        klepto_target_mario();
        if (o->oKleptoDistanceToTarget < 1000.0f)
        {
            o->oAction = KLEPTO_ACT_TURN_TOWARD_MARIO;
            o->oFlags &= ~0x00000008;
        }
    }

    klepto_circle_target(300.0f, 40.0f);
}

static void klepto_act_turn_toward_mario(void)
{
    klepto_target_mario();

    if (func_8030F158() &&
        func_8029F828() &&
        o->oKleptoDistanceToTarget > 800.0f &&
        abs_angle_diff(o->oAngleToMario, o->oFaceAngleYaw) < 0x800 &&
        o->oKleptoUnk1B0 < 0x400)
    {
        PlaySound2(SOUND_OBJECT_KLEPTO1);
        o->oAction = KLEPTO_ACT_DIVE_AT_MARIO;
        o->oMoveAngleYaw = o->oFaceAngleYaw;
        o->oFlags |= 0x00000008;

        func_8029ED38(3);
    }

    klepto_circle_target(1000.0f, 40.0f);
    obj_face_yaw_approach(o->oAngleToMario, 1000);
}

static void klepto_act_dive_at_mario(void)
{
    approach_f32_ptr(&o->oKleptoSpeed, 60.0f, 10.0f);
    if (o->oSoundStateID == 2)
    {
        if (obj_check_anim_frame(11))
        {
            PlaySound2(SOUND_GENERAL_SWISHWATER);
        }

        if (func_802F932C(0))
        {
            if (o->oAnimState != KLEPTO_ANIM_STATE_HOLDING_NOTHING)
            {
                o->oAction = KLEPTO_ACT_CIRCLE_TARGET_HOLDING;
                o->oKleptoTimeUntilTargetChange = 0;
            }
            else
            {
                o->oAction = KLEPTO_ACT_WAIT_FOR_MARIO;
            }
        }
    }
    else
    {
        f32 dy = o->oPosY - gMarioObject->oPosY;
        if (o->oSoundStateID == 3)
        {
            func_802F932C(4);
        }
        else if (o->oVelY > 0.0f && dy > 200.0f)
        {
            func_8029ED38(2);
        }

        o->oKleptoUnk1B0 = -0x3000;
        if (o->oAnimState == KLEPTO_ANIM_STATE_HOLDING_NOTHING)
        {
            if (o->oSubAction == 0)
            {
                o->oKleptoUnk1B0 = obj_turn_pitch_toward_mario(0.0f, 0);
                o->oKleptoYawToTarget = o->oAngleToMario;

                if (dy < 160.0f)
                {
                    o->oSubAction += 1;
                }
            }

            if (gMarioStates[0].action != ACT_SLEEPING &&
                !(gMarioStates[0].action & (ACT_FLAG_SHORT_HITBOX | ACT_FLAG_BUTT_OR_STOMACH_SLIDE)) &&
                o->oDistanceToMario < 200.0f &&
                dy > 50.0f &&
                dy < 90.0f)
            {
                if (mario_lose_cap_to_enemy(1))
                {
                    o->oAnimState = KLEPTO_ANIM_STATE_HOLDING_CAP;
                }
            }
        }
    }

    obj_move_pitch_approach(o->oKleptoUnk1B0, 600);
    obj_face_pitch_approach(o->oMoveAnglePitch, 600);
    obj_rotate_yaw_and_bounce_off_walls(o->oKleptoYawToTarget, 600);
}

static void klepto_act_struck_by_mario(void)
{
    func_8029ED38(1);

    obj_face_pitch_approach(0, 800);
    obj_face_yaw_approach(o->oMoveAngleYaw + 0x8000, 800);
    obj_face_roll_approach(0, 800);

    if (func_8029F788())
    {
        o->oAction = KLEPTO_ACT_RETREAT;
        o->oGravity = 0.0f;

        o->oMoveAnglePitch = -obj_get_pitch_from_vel();
        o->oKleptoSpeed = sqrtf(o->oForwardVel * o->oForwardVel + o->oVelY * o->oVelY);

        o->oHomeX = o->oPosX;
        o->oHomeY = o->oPosY + 500.0f;
        o->oHomeZ = o->oPosZ;
    }
}

static void klepto_act_retreat(void)
{
    func_8029ED38(0);
    approach_f32_ptr(&o->oKleptoSpeed, 40.0f, 10.0f);

    obj_move_pitch_approach(o->oKleptoUnk1B0, 1000);

    obj_face_pitch_approach(o->oMoveAnglePitch, 1000);
    obj_rotate_yaw_and_bounce_off_walls(o->oKleptoYawToTarget, 600);

    if (obj_face_yaw_approach(o->oMoveAngleYaw, 1000))
    {
        if (abs_angle_diff(o->oFaceAnglePitch, o->oMoveAnglePitch) == 0)
        {
            o->oAction = KLEPTO_ACT_RESET_POSITION;
            o->oHomeY = 1500.0f;
            o->oKleptoUnk1AE = -100;
            o->oFlags |= 0x00000008;
            obj_become_tangible();
        }
    }
}

static void klepto_act_reset_position(void)
{
    if (o->oTimer < 300)
    {
        klepto_circle_target(300.0f, 20.0f);
    }
    else if (o->oBehParams2ndByte != 0)
    {
        o->oHomeX = -2000.0f;
        o->oHomeZ = -1000.0f;
        o->oHomeY = o->oKleptoDistanceToTarget = 9999.0f;

        if (o->oPosY > 5000.0f)
        {
            mark_object_for_deletion(o);
        }
        else
        {
            klepto_approach_target(20.0f);
        }
    }
    else
    {
        o->oAction = KLEPTO_ACT_WAIT_FOR_MARIO;
        o->oHomeX = o->oKleptoStartPosX;
        o->oHomeY = o->oKleptoStartPosY;
        o->oHomeZ = o->oKleptoStartPosZ;
    }
}

void obj_set_speed_to_zero(void)
{
    o->oForwardVel = o->oVelY = 0.0f;
}

void bhv_klepto_update(void)
{
    UNUSED s32 unused;

    obj_update_floor_and_walls();

    o->oKleptoDistanceToTarget = obj_lateral_dist_to_home();
    o->oKleptoUnk1B0 = obj_get_pitch_to_home(o->oKleptoDistanceToTarget);
    o->oKleptoYawToTarget = obj_angle_to_home();

    if (o->oAction == KLEPTO_ACT_STRUCK_BY_MARIO)
    {
        klepto_act_struck_by_mario();
    }
    else
    {
        obj_compute_vel_from_move_pitch(o->oKleptoSpeed);

        switch (o->oAction)
        {
        case KLEPTO_ACT_CIRCLE_TARGET_HOLDING:   klepto_circle_target(300.0f, 40.0f); break;
        case KLEPTO_ACT_APPROACH_TARGET_HOLDING: klepto_approach_target(50.0f);       break;
        case KLEPTO_ACT_WAIT_FOR_MARIO:          klepto_act_wait_for_mario();         break;
        case KLEPTO_ACT_TURN_TOWARD_MARIO:       klepto_act_turn_toward_mario();      break;
        case KLEPTO_ACT_DIVE_AT_MARIO:           klepto_act_dive_at_mario();          break;
        case KLEPTO_ACT_RESET_POSITION:          klepto_act_reset_position();         break;
        case KLEPTO_ACT_RETREAT:                 klepto_act_retreat();                break;
        }

        if (obj_handle_attacks(&sKleptoHitbox, o->oAction, sKleptoAttackHandlers))
        {
            PlaySound2(SOUND_OBJECT_KLEPTO2);
            
            if (o->oAnimState == KLEPTO_ANIM_STATE_HOLDING_CAP)
            {
                save_file_clear_flags(SAVE_FLAG_CAP_ON_KLEPTO);
                spawn_object(o, MODEL_MARIOS_CAP, bhvNormalCap);
            }
            else if (o->oAnimState == KLEPTO_ANIM_STATE_HOLDING_STAR)
            {
                CreateStar(-5550.0f, 300.0f, -930.0f);
            }

            o->oAnimState = KLEPTO_ANIM_STATE_HOLDING_NOTHING;
            o->oAction = KLEPTO_ACT_STRUCK_BY_MARIO;
            o->oGravity = -2.0f;

            o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
            o->oFlags &= ~0x00000008;
            obj_become_intangible();
        }
        else if (gMarioStates[0].action == ACT_SLEEPING ||
            (gMarioStates[0].action & (ACT_FLAG_SHORT_HITBOX | ACT_FLAG_BUTT_OR_STOMACH_SLIDE)))
        {
            obj_become_intangible();
        }
        else
        {
            obj_become_tangible();
        }
    }

    obj_roll_to_match_yaw_turn(o->oKleptoYawToTarget, 0x3000, 600);
    obj_move_standard(78);
}

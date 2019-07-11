
/**
 * Behavior for bhvWiggler and bhvWigglerBodyPart.
 * The bhvWiggler object controls the wiggler's behavior, and physically manifests
 * as the wiggler's head. The bhvWigglerBodyPart objects represent the 3 tail body
 * parts, numbered 1 closest to the head, and 3 at the end of the tail.
 * Processing order is bhvWiggler, then bhvWigglerBodyPart 1, 2, then 3.
 */


/**
 * Hitbox for wiggler's non-head body parts.
 */
static struct ObjectHitbox sWigglerBodyPartHitbox =
{
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 99, // never decreases
    /* numLootCoins:      */ 0,
    /* radius:            */ 20,
    /* height:            */ 20,
    /* hurtboxRadius:     */ 20,
    /* hurtboxHeight:     */ 10,
};

/**
 * Hitbox for wiggler's head.
 */
static struct ObjectHitbox sWigglerHitbox =
{
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 4,
    /* numLootCoins:      */ 0,
    /* radius:            */ 60,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 30,
    /* hurtboxHeight:     */ 40,
};

/**
 * Attack handler for wiggler while in the walking action.
 */
static u8 sWigglerAttackHandlers[] =
{
    /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SPECIAL_WIGGLER_JUMPED_ON,
    /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SPECIAL_WIGGLER_JUMPED_ON,
    /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
};

/**
 * Target speed while walking when wiggler has health 1, 2, 3, and 4.
 */
static f32 sWigglerSpeeds[] = { 2.0f, 40.0f, 30.0f, 16.0f };


/**
 * Update function for bhvWigglerBodyPart.
 * Set object position and angle based on wiggler segment data and avoid falling
 * through the floor.
 * Tangible if the wiggler is not in the shrinking action, but does nothing on
 * attack.
 */
void bhv_wiggler_body_part_update(void)
{
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dxz;
    struct ChainSegment *segment = &o->parentObj->oWigglerSegments[o->oBehParams2ndByte];
    f32 posOffset;

    obj_scale(o->parentObj->header.gfx.scale[0]);

    o->oFaceAnglePitch = segment->pitch;
    o->oFaceAngleYaw = segment->yaw;

    // TODO: What is this for?
    posOffset = -37.5f * o->header.gfx.scale[0];
    dy = posOffset * coss(o->oFaceAnglePitch) - posOffset;
    dxz = posOffset * sins(o->oFaceAnglePitch);
    dx = dxz * sins(o->oFaceAngleYaw);
    dz = dxz * coss(o->oFaceAngleYaw);

    o->oPosX = segment->posX + dx;
    o->oPosY = segment->posY + dy;
    o->oPosZ = segment->posZ + dz;

    if (o->oPosY < o->parentObj->oWigglerFallThroughFloorsHeight)
    {
        //! Since position is recomputed each frame, tilting the wiggler up
        //  while on the ground could cause the tail segments to clip through
        //  the floor
        o->oPosY += -30.0f;
        obj_update_floor_height();
        if (o->oFloorHeight > o->oPosY) // TODO: Check ineq swap
        {
            o->oPosY = o->oFloorHeight;
        }
    }

    segment->posY = o->oPosY;

    // Inherit walking animation speed from wiggler
    func_8029ED98(0, o->parentObj->oWigglerWalkAnimSpeed);
    if (o->parentObj->oWigglerWalkAnimSpeed == 0.0f)
    {
        func_8029F6F0();
    }

    if (o->parentObj->oAction == WIGGLER_ACT_SHRINK)
    {
        obj_become_intangible();
    }
    else
    {
        obj_check_attacks(&sWigglerBodyPartHitbox, o->oAction);
    }
}

/**
 * Initialize the segment data and spawn the body part objects.
 */
static void wiggler_init_segments(void)
{
    s32 i;
    struct ChainSegment *segments;
    struct Object *bodyPart;

    segments = mem_pool_alloc(gObjectMemoryPool, 4 * sizeof(struct ChainSegment));
    if (segments != NULL)
    {
        // Each segment represents the global position and orientation of each
        // object. Segment 0 represents the wiggler's head, and segment i>0
        // represents body part i.
        o->oWigglerSegments = segments;
        for (i = 0; i <= 3; i++)
        {
            chain_segment_init(&segments[i]);

            segments[i].posX = o->oPosX;
            segments[i].posY = o->oPosY;
            segments[i].posZ = o->oPosZ;

            segments[i].pitch = o->oFaceAnglePitch;
            segments[i].yaw = o->oFaceAngleYaw;
        }

        o->header.gfx.unk38.animFrame = -1;

        // Spawn each body part
        for (i = 1; i <= 3; i++)
        {
            bodyPart = spawn_object_relative(
                i,
                0, 0, 0,
                o,
                MODEL_WIGGLER_BODY_PART,
                bhvWigglerBodyPart);
            if (bodyPart != NULL)
            {
                func_8029EE20(bodyPart, wiggler_seg5_anims_0500C874, 0);
                bodyPart->header.gfx.unk38.animFrame = (23 * i) % 26 - 1;
            }
        }

        o->oAction = WIGGLER_ACT_WALK;
        obj_unhide();
    }
}

/**
 * Update the tail to reflect changes in the head's yaw and pitch, and ensure
 * that the distance between parts is exactly the intended distance.
 * Since these positions are completely recomputed each frame, it is not possible
 * for a body part to get stuck on geometry and separate from the rest of the
 * body.
 */
static void wiggler_update_segments(void)
{
    struct ChainSegment *prevBodyPart;
    struct ChainSegment *bodyPart;
    f32 dx;
    f32 dy;
    f32 dz;
    s16 dpitch;
    s16 dyaw;
    f32 dxz;
    s32 i;
    f32 segmentLength;

    segmentLength = 35.0f * o->header.gfx.scale[0];

    for (i = 1; i <= 3; i++)
    {
        prevBodyPart = &o->oWigglerSegments[i - 1];
        bodyPart = &o->oWigglerSegments[i];

        dx = bodyPart->posX - prevBodyPart->posX;
        dy = bodyPart->posY - prevBodyPart->posY;
        dz = bodyPart->posZ - prevBodyPart->posZ;

        // As the head turns, propagate this rotation backward if the difference
        // is more than 45 degrees
        dyaw = atan2s(-dz, -dx) - prevBodyPart->yaw;
        clamp_s16(&dyaw, -0x2000, 0x2000);
        bodyPart->yaw = prevBodyPart->yaw + dyaw;

        // As the head tilts, propagate the tilt backward
        dxz = sqrtf(dx*dx + dz*dz);
        dpitch = atan2s(dxz, dy) - prevBodyPart->pitch;
        clamp_s16(&dpitch, -0x2000, 0x2000);
        bodyPart->pitch = prevBodyPart->pitch + dpitch;

        // Set the body part's position relative to the previous body part's
        // position, using the current body part's angles. This means that the
        // head can rotate up to 45 degrees without the body moving
        bodyPart->posY = segmentLength * sins(bodyPart->pitch) + prevBodyPart->posY;
        dxz = segmentLength * coss(bodyPart->pitch);
        bodyPart->posX = prevBodyPart->posX - dxz * sins(bodyPart->yaw);
        bodyPart->posZ = prevBodyPart->posZ - dxz * coss(bodyPart->yaw);
    }
}

/**
 * Show text if necessary. Then walk toward mario if not at full health, and
 * otherwise wander in random directions.
 * If attacked by mario, enter either the jumped on or knockback action.
 */
static void wiggler_act_walk(void)
{
    s16 yawTurnSpeed;

    o->oWigglerWalkAnimSpeed = 0.06f * o->oForwardVel;

    // Update text if necessary
    if (o->oWigglerTextStatus < WIGGLER_TEXT_STATUS_COMPLETED_DIALOGUE)
    {
        if (o->oWigglerTextStatus == WIGGLER_TEXT_STATUS_AWAIT_DIALOGUE)
        {
            func_8031FFB4(0, 60, 40);
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_SHOWING_DIALOGUE;
        }

        // If Mario is positioned below the wiggler, assume he entered through the
        // lower cave entrance, so don't display text.
        if (gMarioObject->oPosY < o->oPosY ||
            obj_update_dialogue_unk2(2, 0, 0xA2, 0x96) != 0)
        {
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_COMPLETED_DIALOGUE;
        }
    }
    else
    {
        //! Every object's health is initially 2048, and wiggler's doesn't change
        //  to 4 until after this runs the first time. It indexes out of bounds
        //  and uses the value 113762.3 for one frame on US.
        obj_forward_vel_approach(sWigglerSpeeds[o->oHealth - 1], 1.0f);

        if (o->oWigglerWalkAwayFromWallTimer != 0)
        {
            o->oWigglerWalkAwayFromWallTimer -= 1;
        }
        else
        {
            if (o->oDistanceToMario >= 25000.0f)
            {
                // If >1200 away from home, turn to home
                o->oWigglerTargetYaw = o->oAngleToMario;
            }

            if (obj_bounce_off_walls_edges_objects(&o->oWigglerTargetYaw))
            {
                //! If the wiggler could self-intersect, or intersect a different
                //  non-mario object, this could potentially be used to force
                //  the wiggler to walk straight - past his usual radius
                o->oWigglerWalkAwayFromWallTimer = random_linear_offset(30, 30);
            }
            else
            {
                if (o->oHealth < 4)
                {
                    o->oWigglerTargetYaw = o->oAngleToMario;
                }
                else if (o->oWigglerTimeUntilRandomTurn != 0)
                {
                    o->oWigglerTimeUntilRandomTurn -= 1;
                }
                else
                {
                    o->oWigglerTargetYaw = o->oMoveAngleYaw + 0x4000 * (s16)RandomSign();
                    o->oWigglerTimeUntilRandomTurn = random_linear_offset(30, 50);
                }
            }
        }

        // If moving at high speeds, could overflow. But can't reach such speeds
        // in practice
        yawTurnSpeed = (s16)(30.0f * o->oForwardVel);
        obj_rotate_yaw_toward(o->oWigglerTargetYaw, yawTurnSpeed);
        obj_face_yaw_approach(o->oMoveAngleYaw, 2 * yawTurnSpeed);

        obj_face_pitch_approach(0, 0x320);

        // For the first two seconds of walking, stay invulnerable
        if (o->oTimer < 60)
        {
            obj_check_attacks(&sWigglerHitbox, o->oAction);
        }
        else if (obj_handle_attacks(&sWigglerHitbox, o->oAction, sWigglerAttackHandlers))
        {
            if (o->oAction != WIGGLER_ACT_JUMPED_ON)
            {
                o->oAction = WIGGLER_ACT_KNOCKBACK;
            }

            o->oWigglerWalkAwayFromWallTimer = 0;
            o->oWigglerWalkAnimSpeed = 0.0f;
        }
    }
}

/**
 * Squish and unsquish, then show text and enter either the walking or shrinking
 * action.
 */
static void wiggler_act_jumped_on(void)
{
    // Text to show on first, second, and third attack.
    s32 attackText[3] = { 0x98, 0xA8, 0x97 };

    // Shrink until the squish speed becomes 0, then unisquish
    if (approach_f32_ptr(&o->oWigglerSquishSpeed, 0.0f, 0.05f))
    {
        // Note that 4 is the default scale
        approach_f32_ptr(&o->header.gfx.scale[1], 4.0f, 0.2f);
    }
    else
    {
        o->header.gfx.scale[1] -= o->oWigglerSquishSpeed;
    }

    // Wait for a second after unsquishing, then show text and either shrink (if
    // defeated) or go back to walking
    if (o->header.gfx.scale[1] >= 4.0f)
    {
        if (o->oTimer > 30)
        {
            if (obj_update_dialogue_unk2(
                2, 0, 0xA2, attackText[o->oHealth - 2]) != 0)
            {
                // Because we don't want the wiggler to disappear after being
                // defeated, we leave its health at 1
                if (--o->oHealth == 1)
                {
                    o->oAction = WIGGLER_ACT_SHRINK;
                    obj_become_intangible();
                }
                else
                {
                    o->oAction = WIGGLER_ACT_WALK;
                    o->oMoveAngleYaw = o->oFaceAngleYaw;

                    if (o->oHealth == 2)
                    {
                        // Jump out of anger (or fear? I'm not a psychologist)
                        PlaySound2(SOUND_OBJECT_WIGGLERUNK2F);
                        o->oForwardVel = 10.0f;
                        o->oVelY = 70.0f;
                    }
                }
            }
        }
    }
    else
    {
        o->oTimer = 0;
    }

    obj_check_attacks(&sWigglerHitbox, o->oAction);
}

/**
 * Decelerate to a stop and then enter the walk action.
 */
static void wiggler_act_knockback(void)
{
    if (o->oVelY > 0.0f)
    {
        o->oFaceAnglePitch -= o->oVelY * 30.0f;
    }
    else
    {
        obj_face_pitch_approach(0, 0x190);
    }

    if (obj_forward_vel_approach(0.0f, 1.0f) && o->oFaceAnglePitch == 0)
    {
        o->oAction = WIGGLER_ACT_WALK;
        o->oMoveAngleYaw = o->oFaceAngleYaw;
    }

    obj_check_attacks(&sWigglerHitbox, o->oAction);
}

/**
 * Shrink, then spawn the star and enter the fall through floor action.
 */
static void wiggler_act_shrink(void)
{
    if (o->oTimer >= 20)
    {
        if (o->oTimer == 20)
        {
            PlaySound2(SOUND_OBJECT_ENEMYDEFEATSHRINK);
        }

        // 4 is the default scale, so shrink to 1/4 of regular size
        if (approach_f32_ptr(&o->header.gfx.scale[0], 1.0f, 0.1f))
        {
            CreateStar(0.0f, 2048.0f, 0.0f);
            o->oAction = WIGGLER_ACT_FALL_THROUGH_FLOOR;
        }

        obj_scale(o->header.gfx.scale[0]);
    }
}

/**
 * Fall through floors until y < 1700, then enter the walking action.
 */
static void wiggler_act_fall_through_floor(void)
{
    if (o->oTimer == 60)
    {
        func_80320CE8(0x416);
        o->oWigglerFallThroughFloorsHeight = 1700.0f;
    }
    else if (o->oTimer > 60)
    {
        if (o->oPosY < o->oWigglerFallThroughFloorsHeight)
        {
            o->oAction = WIGGLER_ACT_WALK;
        }
        else
        {
            o->oFaceAnglePitch = obj_get_pitch_from_vel();
        }

        obj_move_using_fvel_and_gravity();
    }
}

/**
 * Attack handler for when wiggler is jumped or ground pounded on.
 * Stop and enter the jumped on action.
 */
void wiggler_jumped_on_attack_handler(void)
{
    PlaySound2(SOUND_OBJECT_WIGGLERATTACK);
    o->oAction = WIGGLER_ACT_JUMPED_ON;
    o->oForwardVel = o->oVelY = 0.0f;
    o->oWigglerSquishSpeed = 0.4f;
}

/**
 * Update function for bhvWiggler.
 */
void bhv_wiggler_update(void)
{
    //PARTIAL_UPDATE

    if (o->oAction == WIGGLER_ACT_UNINITIALIZED)
    {
        wiggler_init_segments();
    }
    else
    {
        if (o->oAction == WIGGLER_ACT_FALL_THROUGH_FLOOR)
        {
            wiggler_act_fall_through_floor();
        }
        else
        {
            treat_far_home_as_mario(1200.0f);

            // Walking animation and sound
            func_8029ED98(0, o->oWigglerWalkAnimSpeed);
            if (o->oWigglerWalkAnimSpeed != 0.0f)
            {
                func_802F9378(
                    0,
                    13,
                    o->oHealth >= 4
                        ? SOUND_OBJECT_WIGGLERWALK
                        : SOUND_OBJECT_WIGGLER);
            }
            else
            {
                func_8029F6F0();
            }

            obj_update_floor_and_walls();
            switch (o->oAction)
            {
            case WIGGLER_ACT_WALK:               wiggler_act_walk();               break;
            case WIGGLER_ACT_KNOCKBACK:          wiggler_act_knockback();          break;
            case WIGGLER_ACT_JUMPED_ON:          wiggler_act_jumped_on();          break;
            case WIGGLER_ACT_SHRINK:             wiggler_act_shrink();             break;
            case WIGGLER_ACT_FALL_THROUGH_FLOOR: wiggler_act_fall_through_floor(); break;
            }

            obj_move_standard(-78);
        }

        // Update segment 0 with data from the wiggler object
        o->oWigglerSegments[0].posX = o->oPosX;
        o->oWigglerSegments[0].posY = o->oPosY;
        o->oWigglerSegments[0].posZ = o->oPosZ;
        o->oWigglerSegments[0].pitch = o->oFaceAnglePitch;
        o->oWigglerSegments[0].yaw = o->oFaceAngleYaw;

        // Update the rest of the segments to follow segment 0
        wiggler_update_segments();
    }
}

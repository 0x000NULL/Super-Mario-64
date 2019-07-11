// amp.c.inc

static struct ObjectHitbox sAmpHitbox =
{
    /* interactType:      */ INTERACT_SHOCK,
    /* downOffset:        */ 40,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 40,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 50,
    /* hurtboxHeight:     */ 60,
};

void bhv_amp_homing_init(void)
{
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oGravity = 0;
    o->oFriction = 1.0;
    o->oBuoyancy = 1.0;
    o->oAmpHomingAvgY = o->oHomeY;

    obj_scale(0.1f);
    o->header.gfx.node.flags |= 0x10; /* bit 4 */
}

void CheckAmpAttack(void)
{
    set_object_hitbox(o, &sAmpHitbox);
    if (o->oInteractStatus & INT_STATUS_INTERACTED) /* bit 15 */
    {
        if (o->oInteractStatus & INT_STATUS_INTERACTED) /* bit 15 */
            o->oAction = AMP_HOMING_ACT_ATTACK_COOLDOWN;
            
        o->oInteractStatus = 0;
    }
}

void AmpHomingAppearLoop(void)
{
    f32 sp24 = gCameraStatus.camFocAndPosCurrAndGoal[3][0] - o->oPosX;
    f32 sp20 = gCameraStatus.camFocAndPosCurrAndGoal[3][2] - o->oPosZ;
    s16 targetYaw = atan2s(sp20, sp24);
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, targetYaw, 0x1000);
    
    if (o->oTimer < 30)
        obj_scale(0.1 + 0.9 * (f32)(o->oTimer / 30.0f));
    else o->oAnimState = 1;
    
    if (o->oTimer >= 91)
    {
        obj_scale(1.0f);
        o->oAction = AMP_HOMING_ACT_CHASE;
        o->oAmpHomingYPhase = 0;
    }
}

void AmpHomingChaseLoop(void)
{
    if ((o->oAngleToMario - 0x400 < o->oMoveAngleYaw)
    &&  (o->oMoveAngleYaw < o->oAngleToMario + 0x400))
    {
        o->oAmpHomingLockedOn = 1;
        o->oTimer = 0;
    }

    if (o->oAmpHomingLockedOn == 1)
    {
        o->oForwardVel = 15.0f;

        if (o->oAmpHomingAvgY > gMarioObject->header.gfx.pos[1] + 150.0f)
            o->oAmpHomingAvgY -= 10.0f;
        else o->oAmpHomingAvgY = gMarioObject->header.gfx.pos[1] + 150.0f;

        if (o->oTimer >= 31) o->oAmpHomingLockedOn = 0;
    }
    else
    {
        o->oForwardVel = 10.0f;

        obj_turn_toward_object(o, gMarioObject, 16, 0x400);

        if (o->oAmpHomingAvgY < gMarioObject->header.gfx.pos[1] + 250.0f)
            o->oAmpHomingAvgY += 10.0f;
    }

    o->oPosY = o->oAmpHomingAvgY + sins(o->oAmpHomingYPhase * 0x400) * 20.0f;

    CheckAmpAttack();

    if (IsPointCloseToMario(o->oHomeX, o->oHomeY, o->oHomeZ, 1500) == 0)
        o->oAction = AMP_HOMING_ACT_GIVE_UP;
}

void AmpHomingGiveUpLoop(void)
{
    UNUSED u8 filler[8];
    
    o->oForwardVel = 15.0f;
    
    if (o->oTimer >= 151)
    {
        o->oPosX = o->oHomeX;
        o->oPosY = o->oHomeY;
        o->oPosZ = o->oHomeZ;
        o->header.gfx.node.flags |= 0x10; /* bit 4 */
        o->oAction = AMP_HOMING_ACT_INACTIVE;
        o->oAnimState = 0;
        o->oForwardVel = 0;
        o->oAmpHomingAvgY = o->oHomeY;
    }
}

void AmpAttackCooldownLoop(void)
{
    o->header.gfx.unk38.animFrame += 2;
    o->oForwardVel = 0;

    obj_become_intangible();

    if (o->oTimer >= 31) o->oAnimState = 0;
    if (o->oTimer >= 91)
    {
        o->oAnimState = 1;
        obj_become_tangible();
        o->oAction = AMP_HOMING_ACT_CHASE;
    }
}

void bhv_amp_homing_loop(void)
{
    switch (o->oAction)
    {
        case AMP_HOMING_ACT_INACTIVE:
            if (IsPointCloseToMario(o->oHomeX, o->oHomeY, o->oHomeZ, 800) == 1)
            {
                o->oAction = AMP_HOMING_ACT_APPEAR;
                o->header.gfx.node.flags &= ~0x10; /* bit 4 */
            }
            break;

        case AMP_HOMING_ACT_APPEAR:
            AmpHomingAppearLoop();
            break;

        case AMP_HOMING_ACT_CHASE:
            AmpHomingChaseLoop();
            PlaySound(SOUND_CH6_UNKNOWN003);
            break;
            
        case AMP_HOMING_ACT_GIVE_UP:
            AmpHomingGiveUpLoop();
            break;
            
        case AMP_HOMING_ACT_ATTACK_COOLDOWN:
            AmpAttackCooldownLoop();
            break;
    }
    
    ObjectStep();
    
    o->oAmpHomingYPhase++;
}

void bhv_amp_init(void)
{
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oAnimState = 1;
    
    switch (o->oBehParams2ndByte)
    {
        case AMP_BP_ROT_RADIUS_200:
            o->oAmpRadiusOfRotation = 200.0f;
            break;
            
        case AMP_BP_ROT_RADIUS_300:
            o->oAmpRadiusOfRotation = 300.0f;
            break;
            
        case AMP_BP_ROT_RADIUS_400:
            o->oAmpRadiusOfRotation = 400.0f;
            break;
            
        case AMP_BP_ROT_RADIUS_0:
            break;
    }
    
    o->oMoveAngleYaw = RandomU16();
    o->oAction = 2;
}

void FixedAmpIdleLoop(void)
{
    f32 xToMario = gMarioObject->header.gfx.pos[0] - o->oPosX;
    f32 yToMario = gMarioObject->header.gfx.pos[1] + 120.0f - o->oPosY;
    f32 zToMario = gMarioObject->header.gfx.pos[2] - o->oPosZ;
    s16 vAngleToMario = atan2s(sqrtf(xToMario * xToMario + zToMario * zToMario), -yToMario);

    obj_turn_toward_object(o, gMarioObject, 19, 0x1000);
    o->oFaceAnglePitch = approach_s16_symmetric(o->oFaceAnglePitch, vAngleToMario, 0x1000);

    o->oPosY = o->oHomeY + coss(o->oAmpYPhase * 0x458) * 20.0f;

    CheckAmpAttack();

    o->oAmpYPhase++;
}

void AmpIdleLoop(void)
{
    o->oPosX = o->oHomeX + sins(o->oMoveAngleYaw) * o->oAmpRadiusOfRotation;
    o->oPosZ = o->oHomeZ + coss(o->oMoveAngleYaw) * o->oAmpRadiusOfRotation;
    o->oPosY = o->oHomeY + coss(o->oAmpYPhase * 0x8B0) * 30.0f;
    o->oMoveAngleYaw += 0x400;
    o->oFaceAngleYaw = o->oMoveAngleYaw + 0x4000;
    
    CheckAmpAttack();
    
    o->oAmpYPhase++;
    
    PlaySound(SOUND_CH6_UNKNOWN003);
}

void bhv_amp_loop(void)
{
    switch (o->oAction)
    {
        case AMP_ACT_IDLE:
            if (o->oBehParams2ndByte == AMP_BP_ROT_RADIUS_0) FixedAmpIdleLoop();
            else AmpIdleLoop();
            break;

        case AMP_ATTACK_COOLDOWN:
            AmpAttackCooldownLoop();
            break;
    }
}


struct ObjectHitbox sRecoveryHeartHitbox =
{
    /* interactType:      */ 0,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 50,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 50,
    /* hurtboxHeight:     */ 50,
};

void bhv_recovery_heart_loop(void)
{
    set_object_hitbox(o, &sRecoveryHeartHitbox);
    if (are_objects_collided(o, gMarioObject))
    {
        if (o->oSpinningHeartPlayedSound == 0)
        {
            PlaySound2(SOUND_GENERAL_HEARTSPIN);
            o->oSpinningHeartPlayedSound += 1;
        }

        o->oAngleVelYaw = (s32)(200.0f * gMarioStates[0].forwardVel) + 1000;
    }
    else
    {
        o->oSpinningHeartPlayedSound = 0;

        if ((o->oAngleVelYaw -= 50) < 400)
        {
            o->oAngleVelYaw = 400;
            o->oSpinningHeartTotalSpin = 0;
        }
    }

    if ((o->oSpinningHeartTotalSpin += o->oAngleVelYaw) >= 0x10000)
    {
        gMarioStates[0].healCounter += 4;
        o->oSpinningHeartTotalSpin -= 0x10000;
    }

    o->oFaceAngleYaw += o->oAngleVelYaw;
}


static struct ObjectHitbox sMadPianoHitbox =
{
    /* interactType:      */ INTERACT_MR_BLIZZARD,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 99,
    /* numLootCoins:      */ 0,
    /* radius:            */ 200,
    /* height:            */ 150,
    /* hurtboxRadius:     */ 200,
    /* hurtboxHeight:     */ 150,
};

static void mad_piano_act_wait(void)
{
    func_8029ED38(0);

    if (o->oDistanceToMario < 500.0f)
    {
        if (o->oTimer > 20)
        {
            if (gMarioStates[0].forwardVel > 10.0f)
            {
                o->oAction = MAD_PIANO_ACT_ATTACK;
                obj_become_tangible();
            }
        }
    }
    else
    {
        o->oTimer = 0;
    }

    obj_push_mario_away_from_cylinder(280.0f, 150.0f);
}

static void mad_piano_act_attack(void)
{
    obj_update_floor_and_walls();
    func_8029ED38(1);
    func_802F9378(0, 0, SOUND_OBJECT_MADPIANOCHOMPING);
    
    if (o->oDistanceToMario < 500.0f)
    {
        o->oTimer = 0;
    }
    
    if (o->oTimer > 80 && func_8029F788())
    {
        o->oAction = MAD_PIANO_ACT_WAIT;
        o->oForwardVel = 0.0f;
        obj_become_intangible();
    }
    else
    {
        f32 dx = o->oPosX - o->oHomeX;
        f32 dz = o->oPosZ - o->oHomeZ;
        f32 distToHome = sqrtf(dx*dx + dz*dz);

        if (distToHome > 400.0f)
        {
            distToHome = 400.0f / distToHome;
            o->oPosX = o->oHomeX + dx * distToHome;
            o->oPosZ = o->oHomeZ + dz * distToHome;
        }

        obj_rotate_yaw_toward(o->oAngleToMario, 400);
        o->oForwardVel = 5.0f;
    }

    obj_check_attacks(&sMadPianoHitbox, o->oAction);
    obj_move_standard(78);
}

void bhv_mad_piano_update(void)
{
    if (!(o->activeFlags & 0x0008))
    {
        o->oFaceAngleYaw = o->oMoveAngleYaw - 0x4000;

        switch (o->oAction)
        {
        case MAD_PIANO_ACT_WAIT:   mad_piano_act_wait();   break;
        case MAD_PIANO_ACT_ATTACK: mad_piano_act_attack(); break;
        }
    }
}

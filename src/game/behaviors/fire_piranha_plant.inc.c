struct ObjectHitbox sFirePiranhaPlantHitbox =
{
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 1,
    /* radius:            */ 80,
    /* height:            */ 160,
    /* hurtboxRadius:     */ 50,
    /* hurtboxHeight:     */ 150,
};

f32 D_80331B5C[] =
{
    0.5f,
    2.0f,
};

struct ObjectHitbox sPiranhaPlantFireHitbox =
{
    /* interactType:      */ INTERACT_FLAME,
    /* downOffset:        */ 10,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 10,
    /* height:            */ 20,
    /* hurtboxRadius:     */ 10,
    /* hurtboxHeight:     */ 20,
};

s32 sNumActiveFirePiranhaPlants;
s32 sNumKilledFirePiranhaPlants;

void bhv_fire_piranha_plant_init(void)
{
    o->oFirePiranhaPlantNeutralScale = D_80331B5C[(u16)(o->oBehParams >> 16)];
    set_object_hitbox(o, &sFirePiranhaPlantHitbox);

    if ((u16)(o->oBehParams >> 16) != 0)
    {
        o->oFlags |= 0x00004000;
        o->oHealth = 1;

        if (o->oBehParams & 0x0000FF00)
        {
            o->oNumLootCoins = 0;
        }
        else
        {
            o->oNumLootCoins = 2;
        }
    }

    sNumKilledFirePiranhaPlants = 0;
    sNumActiveFirePiranhaPlants = 0;
}

static void fire_piranha_plant_act_hide(void)
{
    if (o->oFirePiranhaPlantDeathSpinTimer != 0)
    {
        o->oMoveAngleYaw += (s32)o->oFirePiranhaPlantDeathSpinVel;
        approach_f32_ptr(&o->oFirePiranhaPlantDeathSpinVel, 0.0f, 200.0f);

        if (func_8029F788())
        {
            if (--o->oFirePiranhaPlantDeathSpinTimer == 0)
            {
                PlaySound2(SOUND_OBJECT_ENEMYDEFEATSHRINK);
            }
        }
    }
    else if (approach_f32_ptr(
        &o->oFirePiranhaPlantScale,
        0.0f,
        0.04f * o->oFirePiranhaPlantNeutralScale))
    {
        obj_become_intangible();
        if (o->oFirePiranhaPlantActive)
        {
            sNumActiveFirePiranhaPlants -= 1;
            o->oFirePiranhaPlantActive = FALSE;

            if ((u16)(o->oBehParams >> 16) != 0 && o->oHealth == 0)
            {
                if (++sNumKilledFirePiranhaPlants == 5)
                {
                    CreateStar(-6300.0f, -1850.0f, -6300.0f);
                }

                obj_die_if_health_non_positive();
                set_object_respawn_info_bits(o, 1);
            }
        }
        else if (sNumActiveFirePiranhaPlants < 2 &&
            o->oTimer > 100 &&
            o->oDistanceToMario > 100.0f && o->oDistanceToMario < 800.0f)
        {
            PlaySound2(SOUND_OBJECT_PIRANHAPLANTAPPEAR);

            o->oFirePiranhaPlantActive = TRUE;
            sNumActiveFirePiranhaPlants += 1;

            obj_unhide();
            o->oAction = FIRE_PIRANHA_PLANT_ACT_GROW;
            o->oMoveAngleYaw = o->oAngleToMario;
        }
        else
        {
            obj_hide();
        }
    }

    func_8029F728();
}

static void fire_piranha_plant_act_grow(void)
{
    func_802F927C(4);

    if (approach_f32_ptr(
        &o->oFirePiranhaPlantScale,
        o->oFirePiranhaPlantNeutralScale,
        0.04f * o->oFirePiranhaPlantNeutralScale))
    {
        if (o->oTimer > 80)
        {
            PlaySound2(SOUND_OBJECT_PIRANHAPLANTSHRINK);
            o->oAction = FIRE_PIRANHA_PLANT_ACT_HIDE;
            func_8029ED38(0);
        }
        else if (o->oTimer < 50)
        {
            obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
        }
        else
        { // TODO: Check if we can put these conditionals on same line
            if (obj_is_rendering_enabled())
            {
                if (obj_check_anim_frame(56))
                {
                    PlaySound2(SOUND_OBJECT_FLAMEBLOWN);
                    obj_spit_fire(
                        0,
                        (s32)(30.0f * o->oFirePiranhaPlantNeutralScale),
                        (s32)(140.0f * o->oFirePiranhaPlantNeutralScale),
                        2.5f * o->oFirePiranhaPlantNeutralScale,
                        MODEL_RED_FLAME_SHADOW,
                        20.0f,
                        15.0f,
                        0x1000);
                }
            }
        }
    }
    else if (o->oFirePiranhaPlantScale > o->oFirePiranhaPlantNeutralScale / 2.0f)
    {
        obj_become_tangible();
    }
}

void bhv_fire_piranha_plant_update(void)
{
    obj_scale(o->oFirePiranhaPlantScale);

    switch (o->oAction)
    {
    case FIRE_PIRANHA_PLANT_ACT_HIDE: fire_piranha_plant_act_hide(); break;
    case FIRE_PIRANHA_PLANT_ACT_GROW: fire_piranha_plant_act_grow(); break;
    }

    if (obj_check_attacks(&sFirePiranhaPlantHitbox, o->oAction))
    {
        if (--o->oHealth < 0)
        {
            if (o->oFirePiranhaPlantActive)
            {
                sNumActiveFirePiranhaPlants -= 1;
            }
        }
        else
        {
            func_8029ED38(2);
        }

        o->oAction = FIRE_PIRANHA_PLANT_ACT_HIDE;
        o->oFirePiranhaPlantDeathSpinTimer = 10;
        o->oFirePiranhaPlantDeathSpinVel = 8000.0f;

        obj_become_intangible();
    }
}

// coin.c.inc

//sp18 = collisionFlagsPtr

static struct ObjectHitbox sMovingYellowCoinHitbox =
{
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

static struct ObjectHitbox sMovingBlueCoinHitbox =
{
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 5,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

s32 CoinStep(s16 *collisionFlagsPtr)
{
    *collisionFlagsPtr = ObjectStep();
    
    ObjCheckFloorDeath(*collisionFlagsPtr, D_803600E0);
    
    if ((*collisionFlagsPtr & 0x1) != 0 && (*collisionFlagsPtr & 0x8) == 0) /* bit 0, bit 3 */
    {
        PlaySound2(SOUND_GENERAL_COINDROP);
        return 1;
    }
    
    return 0;
}

void MovingCoinFlickerLoop(void)
{
    s16 collisionFlags;
    
    CoinStep(&collisionFlags);
    ObjFlickerAndDisappear(o, 0);
}

void CoinCollected(void)
{
    spawn_object(o, MODEL_SPARKLES, bhvGoldenCoinSparkles);
    o->activeFlags = 0;
}

void bhv_moving_yellow_coin_init(void)
{
    o->oGravity = 3.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.5f;
    
    set_object_hitbox(o, &sMovingYellowCoinHitbox);
}

void bhv_moving_yellow_coin_loop(void)
{
    s16 collisionFlags;
    switch (o->oAction)
    {
        case MOV_YCOIN_ACT_IDLE:
            CoinStep(&collisionFlags);
            
            if (o->oTimer < 10) obj_become_intangible();
            else obj_become_tangible();
            
            if (o->oTimer >= 301) o->oAction = 1;
            break;
            
        case MOV_YCOIN_ACT_BLINKING:
            MovingCoinFlickerLoop();
            break;
            
        case MOV_YCOIN_ACT_LAVA_DEATH:
            o->activeFlags = 0;
            break;
            
        case MOV_YCOIN_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = 0;
            break;
    }

    if ((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
    {
        CoinCollected();
        o->oInteractStatus = 0;
    }
}

void bhv_moving_blue_coin_init(void)
{
    o->oGravity = 5.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.5f;
    
    set_object_hitbox(o, &sMovingBlueCoinHitbox);
}

void bhv_moving_blue_coin_loop(void)
{
    s16 collisionFlags;

    switch (o->oAction)
    {
        case MOV_BCOIN_ACT_STILL:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1500) != 0)
                o->oAction = 1;
            break;

        case MOV_BCOIN_ACT_MOVING:
            collisionFlags = ObjectStep();
            if ((collisionFlags & 0x1) != 0) /* bit 0 */
            {
                o->oForwardVel += 25.0f;
                if ((collisionFlags & 0x8) == 0) PlaySound2(SOUND_GENERAL_COINDROP); /* bit 3 */
            }
            else o->oForwardVel *= 0.98;
            
            if (o->oForwardVel > 75.0) o->oForwardVel = 75.0f;
            
            ObjFlickerAndDisappear(o, 600);
            break;
    }
    
    if ((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
    {
        CoinCollected();
        o->oInteractStatus = 0;
    }
}

void bhv_blue_coin_sliding_jumping_init(void)
{
    o->oGravity = 3.0;
    o->oFriction = 0.98;
    o->oBuoyancy = 1.5;
    
    set_object_hitbox(o, &sMovingBlueCoinHitbox);
}

void func_802E540C(void)
{
    s16 collisionFlags;
    
    o->oForwardVel = 15.0;
    o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
    
    if (CoinStep(&collisionFlags) != 0) o->oVelY += 18.0f;
    if ((collisionFlags & 0x2) != 0) o->oAction = 3; /* bit 1 */
    
    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1000) == 0)
        o->oAction = 2;
}

void func_802E54DC(void)
{
    s16 collisionFlags;
    
    CoinStep(&collisionFlags);
    
    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 500) == 1)
        o->oAction = 1;
    
    if (o->oTimer >= 151)
        o->oAction = 3;
}

void bhv_blue_coin_sliding_loop(void)
{
    s16 collisionFlags;

    switch (o->oAction)
    {
        case 0:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 500) == 1)
                o->oAction = 1;
            
            SetObjectVisibility(o, 3000);
            break;
    
        case 1:
            func_802E540C();
            break;
            
        case 2:
            func_802E54DC();
            SetObjectVisibility(o, 3000);
            break;

        case 3:
            CoinStep(&collisionFlags);
            if (o->oTimer >= 61)
                o->oAction = 4;
            break;
            
        case 4:
            MovingCoinFlickerLoop();
            break;
            
        case 100:
            o->activeFlags = 0;
            break;
            
        case 101:
            o->activeFlags = 0;
            break;
    }

    if ((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
    {
        CoinCollected();
        o->oInteractStatus = 0;
    }
}

void bhv_blue_coin_jumping_loop(void)
{
    s16 collisionFlags;

    switch (o->oAction)
    {
        case 0:
            if (o->oTimer == 0)
            {
                obj_become_intangible();
                o->oVelY = 50.0;
            }
            
            ObjectStep();
            
            if (o->oTimer == 15)
            {
                obj_become_tangible();
                o->oAction = 1;
            }
            break;
            
        case 1:
            func_802E540C();
            break;
            
        case 2:
            func_802E54DC();
            SetObjectVisibility(o, 3000);
            break;

        case 3:
            CoinStep(&collisionFlags);
            if (o->oTimer >= 61) o->oAction = 4;
            break;
            
        case 4:
            MovingCoinFlickerLoop();
            break;
    }
    
    if ((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
    {
        CoinCollected();
        o->oInteractStatus = 0;
    }
}

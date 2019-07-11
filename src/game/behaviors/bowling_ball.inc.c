// bowling_ball.c.inc

static struct ObjectHitbox sBowlingBallHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 150,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

// TODO: these are likely Waypoint structs
static s16 D_803315B4[] = { 
    0x0000, 0xED4E, 0x0065, 0xF78A, 
    0x0001, 0xEC78, 0x0051, 0xF53F, 
    0x0002, 0xEC50, 0x0021, 0xF0FA, 
    0x0003, 0xEC9A, 0x0026, 0xEC9A, 
    0x0004, 0xF053, 0xFEFD, 0xECE3, 
    0x0005, 0xF5F3, 0xFC05, 0xED54, 
    0x0006, 0xFBE3, 0xFA89, 0xED3A, 
    0x0007, 0x02F8, 0xF99B, 0xED1F, 
    0x0008, 0x0B32, 0xF801, 0xECEA, 
    0x0009, 0x0D3A, 0xE66E, 0xED1F, 
    0xFFFF, 0x0000
};

// TODO: these are likely Waypoint structs
static s16 D_80331608[] = {
    0x0000, 0xFA3C, 0x001D, 0xFD58, 
    0x0001, 0xFA2C, 0x000E, 0xFBD0, 
    0x0002, 0xFA24, 0x0003, 0xFACD, 
    0x0003, 0xFAA2, 0xFFEF, 0xFA09, 
    0x0004, 0xFB66, 0xFFAD, 0xFA28, 
    0x0005, 0xFEDC, 0xFE58, 0xFA6F, 
    0x0006, 0x00FA, 0xFE15, 0xFA67, 
    0x0007, 0x035E, 0xFD9B, 0xFA57, 
    0x0008, 0x0422, 0xF858, 0xFA57, 
    0xFFFF, 0x0000
};

void bhv_bowling_ball_init(void)
{
    o->oGravity = 5.5f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 2.0f;
}

void func_802EDA14(void)
{
    set_object_hitbox(o, &sBowlingBallHitbox);
    
    if (o->oInteractStatus & INT_STATUS_INTERACTED) o->oInteractStatus = 0;
}

void func_802EDA6C(void)
{
    switch (o->oBehParams2ndByte)
    {
        case BBALL_BP_STYPE_BOB_UPPER:
            o->oBowlingBallUnkFC = segmented_to_virtual(bob_seg7_metal_ball_path0);
            break;
            
        case BBALL_BP_STYPE_TTM:
            o->oBowlingBallUnkFC = segmented_to_virtual(ttm_seg7_trajectory_070170A0);
            break;
            
        case BBALL_BP_STYPE_BOB_LOWER:
            o->oBowlingBallUnkFC = segmented_to_virtual(bob_seg7_metal_ball_path1);
            break;
            
        case BBALL_BP_STYPE_THI_LARGE:
            o->oBowlingBallUnkFC = D_803315B4;
            break;
            
        case BBALL_BP_STYPE_THI_SMALL:
            o->oBowlingBallUnkFC = D_80331608;
            break;
    }
}

void bhv_bowling_ball_roll_loop(void)
{
    s16 collisionFlags;
    s32 sp18;
    
    func_802EDA6C();
    collisionFlags = ObjectStep();
    
    //! Uninitialzed parameter, but the parameter is unused in the called function
    sp18 = obj_follow_path(sp18);
    
    //! oBowlingBallInitYaw is never explicitly initialized, so it is 0.
    o->oBowlingBallTargetYaw = o->oBowlingBallInitYaw;
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oBowlingBallTargetYaw, 0x400);
    if (o->oForwardVel > 70.0)
    {
        o->oForwardVel = 70.0;
    }
    
    func_802EDA14();
    
    if (sp18 == -1)
    {
        if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 7000))
        {
            func_802A3004();
            func_802AA618(0, 0, 92.0f);
        }
        
        o->activeFlags = 0;
    }
    
    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) && (o->oVelY > 5.0f))
        PlaySound2(SOUND_GENERAL_QUIETPOUND1_LOWPRIO);
}

void bhv_bowling_ball_initializeLoop(void)
{
    s32 sp1c;
    
    func_802EDA6C();
    
    //! Uninitialzed parameter, but the parameter is unused in the called function
    sp1c = obj_follow_path(sp1c);
    
    //! oBowlingBallInitYaw is never explicitly initialized, so it is 0.
    o->oMoveAngleYaw = o->oBowlingBallInitYaw;
    
    switch (o->oBehParams2ndByte)
    {
        case BBALL_BP_STYPE_BOB_UPPER:
            o->oForwardVel = 20.0f;
            break;
            
        case BBALL_BP_STYPE_TTM:
            o->oForwardVel = 10.0f;
            break;
            
        case BBALL_BP_STYPE_BOB_LOWER:
            o->oForwardVel = 20.0f;
            break;
            
        case BBALL_BP_STYPE_THI_LARGE:
            o->oForwardVel = 25.0f;
            break;
            
        case BBALL_BP_STYPE_THI_SMALL:
            o->oForwardVel = 10.0f;
            obj_scale(0.3f);
            o->oGraphYOffset = 39.0f;
            break;
    }
}

void bhv_bowling_ball_loop(void)
{
    switch (o->oAction)
    {
        case BBALL_ACT_INITIALIZE:
            o->oAction = BBALL_ACT_ROLL;
            bhv_bowling_ball_initializeLoop();
            break;
            
        case BBALL_ACT_ROLL:
            bhv_bowling_ball_roll_loop();
            break;
    }
    
    if (o->oBehParams2ndByte != 4)
        func_8027F440(4, o->oPosX, o->oPosY, o->oPosZ);
    
    SetObjectVisibility(o, 4000);
}

void bhv_generic_bowling_ball_spawner_init(void)
{
    switch (o->oBehParams2ndByte)
    {
        case BBALL_BP_STYPE_BOB_UPPER:
            o->oBBallSpwnrMaxSpawnDist = 7000.0f;
            o->oBBallSpwnrSpawnOdds = 2.0f;
            break;
            
        case BBALL_BP_STYPE_TTM:
            o->oBBallSpwnrMaxSpawnDist = 8000.0f;
            o->oBBallSpwnrSpawnOdds = 1.0f;
            break;
            
        case BBALL_BP_STYPE_BOB_LOWER:
            o->oBBallSpwnrMaxSpawnDist = 6000.0f;
            o->oBBallSpwnrSpawnOdds = 2.0f;
            break;
    }
}

void bhv_generic_bowling_ball_spawner_loop(void)
{
    struct Object *bowlingBall;
    
    if (o->oTimer == 256)
        o->oTimer = 0;
    
    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 1000)
    ||  (o->oPosY < gMarioObject->header.gfx.pos[1]))
        return;
    
    if ((o->oTimer & o->oBBallSpwnrPeriodMinus1) == 0) /* Modulus */
    {
        if  (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, o->oBBallSpwnrMaxSpawnDist))
        {
            if  ((s32)(RandomFloat() * o->oBBallSpwnrSpawnOdds) == 0)
            {
                bowlingBall = spawn_object(o, MODEL_BOWLING_BALL, bhvBowlingBall);
                bowlingBall->oBehParams2ndByte = o->oBehParams2ndByte;
            }
        }
    }
}

void bhv_thi_bowling_ball_spawner_loop(void)
{
    struct Object *bowlingBall;
    
    if (o->oTimer == 256)
        o->oTimer = 0;
    
    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 800)
    ||  (o->oPosY < gMarioObject->header.gfx.pos[1]))
        return;
        
    if ((o->oTimer % 64) == 0)
    {
        if  (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 12000))
        {
            if  ((s32)(RandomFloat() * 1.5) == 0)
            {
                bowlingBall = spawn_object(o, MODEL_BOWLING_BALL, bhvBowlingBall);
                bowlingBall->oBehParams2ndByte = o->oBehParams2ndByte;
            }
        }
    }
}

void bhv_bob_pit_bowling_ball_init(void)
{
    o->oGravity = 12.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 2.0f;
}

void bhv_bob_pit_bowling_ball_loop(void)
{
    struct FloorGeometry *sp1c;
    UNUSED s16 collisionFlags = ObjectStep();
    
    find_floor_height_and_data(o->oPosX, o->oPosY, o->oPosZ, &sp1c);
    if ((sp1c->normalX == 0) && (sp1c->normalZ == 0))
        o->oForwardVel = 28.0f;
    
    func_802EDA14();
    func_8027F440(4, o->oPosX, o->oPosY, o->oPosZ);
    PlaySound(SOUND_ENVIRONMENT_UNKNOWN2);
    SetObjectVisibility(o, 3000);
}

void bhv_free_bowling_ball_init(void)
{
    o->oGravity = 5.5f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 2.0f;
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oForwardVel = 0;
    o->oMoveAngleYaw = 0;
}

void bhv_free_bowling_ball_roll_loop(void)
{
    s16 collisionFlags = ObjectStep();
    func_802EDA14();
    
    if (o->oForwardVel > 10.0f)
    {
        func_8027F440(4, o->oPosX, o->oPosY, o->oPosZ);
        PlaySound(SOUND_ENVIRONMENT_UNKNOWN2);
    }
    
    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED)
    &&  !(collisionFlags & OBJ_COL_FLAGS_LANDED))
        PlaySound2(SOUND_GENERAL_QUIETPOUND1_LOWPRIO);
        
    if (!IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 6000))
    {
        o->header.gfx.node.flags |= 0x10; /* bit 4 */
        obj_become_intangible();
        
        o->oPosX = o->oHomeX;
        o->oPosY = o->oHomeY;
        o->oPosZ = o->oHomeZ;
        bhv_free_bowling_ball_init();
        o->oAction = FREE_BBALL_ACT_RESET;
    }
}

void bhv_free_bowling_ball_loop(void)
{
    o->oGravity = 5.5f;
    
    switch (o->oAction)
    {
        case FREE_BBALL_ACT_IDLE:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 3000))
            {
                o->oAction = FREE_BBALL_ACT_ROLL;
                o->header.gfx.node.flags &= ~0x10; /* bit 4 */
                obj_become_tangible();
            }
            break;
            
        case FREE_BBALL_ACT_ROLL:
            bhv_free_bowling_ball_roll_loop();
            break;
            
        case FREE_BBALL_ACT_RESET:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 5000))
                o->oAction = FREE_BBALL_ACT_IDLE;
            break;
    }
}

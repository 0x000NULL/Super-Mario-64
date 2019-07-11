// bobomb.c.inc

static struct ObjectHitbox sBobombHitbox =
{
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 65,
    /* height:            */ 113,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_bobomb_init(void)
{
    o->oGravity = 2.5;
    o->oFriction = 0.8;
    o->oBuoyancy = 1.3;
    o->oUnk190 = 0x100;
}

void func_802E5B7C(void)
{
    if (((o->oBehParams >> 8) & 0x1) == 0)
    {
        ObjSpawnYellowCoins(o, 1);
        o->oBehParams = 0x100;
        set_object_respawn_info_bits(o, 1);
    }
}

void BobombExplodeLoop(void)
{
    struct Object *explosion;
    if (o->oTimer < 5) obj_scale(1.0 + (f32)o->oTimer / 5.0);
    else
    {
        explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        explosion->oGraphYOffset += 100.0f;
        
        func_802E5B7C();
        create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
        o->activeFlags = 0;
    }
}

void CheckBobombInteractions(void)
{
    set_object_hitbox(o, &sBobombHitbox);
    if ((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
    {
        if ((o->oInteractStatus & INTERACT_GRABBABLE) != 0) /* bit 1 */
        {
            o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
            o->oForwardVel = 25.0;
            o->oVelY = 30.0;
            o->oAction = BOBOMB_ACT_LAUNCHED;
        }

        if ((o->oInteractStatus & INTERACT_TEXT) != 0) /* bit 23 */
            o->oAction = BOBOMB_ACT_EXPLODE;

        o->oInteractStatus = 0;
    }

    if (attack_collided_non_mario_object(o) == 1) o->oAction = BOBOMB_ACT_EXPLODE;
}

void BobombPatrolLoop(void)
{
    UNUSED s8 filler[4];
    UNUSED s16 sp22;
    s16 collisionFlags;

    sp22 = o->header.gfx.unk38.animFrame;
    o->oForwardVel = 5.0;

    collisionFlags = ObjectStep();
    if ((ObjLeaveIfMarioIsNearHome(o, o->oHomeX, o->oHomeY, o->oHomeZ, 400) == 1)
    &&  (func_802E46C0(o->oMoveAngleYaw, o->oAngleToMario, 0x2000) == 1))
    {
        o->oBobombFuseLit = 1;
        o->oAction = BOBOMB_ACT_CHASE_MARIO;
    }
    ObjCheckFloorDeath(collisionFlags, D_803600E0);
}

void BobombChaseMarioLoop(void)
{
    UNUSED u8 filler[4];
    s16 sp1a, collisionFlags;

    sp1a = ++o->header.gfx.unk38.animFrame;
    o->oForwardVel = 20.0;

    collisionFlags = ObjectStep();

    if (sp1a == 5 || sp1a == 16) PlaySound2(SOUND_OBJECT_BOBOMBWALK);

    obj_turn_toward_object(o, gMarioObject, 16, 0x800);
    ObjCheckFloorDeath(collisionFlags, D_803600E0);
}

void BobombLaunchedLoop(void)
{
    s16 collisionFlags = 0;
    collisionFlags = ObjectStep();
    if ((collisionFlags & 0x1) == 1) o->oAction = BOBOMB_ACT_EXPLODE; /* bit 0 */
}

void GenericBobombFreeLoop(void)
{
    switch (o->oAction)
    {
        case BOBOMB_ACT_PATROL:
            BobombPatrolLoop();
            break;
            
        case BOBOMB_ACT_LAUNCHED:
            BobombLaunchedLoop();
            break;
            
        case BOBOMB_ACT_CHASE_MARIO:
            BobombChaseMarioLoop();
            break;
            
        case BOBOMB_ACT_EXPLODE:
            BobombExplodeLoop();
            break;
            
        case BOBOMB_ACT_LAVA_DEATH:
            if (ObjLavaDeath() == 1) create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
            
        case BOBOMB_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = 0;
            create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
    }
    
    CheckBobombInteractions();
    
    if (o->oBobombFuseTimer >= 151) o->oAction = 3;
}

void StationaryBobombFreeLoop(void)
{
    switch (o->oAction)
    {
        case BOBOMB_ACT_LAUNCHED:
            BobombLaunchedLoop();
            break;
            
        case BOBOMB_ACT_EXPLODE:
            BobombExplodeLoop();
            break;
            
        case BOBOMB_ACT_LAVA_DEATH:
            if (ObjLavaDeath() == 1) create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
            
        case BOBOMB_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = 0;
            create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
    }
    
    CheckBobombInteractions();
    
    if (o->oBobombFuseTimer >= 151) o->oAction = 3;
}

void BobombFreeLoop(void)
{
    if (o->oBehParams2ndByte == BOBOMB_BP_STYPE_GENERIC) GenericBobombFreeLoop();
    else StationaryBobombFreeLoop();
}

void BobombHeldLoop(void)
{
    o->header.gfx.node.flags |= 0x10; /* bit 4 */
    SetObjAnimation(1);
    obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0);

    o->oBobombFuseLit = 1;
    if (o->oBobombFuseTimer >= 151)
    {
        //! Although the Bob-omb's action is set to explode when the fuse timer expires,
        //  BobombExplodeLoop() will not execute until the bob-omb's held state changes. 
        //  This allows the Bob-omb to be regrabbed indefinitely.                        
            
        gMarioObject->oInteractStatus |= INTERACT_DAMAGE; /* bit 3 */
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void BobombDroppedLoop(void)
{
    obj_get_dropped();

    o->header.gfx.node.flags &= ~0x10; /* bit 4 = 0 */
    SetObjAnimation(0);

    o->oHeldState = 0;
    o->oAction = BOBOMB_ACT_PATROL;
}

void BobombThrownLoop(void)
{
    obj_enable_rendering_2();

    o->header.gfx.node.flags &= ~0x10; /* bit 4 = 0 */
    o->oHeldState = 0;
    o->oFlags &= ~0x8; /* bit 3 */
    o->oForwardVel = 25.0;
    o->oVelY = 20.0;
    o->oAction = BOBOMB_ACT_LAUNCHED;
}

//sp18 = blinkTimer

void ObjRandomBlink(s32 *blinkTimer)
{
    if (*blinkTimer == 0)
    {
        if ((s16)(RandomFloat() * 100.0f) == 0)
        {
            o->oAnimState = 1;
            *blinkTimer = 1;
        }
    }
    else
    {
        (*blinkTimer)++;
        if (*blinkTimer >= 6) o->oAnimState = 0;
        if (*blinkTimer >= 11) o->oAnimState = 1;
        if (*blinkTimer >= 16) 
        {
            o->oAnimState = 0;
            *blinkTimer = 0;
        }
    }
}

void bhv_bobomb_loop(void)
{
    s8 dustPeriodMinus1;
    if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 4000) != 0)
    {
        switch (o->oHeldState)
        {
            case HELD_FREE:
                BobombFreeLoop();
                break;
                
            case HELD_HELD:
                BobombHeldLoop();
                break;
                
            case HELD_THROWN:
                BobombThrownLoop();
                break;
                
            case HELD_DROPPED:
                BobombDroppedLoop();
                break;
        }
    
        ObjRandomBlink(&o->oBobombBlinkTimer);
        
        if (o->oBobombFuseLit == 1)
        {
            if (o->oBobombFuseTimer >= 121) dustPeriodMinus1 = 1;
            else dustPeriodMinus1 = 7;
        
            if ((dustPeriodMinus1 & o->oBobombFuseTimer) == 0) /* oBobombFuseTimer % 2 or oBobombFuseTimer % 8 */
                spawn_object(o, MODEL_SMOKE, bhvBobombFuseSmoke);
                
            PlaySound(SOUND_CH6_UNKNOWN008);
            
            o->oBobombFuseTimer++;
        }
    }
}

void bhv_bobomb_fuse_smoke_init(void)
{
    o->oPosX += (s32)(RandomFloat() * 80.0f) - 40;
    o->oPosY += (s32)(RandomFloat() * 80.0f) + 60;
    o->oPosZ += (s32)(RandomFloat() * 80.0f) - 40;
    obj_scale(1.2f);
}

void bhv_bobomb_buddy_init(void)
{
    o->oGravity = 2.5;
    o->oFriction = 0.8;
    o->oBuoyancy = 1.3;
    o->oUnk190 = 0x4000;
}

void BobombBuddyIdleLoop(void)
{
    UNUSED u8 filler[4];
    s16 sp1a = o->header.gfx.unk38.animFrame;
    UNUSED s16 collisionFlags = 0;

    o->oBobombBuddyPosXCopy = o->oPosX;
    o->oBobombBuddyPosYCopy = o->oPosY;
    o->oBobombBuddyPosZCopy = o->oPosZ;

    collisionFlags = ObjectStep();

    if ((sp1a == 5) || (sp1a == 16)) PlaySound2(SOUND_OBJECT_BOBOMBWALK);

    if (o->oDistanceToMario < 1000.0f)
        o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x140);

    if (o->oInteractStatus == INT_STATUS_INTERACTED) o->oAction = BOBOMB_BUDDY_ACT_TURN_TO_TALK;
}

//sp30 = arg0
//sp34 = arg1

void BobombBuddyCannonLoop(s16 arg0, s16 arg1)
{
    struct Object* sp2c;
    s16 sp2a, sp28;
    
    switch (o->oBobombBuddyCannonStatus)
    {
        case BOBOMB_BUDDY_CANNON_UNOPENED:
            sp2a = func_8028F8E0(162, o, arg0);
            if (sp2a != 0)
            {
                save_file_set_cannon_unlocked();
                sp2c = obj_nearest_object_with_behavior(bhvCannonClosed);
                if (sp2c != 0) o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENING;
                else o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_STOP_TALKING;
            }
            break;
            
        case BOBOMB_BUDDY_CANNON_OPENING:
            sp2c = obj_nearest_object_with_behavior(bhvCannonClosed);
            sp28 = func_8028F9E8(150, sp2c);
            if (sp28 == -1) o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENED;
            break;
            
        case BOBOMB_BUDDY_CANNON_OPENED:
            sp2a = func_8028F8E0(162, o, arg1);
            if (sp2a != 0) o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_STOP_TALKING;
            break;
            
        case BOBOMB_BUDDY_CANNON_STOP_TALKING:
            set_mario_npc_dialogue(0);
            
            o->activeFlags &= ~0x20; /* bit 5 */
            o->oBobombBuddyHasTalkedToMario = BOBOMB_BUDDY_HAS_TALKED;
            o->oInteractStatus = 0;
            o->oAction = BOBOMB_BUDDY_ACT_IDLE;
            o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENED;
            break;
    }
}

void BobombBuddyTalkLoop(void)
{
    if (set_mario_npc_dialogue(1) == 2)
    {
        o->activeFlags |= 0x20; /* bit 5 */

        switch (o->oBobombBuddyRole)
        {
            case BOBOMB_BUDDY_ROLE_ADVICE:
                if (func_8028F8E0(162, o, o->oBehParams2ndByte) != BOBOMB_BUDDY_BP_STYPE_GENERIC)
                {
                    set_mario_npc_dialogue(0);

                    o->activeFlags &= ~0x20; /* bit 5 */
                    o->oBobombBuddyHasTalkedToMario = BOBOMB_BUDDY_HAS_TALKED;
                    o->oInteractStatus = 0;
                    o->oAction = BOBOMB_BUDDY_ACT_IDLE;
                }
                break;

            case BOBOMB_BUDDY_ROLE_CANNON:
                if (gCurrCourseNum == COURSE_BOB) BobombBuddyCannonLoop(4, 105);
                else BobombBuddyCannonLoop(47, 106);
                break;
        }
    }
}

void BobombBuddyTurnToTalkLoop(void)
{
    s16 sp1e = o->header.gfx.unk38.animFrame;
    if ((sp1e == 5) || (sp1e == 16)) PlaySound2(SOUND_OBJECT_BOBOMBWALK);
    
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x1000);
    if ((s16)o->oMoveAngleYaw == (s16)o->oAngleToMario)
        o->oAction = BOBOMB_BUDDY_ACT_TALK;
    
    PlaySound2(SOUND_ACTION_UNKNOWN45B);
}

void BobombBuddyActionLoop(void)
{
    switch (o->oAction)
    {
        case BOBOMB_BUDDY_ACT_IDLE:
            BobombBuddyIdleLoop();
            break;
            
        case BOBOMB_BUDDY_ACT_TURN_TO_TALK:
            BobombBuddyTurnToTalkLoop();
            break;
            
        case BOBOMB_BUDDY_ACT_TALK:
            BobombBuddyTalkLoop();
            break;
    }

    SetObjectVisibility(o, 3000);
}

void bhv_bobomb_buddy_loop(void)
{
    BobombBuddyActionLoop();
    
    ObjRandomBlink(&o->oBobombBuddyBlinkTimer);
    
    o->oInteractStatus = 0;
}

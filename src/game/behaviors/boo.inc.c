// boo.c.inc


struct ObjectHitbox sBooGivingStarHitbox =
{
    /* interactType: */      0,
    /* downOffset: */        0,
    /* damageOrCoinValue: */ 3,
    /* health: */            3,
    /* numLootCoins: */      0,
    /* radius: */            140,
    /* height: */            80,
    /* hurtboxRadius: */     40,
    /* hurtboxHeight: */     60,
};

s16 TableCourtyardBooTripletPos[][3] = {{0,50,0},{210,110,210},{-210,70,-210}};

void boo_stop(void)
{
    o->oForwardVel = 0.0f;
    o->oVelY = 0.0f;
    o->oGravity = 0.0f;
}

void bhv_boo_init(void)
{
    o->oBooUnk110 = o->oMoveAngleYaw;
}

s32 func_802C2A94(void)
{
    if(obj_has_behavior(bhvSpawnedBoo) || obj_has_behavior(bhvSpawnedBoo2))
    {
        if(gMarioOnMerryGoRound == 0)
            return 1;
        else
            return 0;
    }
    else
    {
        if(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)
            return 1;
        if(o->oRoom == 10)
            if(gTimeStopState & TIME_STOP_MARIO_OPENED_DOOR)
                return 1;
    }
    return 0;
}

s32 boo_should_be_active(void)
{
    f32 radius;
    if(obj_has_behavior(bhvPatrollingBoo))
        radius = 5000.0f;
    else
        radius = 1500.0f;
    if(obj_has_behavior(bhvSpawnedBoo) || obj_has_behavior(bhvSpawnedBoo2))
    {
        if(gMarioOnMerryGoRound == 1)
            return 1;
        else
            return 0;
    }
    else if(o->oRoom == -1)
    {
        if(o->oDistanceToMario < radius)
            return 1;
    }
    else if(!func_802C2A94())
    {
        if(o->oDistanceToMario < radius && (o->oRoom == gMarioCurrentRoom || gMarioCurrentRoom == 0))
                return 1;
    }
    return 0;
}

void bhv_boo_group_init(void)
{
    s32 i;
    struct Object* sp28;
    if(gDisplayedStars < 12)
        mark_object_for_deletion(o);
    else
    {
        for(i=0;i<3;i++)
        {
            sp28 = spawn_object_relative(
                1,
                TableCourtyardBooTripletPos[i][0],
                TableCourtyardBooTripletPos[i][1],
                TableCourtyardBooTripletPos[i][2],
                o,
                MODEL_BOO,
                bhvBoo2
            );
            sp28->oMoveAngleYaw = RandomU16();
        }
    }
}

void func_802C2D7C(void)
{
    f32 sp1C;
    if(o->oBooUnkF4 != o->oOpacity)
    {
        if(o->oBooUnkF4 > o->oOpacity)
        {
            o->oOpacity += 20;
            if(o->oBooUnkF4 < o->oOpacity)
                o->oOpacity = o->oBooUnkF4;
        }
        else
        {
            o->oOpacity -= 20;
            if(o->oBooUnkF4 > o->oOpacity)
                o->oOpacity = o->oBooUnkF4;
        }
    }
    sp1C = (o->oOpacity/255.0f * 0.4 + 0.6) * o->oBooUnkF8;
    scale_object(o,sp1C);
}

void func_802C2EB8(s32 a0)
{
    o->oFaceAnglePitch = sins(o->oBooUnkFC) * 0x400;
    if(o->oOpacity == 0xFF || a0 == 1)
    {
        o->header.gfx.scale[0] =    sins(o->oBooUnkFC) * 0.08 + o->oBooUnkF8;
        o->header.gfx.scale[1] = -sins(o->oBooUnkFC) * 0.08 + o->oBooUnkF8;
        o->header.gfx.scale[2] = o->header.gfx.scale[0];
        o->oGravity = sins(o->oBooUnkFC) * o->oBooUnkF8;
        o->oBooUnkFC += 0x400;
    }
}

s32 func_802C3008(void)
{
    s16 sp26 = abs_angle_diff(o->oAngleToMario,o->oMoveAngleYaw);
    s16 sp24 = abs_angle_diff(o->oMoveAngleYaw,gMarioObject->oFaceAngleYaw);
    s16 sp22 = 0x1568;
    s16 sp20 = 0x6b58;
    s32 sp1C = 0;
    o->oVelY = 0.0f;
    if(sp26 > sp22 || sp24 < sp20)
    {
        if(o->oOpacity == 40)
        {
            o->oBooUnkF4 = 0xFF;
            PlaySound2(SOUND_OBJECT_BOOLAUGH2);
        }
        if(o->oOpacity > 180)
            sp1C = 1;
    }
    else if(o->oOpacity == 0xFF)
        o->oBooUnkF4 = 40;
    return sp1C;
}

void func_802C313C(s32 a0)
{
    obj_become_intangible();
    o->oFlags &= ~8;
    o->oBooUnk104 = (f32) o->oMoveAngleYaw;
    if(a0 != 0)
        o->oBooUnk100 = gMarioObject->oMoveAngleYaw;
    else if(coss((s16)o->oMoveAngleYaw - (s16)o->oAngleToMario) < 0.0f)
        o->oBooUnk100 = o->oMoveAngleYaw;
    else
        o->oBooUnk100 = (s16)(o->oMoveAngleYaw + 0x8000);
}

void func_802C3238(s32 a0,f32 a1)
{
    s32 sp4 = o->oTimer * 0x800 + 0x800;
    o->oForwardVel = a1;
    o->oVelY = coss(sp4);
    o->oMoveAngleYaw = o->oBooUnk100;
    if(a0 != 0)
    {
        o->oFaceAngleYaw    += D_8032F0CC[o->oTimer];
        o->oFaceAngleRoll += D_8032F0CC[o->oTimer];
    }
}

void func_802C3304(void)
{
    s32 sp4 = o->oTimer * 0x2000 + 0xFFFC2000;
    o->oFaceAngleYaw += coss(sp4) * 0x400;
}

void func_802C3384(void)
{
    o->oMoveAngleYaw = o->oBooUnk104;
    o->oFlags |= 8;
    o->oInteractStatus = 0;
}

s32 func_802C33D0(f32 a0)
{
    boo_stop();
    if(o->oTimer == 0)
        func_802C313C(0);
    if(o->oTimer < 32)
        func_802C3238(0,D_8032F0CC[o->oTimer]/5000.0f*a0);
    else
    {
        obj_become_tangible();
        func_802C3384();
        o->oAction = 1;
        return 1;
    }
    return 0;
}

s32 func_802C34B4(f32 a0)
{
    boo_stop();
    if(o->oTimer == 0)
        func_802C313C(1);
    if(o->oTimer < 32)
        func_802C3238(1,D_8032F0CC[o->oTimer]/5000.0f*a0);
    else if(o->oTimer < 48)
        func_802C3304();
    else
    {
        obj_become_tangible();
        func_802C3384();
        o->oAction = 1;
        return 1;
    }
    return 0;
}

s32 func_802C35C0(void)
{
    s32* sp1C; //FIXME
    if(o->oTimer == 0)
    {
        o->oForwardVel = 40.0f;
        o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw;
        o->oUnk88 = 1;
        o->oFlags &= ~8;
    }
    else
    {
        if(o->oTimer == 5)
            o->oBooUnkF4 = 0;
        if(o->oTimer > 30 || o->oMoveFlags & 0x200)
        {
            func_802A3004();
            o->oUnk88 = 2;
#ifndef VERSION_JP
            if(o->oBooUnk108 != 0)
            {
                sp1C = o->oBooUnk108;
                if(!obj_has_behavior(bhvBoo3))
                {
                    sp1C[107]++;
                }
            }
#else
            if(o->oBooUnk108 != 0)
            {
                sp1C = o->oBooUnk108;
                sp1C[107]++;
            }
#endif
            return 1;
        }
    }
    o->oVelY = 5.0f;
    o->oFaceAngleRoll += 0x800;
    o->oFaceAngleYaw += 0x800;
    return 0;
}

s32 func_802C3738(u32 a0)
{
    if((o->oInteractStatus & INT_STATUS_ATTACK_MASK) == a0)
        return 1;
    else
        return 0;
}

s32 func_802C3778(void)
{
    s32 sp1C = 0;
    if(o->oInteractStatus & INT_STATUS_INTERACTED)
    {
        if(o->oInteractStatus & INT_STATUS_WAS_ATTACKED && func_802C3738(3) == 0)
        {
            obj_become_intangible();
            o->oInteractStatus = 0;
            PlaySound2(SOUND_OBJECT_BOOLAUGH1);
            sp1C = 1;
        }
        else
        {
            PlaySound2(SOUND_OBJECT_UNKNOWN1);
            o->oInteractStatus = 0;
            sp1C = -1;
        }
    }
    return sp1C;
}

void func_802C3830(f32 a0,s16 a1,f32 a2)
{
    f32 sp1C;
    s16 sp1A;
    if(func_802C3008())
    {
        o->oInteractType = 0x8000;
        if(obj_lateral_dist_from_mario_to_home() > 1500.0f)
            sp1A = obj_angle_to_home();
        else
            sp1A = o->oAngleToMario;
        obj_rotate_yaw_toward(sp1A,a1);
        o->oVelY = 0.0f;
        if(mario_is_in_air_action() == 0)
        {
            sp1C = o->oPosY - gMarioObject->oPosY;
            if(a0 < sp1C && sp1C < 500.0f)
                o->oVelY = func_802A0BF4(o->oPosY,gMarioObject->oPosY+50.0f,10.f,2.0f);
        }
        func_8029F684(10.0f-o->oBooUnk10C,a2);
        if(o->oForwardVel != 0.0f)
            func_802C2EB8(0);
    }
    else
    {
        o->oInteractType = 0;
        o->oForwardVel = 0.0f;
        o->oVelY = 0.0f;
        o->oGravity = 0.0f;
    }
}

void ActionBoo0(void)
{
    o->activeFlags |= 0x40;
    if(o->oBehParams2ndByte == 2)
        o->oRoom = 10;
    obj_set_pos_to_home();
    o->oMoveAngleYaw = o->oBooUnk110;
    boo_stop();
    o->oBooUnk108 = obj_nearest_object_with_behavior(bhvBooGivingStar);
    o->oBooUnkF8 = 1.0f;
    o->oBooUnkF4 = 0xFF;
    if(boo_should_be_active())
    {
		// Condition is met if the object is bhvPatrollingBoo or bhvSpawnedBoo2
        if(o->oBehParams2ndByte == 2)
        {
            o->oBooUnk108 = 0;
            o->oAction = 5;
        }
        else
            o->oAction = 1;
    }
}

void ActionBoo5(void)
{
    if(o->oTimer < 30)
    {
        o->oVelY = 0.0f;
        o->oForwardVel = 13.0f;
        func_802C2EB8(0);
        o->oWallHitboxRadius = 0.0f;
    }
    else
    {
        o->oAction = 1;
        o->oWallHitboxRadius = 30.0f;
    }
}

void ActionBoo1(void)
{
    s32 sp1C;
    if(o->oTimer == 0)
    {
        o->oBooUnk10C = -RandomFloat() * 5.0f;
        o->oUnk1B0_S16 = (s32)(RandomFloat() * 128.0f);
    }
    func_802C3830(-100.0f,o->oUnk1B0_S16+0x180,0.5f);
    sp1C = func_802C3778();
    if(func_802C2A94())
        o->oAction = 0;
    if(sp1C == -1)
        o->oAction = 2;
    if(sp1C == 1)
        o->oAction = 3;
    if(sp1C == 1)
        create_sound_spawner(SOUND_OBJECT_DYINGENEMY1);
}

void ActionBoo2(void)
{
    if(func_802C33D0(20.0f))
        o->oAction = 1;
}

void ActionBoo3(void)
{
    if(func_802C35C0())
    {
        if(o->oBehParams2ndByte != 0)
            mark_object_for_deletion(o);
        else
        {
            o->oAction = 4;
            obj_disable();
        }
    }
}

// Called when a Go on a Ghost Hunt boo dies
void ActionBoo4(void)
{
    s32 dialogID;
    
    // If there are no remaining "minion" boos, show the dialog of the Big Boo
    if(obj_nearest_object_with_behavior(bhvBoo2) == NULL)
        dialogID = 108;
    else
        dialogID = 107;
    
    if(obj_update_dialogue_unk1(2,2,dialogID,0))
    {
        create_sound_spawner(SOUND_OBJECT_DYINGENEMY1);
        mark_object_for_deletion(o);
        if(dialogID == 108) // If the Big Boo should spawn, play the jingle
            play_puzzle_jingle();
    }
}

void (*TableBooActions[])(void) = {ActionBoo0,ActionBoo1,ActionBoo2,ActionBoo3,ActionBoo4,ActionBoo5};

void bhv_boo_loop(void)
{
    obj_update_floor_and_walls();
    obj_call_action_function(TableBooActions);
    obj_move_standard(78);
    func_802C2D7C();
    if(object_has_behavior(o->parentObj,bhvSpawnBigBoo))
    {
        if(o->activeFlags == 0)
            o->parentObj->oUnk88++;
    }
    o->oInteractStatus = 0;
}

void ActionBooGivingStar0(void)
{
    if(obj_has_behavior(bhvPatrollingBoo))
    {
        func_802C49E0();
        o->oUnk1AC_S32 = 10;
    }
    o->oBooUnk108 = 0;
#ifndef VERSION_JP
    if(boo_should_be_active() && gDebugInfo[5][0] + 5 <= o->oUnk1AC_S32)
#else
    if(boo_should_be_active() && o->oUnk1AC_S32 >= 5)
#endif
    {
        o->oAction = 1;
        obj_set_pos_to_home();
        o->oMoveAngleYaw = o->oBooUnk110;
        obj_unhide();
        o->oBooUnkF4 = 0xFF;
        o->oBooUnkF8 = 3.0f;
        o->oHealth = 3;
        obj_scale(3.0f);
        obj_become_tangible();
    }
    else
    {
        obj_hide();
        obj_become_intangible();
        boo_stop();
    }
}

void ActionBooGivingStar1(void)
{
    s32 sp24;
    s16 sp22;
    f32 sp1C;
    if(o->oHealth == 3)
    {
        sp22 = 0x180;
        sp1C = 0.5f;
    }
    else if(o->oHealth == 2)
    {
        sp22 = 0x240;
        sp1C = 0.6f;
    }
    else
    {
        sp22 = 0x300;
        sp1C = 0.8f;
    }
    func_802C3830(-100.0f,sp22,sp1C);
    sp24 = func_802C3778();
    if(obj_has_behavior(bhvSpawnedBoo))
    {
        if(gMarioOnMerryGoRound == 0)
            o->oAction = 0;
    }
    else if(func_802C2A94())
        o->oAction = 0;
    if(sp24 == -1)
        o->oAction = 2;
    if(sp24 == 1)
        o->oAction = 3;
    if(sp24 == 1)
        create_sound_spawner(SOUND_OBJECT_THWOMP);
}

void ActionBooGivingStar2(void)
{
    if(func_802C33D0(20.0f))
        o->oAction = 1;
}

void func_802C41AC(void)
{
    CreateStar(980.0f,1100.0f,250.0f);
}

void func_802C41E4(void)
{
    CreateStar(700.0f,3200.0f,1900.0f);
}

void func_802C4220(void)
{
    struct Object* sp1C;
    CreateStar(-1600.0f,-2100.0f,205.0f);
    sp1C = obj_nearest_object_with_behavior(bhvRotatingMerryGoRound);
    if(sp1C != NULL)
        sp1C->oUnk88 = 1;
}

void ActionBooGivingStar3(void)
{
    if(o->oTimer == 0)
        o->oHealth--;
    if(o->oHealth == 0)
    {
        if(func_802C35C0())
        {
            obj_disable();
            o->oAction = 4;
            set_object_angle(o,0,0,0);
            if(o->oBehParams2ndByte == 0)
                func_802C41AC();
            else if(o->oBehParams2ndByte == 1)
                func_802C4220();
            else
                func_802C41E4();
        }
    }
    else
    {
        if(o->oTimer == 0)
        {
            func_802A3004();
            o->oBooUnkF8 -= 0.5;
        }
        if(func_802C34B4(40.0f))
            o->oAction = 1;
    }
}

void ActionBooGivingStar4(void)
{
#ifdef VERSION_US
    boo_stop();
#endif
    if(o->oBehParams2ndByte == 0)
    {
        set_object_pos(o,973,0,626);
        if(o->oTimer > 60 && o->oDistanceToMario < 600.0f)
        {
            set_object_pos(o,973,0,717);
            spawn_object_relative(0,0,0,     0,o,MODEL_BBH_STAIRCASE_STEP,bhvBooBossSpawnedBridge);
            spawn_object_relative(1,0,0,-200,o,MODEL_BBH_STAIRCASE_STEP,bhvBooBossSpawnedBridge);
            spawn_object_relative(2,0,0, 200,o,MODEL_BBH_STAIRCASE_STEP,bhvBooBossSpawnedBridge);
            mark_object_for_deletion(o);
        }
    }
    else
        mark_object_for_deletion(o);
}

void (*TableBooGivingStarActions[])(void) = {ActionBooGivingStar0,ActionBooGivingStar1,ActionBooGivingStar2,ActionBooGivingStar3,ActionBooGivingStar4};

void bhv_boo_giving_star_loop(void)
{
    set_object_hitbox(o,&sBooGivingStarHitbox);
    o->oGraphYOffset = o->oBooUnkF8 * 60.0f;
    obj_update_floor_and_walls();
    obj_call_action_function(TableBooGivingStarActions);
    obj_move_standard(78);
    func_802C2D7C();
    o->oInteractStatus = 0;
}

void ActionBooWithCage0(void)
{
    o->oBooUnk108 = 0;
    o->oBooUnkF4 = 0xFF;
    o->oBooUnkF8 = 2.0f;
    obj_scale(2.0f);
    obj_become_tangible();
    if(boo_should_be_active())
        o->oAction = 1;
}

void ActionBooWithCage1(void)
{
    s32 sp1C;

    func_802C3830(100.0f,512,0.5f);
    sp1C = func_802C3778();
    if(func_802C2A94())
        o->oAction = 0;
    if(sp1C == -1)
        o->oAction = 2;
    if(sp1C == 1)
        o->oAction = 3;
}

void ActionBooWithCage2(void)
{
    if(func_802C33D0(20.0f))
        o->oAction = 1;
}

void ActionBooWithCage3(void)
{
    if(func_802C35C0())
        mark_object_for_deletion(o);
}

void bhv_boo_with_cage_init(void)
{
    struct Object* cage;
    if(gDisplayedStars < 12)
        mark_object_for_deletion(o);
    else
    {
        cage = spawn_object(o,MODEL_HAUNTED_CAGE,bhvBooCage);
        cage->oBehParams = o->oBehParams;
    }
}

void (*TableBooWithCageActions[])(void) = {ActionBooWithCage0,ActionBooWithCage1,ActionBooWithCage2,ActionBooWithCage3};

void bhv_boo_with_cage_loop(void)
{
    obj_update_floor_and_walls();
    obj_call_action_function(TableBooWithCageActions);
    obj_move_standard(78);
    func_802C2D7C();
    o->oInteractStatus = 0;
}

void bhv_spawn_big_boo_loop(void)
{
    struct Object* sp24;
    switch(o->oAction)
    {
    case 0:
        if(o->oDistanceToMario < 1000.0f)
        {
            if(o->oUnk88 < 5)
            {
                if(o->oBooUnkFC != 5)
                {
                    if(o->oBooUnkFC - o->oUnk88 < 2)
                    {
                        spawn_object(o,MODEL_BOO,bhvSpawnedBoo2);
                        o->oBooUnkFC++;
                    }
                }
                o->oAction++;
            }
            if(o->oUnk88 > 4)
            {
                sp24 = spawn_object(o,MODEL_BOO,bhvSpawnedBoo);
                copy_object_behavior_params(sp24,o);
                o->oAction = 2;
#ifndef VERSION_JP
                play_puzzle_jingle();
#else
                SetSound(SOUND_CH8_UNK6A,D_803320E0);
#endif
            }
        }
        break;
    case 1:
            if(o->oTimer > 60)
                o->oAction = 0;
        break;
    case 2:
        break;
    }
}

void func_802C49E0(void)
{
    gSecondCameraFocus = o;
}

void bhv_animated_texture_loop(void)
{
    func_802A3470();
}

void bhv_boo_in_castle_loop(void)
{
    s16 sp1E;
    o->oBooUnkF8 = 2.0f;
    if(o->oAction == 0)
    {
        obj_hide();
        if(gDisplayedStars < 12)
            mark_object_for_deletion(o);
        if(gMarioCurrentRoom == 1)
            o->oAction++;
    }
    else if(o->oAction == 1)
    {
        obj_unhide();
        o->oOpacity = 180;
        if(o->oTimer == 0)
            obj_scale(o->oBooUnkF8);
        if(o->oDistanceToMario < 1000.0f)
        {
            o->oAction++;
            PlaySound2(SOUND_OBJECT_BOOLAUGH2);
        }
        o->oForwardVel = 0.0f;
        sp1E = o->oAngleToMario;
    }
    else
    {
        obj_obj_forward_vel_approach_upward(32.0f,1.0f);
        o->oHomeX = -1000.0f;
        o->oHomeZ = -9000.0f;
        sp1E = obj_angle_to_home();
        if(o->oPosZ < -5000.0f)
        {
            if(o->oOpacity > 0)
                o->oOpacity -= 20;
            else
                o->oOpacity = 0;
        }
        if(o->activeFlags & 8)
            o->oAction = 1;
    }
    o->oVelY = 0.0f;
    sp1E = obj_angle_to_home();
    obj_rotate_yaw_toward(sp1E,0x5A8);
    func_802C2EB8(1);
    obj_move_using_fvel_and_gravity();
}

void bhv_boo_boss_spawned_bridge_loop(void)
{
    f32 sp24;
    switch(o->oBehParams2ndByte)
    {
    case 1: sp24 = 0.0f;        break;
    case 0: sp24 = -206.0f; break;
    case 2: sp24 = -413.0f; break;
    }
    switch(o->oAction)
    {
    case 0:
        o->oPosY = o->oHomeY - 620.0f;
        o->oAction++;
    case 1:
        o->oPosY += 8.0f;
        PlaySound(SOUND_ENVIRONMENT_ELEVATOR2);
        if(o->oPosY > sp24)
        {
            o->oPosY = sp24;
            o->oAction++;
        }
        break;
    case 2:
        if(o->oTimer == 0)
            PlaySound2(SOUND_GENERAL_UNKNOWN4_LOWPRIO);
        if(func_802A362C(o->oTimer))
            o->oAction++;
        break;
    case 3:
        if(o->oTimer == 0 && o->oBehParams2ndByte == 1)
            play_puzzle_jingle();
        break;
    }
}

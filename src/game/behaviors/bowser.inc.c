// bowser.c.inc

void ActionBowserTailAnchor0(void)
{
    struct Object* bowser = o->parentObj;
    obj_become_tangible();
    obj_scale(1.0f);
    if(bowser->oAction == 19)
        bowser->oIntangibleTimer = -1;
    else if(are_objects_collided(o,gMarioObject))
    {
        bowser->oIntangibleTimer = 0;
        o->oAction = 2;
    }
    else
        bowser->oIntangibleTimer = -1;
}

void ActionBowserTailAnchor1(void)
{
    if(o->oTimer > 30)
        o->oAction = 0;
}

void ActionBowserTailAnchor2(void)
{
    if(o->parentObj->oAction == 19)
    {
        o->parentObj->oIntangibleTimer = -1;
        o->oAction = 0;
    }
    obj_become_intangible();
}

void (*TableBowserTailAnchorActions[])(void) = {ActionBowserTailAnchor0,ActionBowserTailAnchor1,ActionBowserTailAnchor2};
s8 D_8032F4FC[] = {7,8,9,12,13,14,15,4,3,16,17,19,3,3,3,3};
s16 D_8032F50C[] = {60,0};
s16 D_8032F510[] = {50,0};
s8 D_8032F514[] = {24,42,60,-1};
s16 D_8032F518[3] = {119,120,121};
s16 D_8032F520[][3] = {
    { 1,  10,  40},
    { 0,  0,   74},
    {-1, -10,  114},
    { 1, -20,  134},
    {-1,  20,  154},
    { 1,  40,  164},
    {-1, -40,  174},
    { 1, -80,  179},
    {-1,  80,  184},
    { 1,  160, 186},
    {-1, -160, 186},
    { 1, 0,    0}
};

void bhv_bowser_tail_anchor_loop(void)
{
    obj_call_action_function(TableBowserTailAnchorActions);
    o->oParentRelativePosX = 90.0f;
    if(o->parentObj->oAction == 4)
        o->parentObj->oIntangibleTimer = -1;
    o->oInteractStatus = 0;
}

void bhv_bowser_flame_spawn_loop(void)
{
    struct Object* bowser = o->parentObj;
    s32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24 = coss(bowser->oMoveAngleYaw);
    f32 sp20 = sins(bowser->oMoveAngleYaw);
    s16 *sp1C = segmented_to_virtual(bowser_seg6_unkmoveshorts_060576FC);
    if(bowser->oSoundStateID == 6)
    {
        sp30 = bowser->header.gfx.unk38.animFrame + 1.0f;
        if(bowser->header.gfx.unk38.curAnim->unk08 == sp30)
            sp30 = 0;
        if(sp30 > 45 && sp30 < 85)
        {
            PlaySound(SOUND_CH6_UNKNOWN000);
            sp2C = sp1C[5*sp30];
            sp28 = sp1C[5*sp30+2];
            o->oPosX = bowser->oPosX + (sp28 * sp20 + sp2C * sp24);
            o->oPosY = bowser->oPosY + sp1C[5*sp30+1];
            o->oPosZ = bowser->oPosZ + (sp28 * sp24 - sp2C * sp20);
            o->oMoveAnglePitch = sp1C[5*sp30+4] + 0xC00;
            o->oMoveAngleYaw = sp1C[5*sp30+3] + (s16)bowser->oMoveAngleYaw;
            if(!(sp30 & 1))
                spawn_object(o,MODEL_RED_FLAME,bhvFlameMovingForwardGrowing);
        }
    }
}

void bhv_bowser_body_anchor_loop(void)
{
    copy_object_pos_and_angle(o,o->parentObj);
    if(o->parentObj->oAction == 4) {
#ifdef VERSION_US
        if(o->parentObj->oSubAction == 11)
            o->oInteractType = 0;
        else
        o->oInteractType = 0x800000;    
#else
        o->oInteractType = 0x800000;
#endif
    }
    else
    {
        o->oInteractType = 8;
        if(o->parentObj->oOpacity < 100)
            obj_become_intangible();
        else
            obj_become_tangible();
    }
    if(o->parentObj->oHeldState != HELD_FREE)
        obj_become_intangible();
    o->oInteractStatus = 0;
}

s32 func_802B38B4(void)
{
    struct Object* wave;
    if(o->oBehParams2ndByte == 2)
    {
        wave = spawn_object(o,MODEL_BOWSER_WAVE,bhvBowserShockWave);
        wave->oPosY = o->oFloorHeight;
        return 1;
    }
    return 0;
}

void func_802B392C(s32* a)
{
    if(o->oMoveFlags & 1)
    {
        a[0]++;
        if(a[0] < 4)
        {
            func_802A11B4(o,8);
            func_802AA618(0,0,60.0f);
            PlaySound2(SOUND_OBJECT_BOWSERWALK);
        }
    }
}

#define BITDW (o->oBehParams2ndByte==0)
#define BITFS (o->oBehParams2ndByte==1)
#define BITS    (o->oBehParams2ndByte==2)

s32 func_802B39B8(void)
{
    func_8029ED38(15);
    if(obj_check_anim_frame(21))
        o->oForwardVel = 3.0f;
    if(func_8029F788())
        return 1;
    else
        return 0;
}

s32 func_802B3A30(void)
{
    o->oForwardVel = 3.0f;
    func_8029ED38(13);
    if(func_8029F788())
        return 1;
    else
        return 0;
}

s32 func_802B3A98(void)
{
    func_8029ED38(14);
    if(obj_check_anim_frame(20))
        o->oForwardVel = 0.0f;
    if(func_8029F788())
        return 1;
    else
        return 0;
}

void func_802B3B0C(void)
{
    if(o->oUnk88 == 0)
        o->oAction = 5;
    else if(o->oUnk88 == 1)
        o->oAction = 6;
    else if(o->oBehParams2ndByte == 1)
        o->oAction = 13;
    else
        o->oAction = 0;
}

void ActionBowser5() // not much
{
    o->oForwardVel = 0.0f;
    func_8029ED38(12);
    func_802B3B0C();
}

void ActionBowser6(void)
{
    if(o->oSubAction == 0)
    {
        if(func_802B39B8())
            o->oSubAction++;
    }
    else if(o->oSubAction == 1)
    {
        if(func_802B3A30())
            o->oSubAction++;
    }
    else if(func_802B3A98())
    {
        if(o->oUnk88 == 1)
            o->oUnk88 = 0;
        func_802B3B0C();
    }
}

void Unknown802B3CCC(void) // unused
{
    if(gDebugInfo[5][1] != 0)
    {
        o->oAction = D_8032F4FC[gDebugInfo[5][2] & 0xf];
        gDebugInfo[5][1] = 0;
    }
}

void func_802B3D24(void)
{
    f32 rand = RandomFloat();
    if(o->oBowserUnk110 == 0)
    {
        if(o->oBowserUnkF4 & 2)
        {
            if(o->oDistanceToMario < 1500.0f)
                o->oAction = 15; // nearby
            else
                o->oAction = 17; // far away
        }
        else
            o->oAction = 14;
        o->oBowserUnk110++;
    }
    else
    {
        o->oBowserUnk110 = 0;
#ifndef VERSION_JP
        if(!gCurrDemoInput) {
            if(rand < 0.1)
                o->oAction = 3; // rare 1/10 chance
            else
                o->oAction = 14; // common
        } else {
            o->oAction = 14; // ensure demo starts with action 14.
        }
#else
        if(rand < 0.1)
            o->oAction = 3; // rare 1/10 chance
        else
            o->oAction = 14; // common
#endif
    }
}

void func_802B3E44(void)
{
    f32 rand = RandomFloat();
    if(o->oBowserUnk110 == 0)
    {
        if(o->oBowserUnkF4 & 2)
        {
            if(o->oDistanceToMario < 1300.0f) // nearby
            {
                if(rand < 0.5) // 50/50
                    o->oAction = 16;
                else
                    o->oAction = 9;
            }
            else // far away
            {
                o->oAction = 7;
                if(500.0f < o->oBowserDistToCentre && o->oBowserDistToCentre < 1500.0f && rand < 0.5) // away from centre and good luck
                    o->oAction = 13;
            }
        }
        else
            o->oAction = 14;
        o->oBowserUnk110++;
    }
    else
    {
        o->oBowserUnk110 = 0;
        o->oAction = 14;
    }
}

void func_802B3FDC(void)
{
    f32 rand = RandomFloat();
    if(o->oBowserUnkF4 & 2)
    {
        if(o->oDistanceToMario < 1000.0f)
        {
            if(rand < 0.4)
                o->oAction = 9;
            else if(rand < 0.8)
                o->oAction = 8;
            else
                o->oAction = 15;
        }
        else if(rand < 0.5)
            o->oAction = 13;
        else
            o->oAction = 7;
    }
    else
        o->oAction = 14;
}

void func_802B4124(void)
{
    o->oAction = 13;
}

void func_802B4144(void)
{
    switch(o->oBowserUnk110)
    {
    case 0:
        if(o->oBowserUnk106 == 0)
            func_802B3FDC();
        else
            func_802B4124();
        o->oBowserUnk110 = 1;
        break;
    case 1:
        o->oBowserUnk110 = 0;
        o->oAction = 14;
        break;
    }
}

#if defined(VERSION_US) || defined(VERSION_EU)
void func_u_802B4AF4(void)
{
    if(o->oVelY < 0 && o->oPosY < (o->oHomeY - 300.0f))
    {
        o->oPosX = o->oPosZ = 0;
        o->oPosY = o->oHomeY + 2000.0f;
        o->oVelY = 0;
        o->oForwardVel = 0;
    }
}
#endif

void ActionBowser18() // unused?
{
    if(func_802A4AB0(12))
        o->oAction = 0;
}

void ActionBowser0() // only lasts one frame
{
    o->oEyesShut = 0;
    func_8029ED38(12);
    // stop him still
    o->oAngleVelYaw = 0;
    o->oForwardVel = 0.0f;
    o->oVelY = 0.0f;
    if(BITDW)
        func_802B3D24();
    else if(BITFS)
        func_802B3E44();
    else
        func_802B4144();
    // Action 14 commonly follows
}

void ActionBowser15(void)
{
    o->oForwardVel = 0.0f;
    if(o->oTimer == 0)
        PlaySound2(SOUND_OBJECT_BOWSERINHALING);
    if(func_802A4AB0(6))
        o->oAction = 0;
}

void ActionBowser14() // turn towards Mario
{
    UNUSED s32 facing; // is Bowser facing Mario?
    s16 turnSpeed;
    s16 angleFromMario = abs_angle_diff(o->oMoveAngleYaw,o->oAngleToMario);
    if(BITFS)
        turnSpeed = 0x400;
    else if(o->oHealth > 2)
        turnSpeed = 0x400;
    else if(o->oHealth == 2)
        turnSpeed = 0x300;
    else
        turnSpeed = 0x200;
    facing = obj_rotate_yaw_toward(o->oAngleToMario,turnSpeed);
    if(o->oSubAction == 0)
    {
        o->oBowserUnkF8 = 0;
        if(func_802B39B8())
            o->oSubAction++;
    }
    else if(o->oSubAction == 1)
    {
        if(func_802B3A30())
        {
            o->oBowserUnkF8++;
            if(o->oBowserUnkF4 & 0x20000)
            {
                if(o->oBowserUnkF8 > 4)
                    o->oBowserUnkF4 &= ~0x20000;
            }
            else if(angleFromMario < 0x2000)
                o->oSubAction++;
        }
    }
    else if(func_802B3A98())
        o->oAction = 0;
}

void ActionBowser16(void)
{
    switch(o->oSubAction)
    {
    case 0:
        obj_become_intangible();
        o->oUnk1AC_S16 = 0;
        o->oBowserUnkF8 = 30;
        if(o->oTimer == 0)
            PlaySound2(SOUND_CH9_UNK66);
        if(o->oOpacity == 0)
        {
            o->oSubAction++;
            o->oMoveAngleYaw = o->oAngleToMario;
        }
        break;
    case 1:
        if(o->oBowserUnkF8--)
            o->oForwardVel = 100.0f;
        else
        {
            o->oSubAction = 2;
            o->oMoveAngleYaw = o->oAngleToMario;
        }
        if(abs_angle_diff(o->oMoveAngleYaw,o->oAngleToMario) > 0x4000)
            if(o->oDistanceToMario > 500.0f)
            {
                o->oSubAction = 2;
                o->oMoveAngleYaw =    o->oAngleToMario; // large change in angle?
                PlaySound2(SOUND_CH9_UNK66);
            }
        break;
    case 2:
        o->oForwardVel = 0.0f;
        o->oUnk1AC_S16 = 0xFF;
        if(o->oOpacity == 0xFF)
            o->oAction = 0;
        obj_become_tangible();
        break;
    }
}

void ActionBowser8(void) // only in sky
{
    s32 frame;
    func_8029ED38(11);
    frame = o->header.gfx.unk38.animFrame;
    if(frame > 24 && frame < 36)
        {
            PlaySound(SOUND_CH6_UNKNOWN000);
            if(frame == 35)
                spawn_object_relative(1,0,0x190,0x64,o,MODEL_RED_FLAME,bhvBlueBowserFlame);
            else
                spawn_object_relative(0,0,0x190,0x64,o,MODEL_RED_FLAME,bhvBlueBowserFlame);
        }
    if(func_8029F788())
        o->oAction = 0;
    o->oBowserUnkF4 |= 0x20000;
}

void ActionBowser12(void)
{
    if(o->oTimer == 0)
    {
        o->oForwardVel = -400.0f;
        o->oVelY = 100.0f;
        o->oMoveAngleYaw = o->oBowserAngleToCentre + 0x8000;
        o->oEyesShut = 1;
    }
    if(o->oSubAction == 0)
    {
        func_8029ED38(25);
        o->oSubAction++;
        o->oBowserUnkF8 = 0;
    }
    else if(o->oSubAction == 1)
    {
        func_8029ED38(25);
        func_8029F728();
        func_802B392C(&o->oBowserUnkF8);
        if((o->oBowserUnkF8 > 2))
        {
            func_8029ED38(26);
            o->oVelY = 0.0f;
            o->oForwardVel = 0.0f;
            o->oSubAction++;
        }
    }
    else if(o->oSubAction == 2)
    {
        if(func_8029F788())
        {
            if(o->oHealth == 1)
                o->oAction = 3;
            else
                o->oAction = 0;
            o->oEyesShut = 0;
        }
    }
    else {}
}

s32 func_802B4A44(void)
{
    func_8029ED38(9);
    if(obj_check_anim_frame(11))
        return 1;
    else
        return 0;
}

s32 func_802B4A94(void)
{
    if(o->oMoveFlags & 1)
    {
        o->oForwardVel = 0;
        o->oVelY = 0;
        func_802AA618(0,0,60.0f);
        func_8029ED38(8);
        o->header.gfx.unk38.animFrame = 0;
        func_802A11B4(o,7);
        if(BITDW)
        {
            if(o->oDistanceToMario < 850.0f)
                gMarioObject->oInteractStatus |= INTERACT_GRABBABLE;
            else
                gMarioObject->oInteractStatus |= INTERACT_HOOT; // hmm...
        }
        return 1;
    }
    else
        return 0;
}

void func_802B4BA4(void)
{
    if(BITS && o->oBowserUnkF4 & 0x10000)
        if(o->oBowserDistToCentre > 1000.0f)
            o->oForwardVel = 60.0f;
}

void ActionBowser13(void)
{
    UNUSED s32 unused;
    if(o->oSubAction == 0)
    {
        if(func_802B4A44())
        {
            if(BITS && o->oBowserUnkF4 & 0x10000)
                o->oVelY = 70.0f;
            else
                o->oVelY = 80.0f;
            o->oBowserUnkF8 = 0;
            func_802B4BA4();
            o->oSubAction++;
        }
    }
    else if(o->oSubAction == 1)
    {
#ifndef VERSION_JP
        if(o->oBehParams2ndByte == 2 && o->oUnknownUnkF4_S32 & 0x10000)
            func_u_802B4AF4();
        if(func_802B4A94())
        {
            o->oBowserUnkF4 &= 0xfffeffff;
            o->oForwardVel = 0.0f;
            o->oSubAction++;
            func_802B38B4();
            if(BITFS)
                o->oAction = 19;
        }
        else {}
#else
        if(func_802B4A94())
        {
            o->oBowserUnkF4 &= 0xfffeffff;
            o->oForwardVel = 0.0f;
            o->oSubAction++;
            func_802B38B4();
            if(BITFS)
                o->oAction = 19;
        }
        else {}
#endif
    }
    else if(func_8029F788())
        o->oAction = 0;
}

void ActionBowser17(void)
{
    f32 sp1C = D_8032F50C[0];
    f32 sp18 = D_8032F510[0];
    if(o->oSubAction == 0)
    {
        if(func_802B4A44())
        {
            o->oVelY = sp1C;
            o->oForwardVel = sp18;
            o->oBowserUnkF8 = 0;
            o->oSubAction++;
        }
    }
    else if(o->oSubAction == 1)
    {
        if(func_802B4A94())
            o->oSubAction++;
    }
    else if(func_8029F788())
        o->oAction = 0;
}

void ActionBowser10(void)
{
    o->oForwardVel = 0.0f;
    if(o->oTimer == 0)
        o->oBowserUnkF8 = 0;
    switch(o->oSubAction)
    {
    case 0:
        func_8029ED38(23);
        if(func_8029F788())
            o->oBowserUnkF8++;
        if(o->oBowserUnkF8 > 0)
            o->oSubAction++;
        break;
    case 1:
        func_8029ED38(24);
        if(func_8029F788())
            o->oAction = 11;
        break;
    }
}

void ActionBowser9(void)
{
    if(gDisplayedHealthWedges < 4)
        o->oBowserUnk108 = 3;
    else
        o->oBowserUnk108 = RandomFloat() * 3.0f + 1.0f;
    func_8029ED38(22);
    if(obj_check_anim_frame(5))
        obj_spit_fire(0,200,180,7.0f,MODEL_RED_FLAME,30.0f,10.0f,0x1000);
    if(func_8029F788())
        o->oSubAction++;
    if(o->oSubAction >= o->oBowserUnk108)
        o->oAction = 0;
}

s32 func_802B5108(s32 a0,s16 a1)
{
    if(o->oSubAction == 0)
    {
        if(func_802A4AB0(15))
            o->oSubAction++;
    }
    else if(o->oSubAction == 1)
    {
        if(func_802A4AB0(14))
            o->oSubAction++;
    }
    else
        func_8029ED38(12);
    o->oForwardVel = 0.0f;
    o->oMoveAngleYaw += a1;
    if(o->oTimer >= a0)
        return 1;
    else
        return 0;
}

void ActionBowser11(void)
{
    if(func_802B5108(63,0x200))
        o->oAction = 0;
}

void ActionBowser7(void)
{
    s32 sp34;
    if(o->oTimer == 0)
        o->oForwardVel = 0.0f;
    switch(o->oSubAction)
    {
    case 0:
        o->oBowserUnkF8 = 0;
        if(func_802A4AB0(18))
            o->oSubAction = 1;
        break;
    case 1:
        o->oForwardVel = 50.0f;
        if(func_802A4AB0(0x13) != 0)
        {
            o->oBowserUnkF8++;
            if(o->oBowserUnkF8 >= 6)
                o->oSubAction = 3;
            if(o->oBowserUnkF8 >= 2)
                if(abs_angle_diff(o->oAngleToMario,o->oMoveAngleYaw) > 0x2000)
                    o->oSubAction = 3;
        }
        obj_rotate_yaw_toward(o->oAngleToMario,0x200);
        break;
    case 3:
        o->oBowserUnkF8 = 0;
        func_8029ED38(21);
        spawn_object_relative_with_scale(0, 100,-50,0,3.0f,o,MODEL_SMOKE,&bhvWhitePuffSmoke2);
        spawn_object_relative_with_scale(0,-100,-50,0,3.0f,o,MODEL_SMOKE,&bhvWhitePuffSmoke2);
        if(approach_f32_signed(&o->oForwardVel,0,-1.0f))
            o->oSubAction = 2;
        func_8029F728();
        break;
    case 2:
        o->oForwardVel = 0.0f;
        func_8029ED38(20);
        if(func_8029F788())
        {
            if(BITS)
                sp34 = 10;
            else
                sp34 = 30;
            if(o->oBowserUnkF8 > sp34)
                o->oAction = 0;
            o->oBowserUnkF8++;
        }
        func_8029F728();
        break;
    }
    if(o->oMoveFlags & 0x400)
        o->oAction = 10;
}

s32 func_802B5588(void)
{
    struct Object* mine;
    f32 sp18;
    mine = obj_find_nearest_object_with_behavior(bhvBowserBomb, &sp18);
    if(mine != NULL && sp18 < 800.0f)
    {
        mine->oInteractStatus |= INTERACT_MR_BLIZZARD;
        return 1;
    }
    return 0;
}

void ActionBowser1(void) // unused?
{
    UNUSED s32 unused;
    if(o->oTimer < 2)
        o->oBowserUnkF8 = 0;
    if(o->oSubAction == 0)
    {
        func_8029ED38(2);
        func_802B392C(&o->oBowserUnkF8);
        if(o->oMoveFlags & 2)
        {
            o->oForwardVel = 0.0f;
            o->oSubAction++;
        }
    }
    else if(func_802A4AB0(0))
        o->oAction = 0;
    if(func_802B5588())
    {
        o->oHealth--;
        if(o->oHealth <= 0)
            o->oAction = 4;
        else
            o->oAction = 12;
    }
}

void func_802B5738(void)
{
    o->oUnk1AC_S16 = 0;
    if(o->oOpacity == 0)
    {
        o->oForwardVel = 0.0f;
        o->oVelY = 0.0f;
        o->oPosY = o->oHomeY - 1000.0f;
    }
}

void ActionBowser2(void)
{
    s32 sp2C;
    UNUSED s32 unused;
    s8* sp24 = (s8*)o->oFloor;
    if(sp24 != NULL && sp24[4] & 1)
        sp2C = 1;
    else
        sp2C = 0;
    o->oBowserUnkF4 |= 0x10000;
    switch(o->oSubAction)
    {
    case 0:
        if(o->oTimer == 0)
        {
            o->oFaceAnglePitch = 0;
            o->oFaceAngleRoll = 0;
        } //? missing else
        o->oFaceAnglePitch += 0x800;
        o->oFaceAngleRoll += 0x800;
        if(!(o->oFaceAnglePitch & 0xFFFF))
            o->oSubAction++;
        func_802B5738();
        break;
    case 1:
        func_8029ED38(9);
        if(obj_check_anim_frame(11))
        {
            o->oMoveAngleYaw = o->oBowserAngleToCentre;
            o->oVelY = 150.0f;
            o->oUnk1AC_S16 = 0xFF;
            o->oBowserUnkF8 = 0;
            o->oSubAction++;
        }
        else
            func_802B5738();
        break;
    case 2:
        if(o->oPosY > o->oHomeY)
        {
            o->oDragStrength = 0.0f;
            if(o->oBowserDistToCentre < 2500.0f)
            {
                if(absf(o->oFloorHeight - o->oHomeY) < 100.0f)
                    approach_f32_signed(&o->oForwardVel,0,-5.0f);
                else
                    obj_obj_forward_vel_approach_upward(150.0f,2.0f);
            }
            else
                obj_obj_forward_vel_approach_upward(150.0f,2.0f);
        }
        if(func_802B4A94())
        {
            o->oDragStrength = 10.0f;
            o->oSubAction++;
            if(sp2C == 0)
                func_802B38B4();
            else if(BITS)
                o->oAction = 13;
            if(BITFS)
                o->oAction = 19;
        }
#ifndef VERSION_JP
        func_u_802B4AF4();
#else
        if(o->oVelY < 0.0f && o->oPosY < o->oHomeY - 300.0f)
        {
            o->oPosZ = 0.0f, o->oPosX = o->oPosZ;
            o->oPosY = o->oHomeY + 2000.0f;
            o->oVelY = 0.0f;
        }
#endif
        break;
    case 3:
        if(func_8029F788())
        {
            o->oAction = 0;
            o->oBowserUnkF4 &= ~0x10000;
            func_8029F728();
        }
        break;
    }
    print_debug_bottom_up("sp %d",o->oForwardVel);
}

void ActionBowser3(void)
{
    if(item_in_array(o->oTimer,D_8032F514))
        PlaySound2(SOUND_OBJECT_BOWSERWALK);
    if(func_802A4AB0(10))
        o->oAction = 0;
}

void func_802B5C78(void)
{
    if(BITS)
        gSecondCameraFocus = spawn_object(o,MODEL_STAR,&bhvGrandStar);
    else
    {
        gSecondCameraFocus = spawn_object(o,MODEL_BOWSER_KEY_2,&bhvBowserKey2);
        PlaySound2(SOUND_CH8_UNK61);
    }
    gSecondCameraFocus->oAngleVelYaw = o->oAngleVelYaw;
}

void func_802B5D18(void)
{
    func_8029ED38(16);
    if(BITS)
        o->oForwardVel = -400.0f;
    else
        o->oForwardVel = -200.0f;
    o->oVelY = 100.0f;
    o->oMoveAngleYaw = o->oBowserAngleToCentre + 0x8000;
    o->oBowserUnkF8 = 0;
    o->oSubAction++;
}

void func_802B5DD8(void)
{
    o->oEyesShut = 1;
    func_802B392C(&o->oBowserUnkF8);
    if(o->oMoveFlags & 1)
        PlaySound2(SOUND_OBJECT_BOWSERWALK);
    if(o->oMoveFlags & 2)
    {
        o->oForwardVel = 0.0f;
        o->oSubAction++;
    }
}

s32 func_802B5E7C(void)
{
    s32 ret = 0;
    obj_become_intangible();
    if(func_802A4AB0(17) && o->oDistanceToMario < 700.0f && abs_angle_diff(gMarioObject->oMoveAngleYaw,o->oAngleToMario) > 0x6000)
        ret = 1;
    func_8029F728();
    o->oBowserUnkF8 = 0;
    return ret;
}

s32 func_802B5F20(void)
{
    s32 ret = 0;
    if(o->header.gfx.scale[0] < 0.8)
        o->oAngleVelYaw += 0x80;
    if(o->header.gfx.scale[0] > 0.2)
    {
        o->header.gfx.scale[0] = o->header.gfx.scale[0] - 0.02;
        o->header.gfx.scale[2] = o->header.gfx.scale[2] - 0.02;
    }
    else
    {
        o->header.gfx.scale[1] = o->header.gfx.scale[1] - 0.01;
        o->oVelY = 20.0f;
        o->oGravity = 0.0f;
    }
    if(o->header.gfx.scale[1] < 0.5)
        ret = 1;
    o->oMoveAngleYaw += o->oAngleVelYaw;
    if(o->oOpacity >= 3)
        o->oOpacity -= 2;
    return ret;
}

void func_802B60B8(void)
{
    obj_scale(0);
    o->oForwardVel = 0.0f;
    o->oVelY = 0.0f;
    o->oGravity = 0.0f;
}

s32 func_802B6120(void)
{
    s32 ret = 0;
    if(o->oBowserUnkF8 < 2)
    {
        if(o->oBowserUnkF8 == 0)
        {
            func_8031FFB4(0,60,40);
            o->oBowserUnkF8++;
        }
        if(obj_update_dialogue_unk1(2,18,D_8032F518[o->oBehParams2ndByte],0))
        {
            o->oBowserUnkF8++;
            PlaySound2(SOUND_CH8_UNK60);
            func_80320040(0,60);
            func_8031F7CC(0,1);
        }
    }
    else if(func_802B5F20())
    {
        func_802B60B8();
        spawn_triangle_break_particles(20,116,1.0f,0);
        func_802B5C78();
        set_mario_npc_dialogue(0);
        ret = 1;
    }
    return ret;
}

s32 func_802B6254(void)
{
    UNUSED s32 unused;
    s32 ret = 0;
    s32 sp1C;
    if(o->oBowserUnkF8 < 2)
    {
        if(gDisplayedStars < 120)
            sp1C = 121;
        else
            sp1C = 163;
        if(o->oBowserUnkF8 == 0)
        {
            func_8031FFB4(0,60,40);
            o->oBowserUnkF8++;
        }
        if(obj_update_dialogue_unk1(2,18,sp1C,0))
        {
            obj_set_model(MODEL_BOWSER2);
            func_80320040(0,60);
            func_8031F7CC(0,1);
            func_802B5C78();
            o->oBowserUnkF8++;
        }
    }
    else if(o->oOpacity > 4)
        o->oOpacity -= 4;
    else
    {
        func_802B60B8();
        ret = 1;
    }
    return ret;
}

void ActionBowser4(void)
{
    switch(o->oSubAction)
    {
    case 0:
        func_802B5D18();
        break;
    case 1:
        func_802B5DD8();
        break;
    case 2:
        if(func_802B5E7C())
        {
            o->oBowserUnkF8 = 0;
            if(BITS)
                o->oSubAction = 10;
            else
            {
                o->activeFlags |= 0x80;
                o->oSubAction++;
            }
        }
        break;
    case 3:
        if(func_802B6120())
            o->oSubAction++;
        break;
    case 4:
        break;
    case 10:
        if(func_802B6254())
            o->oSubAction++;
        break;
    case 11:
        break;
    }
}

void func_802B64E8(struct Object* bowser,s16 a1)
{
    s16 angle;
    angle = o->oBowserAngleToCentre + 0x8000;
    bowser->oAngleVelPitch = coss(angle) * a1;
    bowser->oAngleVelRoll = -sins(angle) * a1;
}

void ActionBowser19(void)
{
    struct Object* platform = obj_nearest_object_with_behavior(bhvTiltingBowserLavaPlatform);
    UNUSED s16 sp2A = o->oBowserAngleToCentre + 0x8000;
    s16 sp28;
    UNUSED s32 unused;
    s32 i;
    s32 sp1C;
    if(platform == NULL)
        o->oAction = 0;
    else
    {
        i = 0;
        sp1C = 1;
        while(D_8032F520[i][2]!=0)
        {
            if(o->oTimer < D_8032F520[i][2])
            {
                sp28 = D_8032F520[i][1];
                if(D_8032F520[i][0] > 0)
                    sp28 = (D_8032F520[i][2]-o->oTimer-1)*sp28;
                else
                    sp28 = (o->oTimer-D_8032F520[i-1][2])*sp28;
                func_802B64E8(platform,sp28);
                if(sp28 != 0)
                    SetSound(SOUND_ENVIRONMENT_UNKNOWN4,platform->header.gfx.cameraToObject);
                sp1C = 0;
                break;
            }
            i++;
        }
        if(sp1C)
        {
            o->oAction = 0;
            platform->oAngleVelPitch = 0;
            platform->oAngleVelRoll = 0;
            platform->oFaceAnglePitch = 0;
            platform->oFaceAngleRoll = 0;
        }
    }
    func_8029F728();
}

s32 func_802B67C4(void) // bowser off stage?
{
    if(o->oAction != 2 && o->oAction != 19)
    {
        if(o->oPosY < o->oHomeY - 1000.0f)
            return 1;
        if(o->oMoveFlags & 1)
        {
            if(o->oFloorType == 1)
                return 1;
            if(o->oFloorType == 10)
                return 1;
        }
    }
    return 0;
}

void (*TableBowserActions[])(void) = {
    ActionBowser0,
    ActionBowser1,
    ActionBowser2,
    ActionBowser3,
    ActionBowser4,
    ActionBowser5,
    ActionBowser6,
    ActionBowser7,
    ActionBowser8,
    ActionBowser9,
    ActionBowser10,
    ActionBowser11,
    ActionBowser12,
    ActionBowser13,
    ActionBowser14,
    ActionBowser15,
    ActionBowser16,
    ActionBowser17,
    ActionBowser18,
    ActionBowser19
};
struct SoundState D_8032F5B8[] = {
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {1, 0,  -1,  SOUND_OBJECT_BOWSERWALK},
    {1, 0,  -1,  SOUND_CH9_UNK04},
    {1, 0,  -1,  SOUND_CH9_UNK04},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {1, 20, 40,  SOUND_OBJECT_BOWSERWALK},
    {1, 20, -1,  SOUND_OBJECT_BOWSERWALK},
    {1, 20, 40,  SOUND_OBJECT_BOWSERWALK},
    {1, 0,  -1,  SOUND_OBJECT_BOWSERTAILPICKUP},
    {1, 0,  -1,  SOUND_OBJECT_BOWSERDEFEATED},
    {1, 8,  -1,  SOUND_OBJECT_BOWSERWALK},
    {1, 8,  17,  SOUND_OBJECT_BOWSERWALK},
    {1, 8,  -10, SOUND_OBJECT_BOWSERWALK},
    {0, 0,  0,   NO_SOUND},
    {1, 5,  -1,  SOUND_OBJECT_FLAMEBLOWN},
    {0, 0,  0,   NO_SOUND},
    {0, 0,  0,   NO_SOUND},
    {1, 0,  -1,  SOUND_OBJECT_BOWSERTAILPICKUP},
    {1, 0,  -1,  SOUND_CH9_UNK04}
};
s8 D_8032F690[4] = {0,0,1,0};
s8 D_8032F694[4] = {1,1,3,0};
extern u8 bowser_3_seg7_collision_07004B94[];
extern u8 bowser_3_seg7_collision_07004C18[];
extern u8 bowser_3_seg7_collision_07004C9C[];
extern u8 bowser_3_seg7_collision_07004D20[];
extern u8 bowser_3_seg7_collision_07004DA4[];
extern u8 bowser_3_seg7_collision_07004E28[];
extern u8 bowser_3_seg7_collision_07004EAC[];
extern u8 bowser_3_seg7_collision_07004F30[];
extern u8 bowser_3_seg7_collision_07004FB4[];
extern u8 bowser_3_seg7_collision_07005038[];
struct Struct8032F698 D_8032F698[] = {
    {NULL,0,0,0,0},
    {bowser_3_seg7_collision_07004B94, -800,  -1000, -20992, 0},
    {bowser_3_seg7_collision_07004C18, -1158, 390,   -18432, 0},
    {bowser_3_seg7_collision_07004C9C, -1158, 390,   -7680,  0},
    {bowser_3_seg7_collision_07004D20, 0,     1240,  -6144,  0},
    {bowser_3_seg7_collision_07004DA4, 0,     1240,  6144,   0},
    {bowser_3_seg7_collision_07004E28, 1158,  390,   7680,   0},
    {bowser_3_seg7_collision_07004EAC, 1158,  390,   18432,  0},
    {bowser_3_seg7_collision_07004F30, 800,   -1000, 20992,  0},
    {bowser_3_seg7_collision_07004FB4, 800,   -1000, -31744, 0},
    {bowser_3_seg7_collision_07005038, -800,  -1000, 31744,  0}
};

void func_802B688C(void)
{
    struct Surface* floor;
    struct Object* platform;
    UNUSED f32 floorHeight;
    if((platform = o->platform) != NULL)
        apply_platform_displacement(0,platform);
    o->oBowserUnk10E = 0;
    obj_update_floor_and_walls();
    obj_call_action_function(TableBowserActions);
    obj_move_standard(-78);
    if(func_802B67C4())
        o->oAction = 2; // bowser go home?
    floorHeight = find_floor(o->oPosX,o->oPosY,o->oPosZ,&floor);
    if((floor != NULL) && (floor->object != 0))
        o->platform = floor->object;
    else
        o->platform = NULL;
    exec_anim_sound_state(D_8032F5B8);
}

void func_802B697C(void)
{
    o->oBowserUnkF4 &= ~0x20000;
    obj_become_intangible();
    switch(o->oBowserUnk10E)
    {
    case 0:
        PlaySound2(SOUND_OBJECT_BOWSERTAILPICKUP);
        func_8029FA5C(3,1);
        o->oBowserUnk10E++;
        break;
    case 1:
        if(func_8029F788())
        {
            func_8029ED38(2);
            o->oBowserUnk10E++;
        }
        break;
    case 2:
        break;
    }
    o->oMoveFlags = 0;
    o->oBowserHeldAnglePitch = gMarioObject->oMoveAnglePitch;
    o->oBowserHeldAngleVelYaw = gMarioObject->oAngleVelYaw;
    o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw;
}

void func_802B6AC0(void)
{
    f32 sp1C;
    o->oBowserUnk10E = 0;
    obj_get_thrown_or_placed(1.0f,1.0f,1);
    sp1C = o->oBowserHeldAngleVelYaw / 3000.0 * 70.0f;
    if(sp1C < 0.0f)
        sp1C = -sp1C;
    if(sp1C > 90.0f)
        sp1C *= 2.5; // > 90 => get bigger?
    o->oForwardVel = coss(o->oBowserHeldAnglePitch) * sp1C;
    o->oVelY = -sins(o->oBowserHeldAnglePitch) * sp1C;
    obj_become_intangible();
    o->prevObj->oAction = 1; // not sure what prevObj is
    o->prevObj->oTimer = 0;
    o->prevObj->oSubAction = 0;
    o->oTimer = 0;
    o->oSubAction = 0;
}

void bhv_bowser_loop(void)
{
    s16 angleToMario;    // AngleToMario    from Bowser's perspective
    s16 angleToCentre; // AngleToCentre from Bowser's perspective

    o->oBowserDistToCentre = sqrtf(o->oPosX*o->oPosX + o->oPosZ*o->oPosZ);
    o->oBowserAngleToCentre = atan2s(0.0f-o->oPosZ,0.0f-o->oPosX);
    angleToMario = abs_angle_diff(o->oMoveAngleYaw,o->oAngleToMario);
    angleToCentre = abs_angle_diff(o->oMoveAngleYaw,o->oBowserAngleToCentre);
    o->oBowserUnkF4 &= ~0xFF;
    if(angleToMario < 0x2000)
        o->oBowserUnkF4 |= 2;
    if(angleToCentre < 0x3800)
        o->oBowserUnkF4 |= 4;
    if(o->oBowserDistToCentre < 1000.0f)
        o->oBowserUnkF4 |= 0x10;
    if(o->oDistanceToMario < 850.0f)
        o->oBowserUnkF4 |= 8;
    switch(o->oHeldState)
    {
    case HELD_FREE:        func_802B688C(); break;
    case HELD_HELD:        func_802B697C(); break;
    case HELD_THROWN:    func_802B6AC0(); break;
    case HELD_DROPPED: func_802B6AC0(); break;
    }
    obj_align_gfx_with_floor();
    if(o->oAction != 4)
        if(o->oUnk1AC_S16 != o->oOpacity)
        {
            if(o->oUnk1AC_S16 > o->oOpacity)
            {
                o->oOpacity += 20;
                if(o->oOpacity >= 0x100)
                    o->oOpacity = 0xFF;
            }
            else
            {
                o->oOpacity -= 20;
                if(o->oOpacity < 0)
                    o->oOpacity = 0;
            }
        }
}

void bhv_bowser_init(void)
{
    s32 level; // 0 is dw, 1 is fs, 2 is sky
    o->oBowserUnk110 = 1;
    o->oOpacity = 0xFF;
    o->oUnk1AC_S16 = 0xFF;
    if(gCurrLevelNum == LEVEL_BOWSER_2)
        level = 1;
    else if(gCurrLevelNum == LEVEL_BOWSER_3)
        level = 2;
    else
        level = 0;
    o->oBehParams2ndByte = level;
    o->oUnk1B2 = D_8032F690[level];
    o->oHealth = D_8032F694[level];
    func_802A11B4(o,4);
    o->oAction = 5;
    o->oUnk1AE = 0;
    o->oEyesShut = 0;
}

#undef BITDW
#undef BITFS
#undef BITS

Gfx *Geo18_802B798C(s32 run, UNUSED struct GraphNode *node, Mat4 mtx)
{
    Mat4 sp20;
    struct Object* sp1C;

    if(run == TRUE)
    {
        sp1C = (struct Object*)D_8032CFA0;
        if(sp1C->prevObj != NULL)
        {
            func_8029D704(sp20,mtx,D_8032CF9C->unk34);
            func_8029D558(sp20,sp1C->prevObj);
            func_8029EA0C(sp1C->prevObj);
        }
    }
    return NULL;
}

void func_802B70C8(struct Object* a0, struct GraphNodeSwitchCase * switchCase)
{
    s32 sp1C;
    s16 sp1A;
    sp1A = abs_angle_diff(a0->oMoveAngleYaw,a0->oAngleToMario);
    sp1C = switchCase->unk1E;
    switch(sp1C)
    {
    case 0:
        if(sp1A > 0x2000)
        {
            if(a0->oAngleVelYaw > 0)
                switchCase->unk1E = 5;
            if(a0->oAngleVelYaw < 0)
                switchCase->unk1E = 3;
        }
        if(a0->oUnk1AE > 50)
            switchCase->unk1E = 1;
        break;
    case 1:
        if(a0->oUnk1AE > 2)
            switchCase->unk1E = 2;
        break;
    case 2:
        if(a0->oUnk1AE > 2)
            switchCase->unk1E = 9;
        break;
    case 9:
        if(a0->oUnk1AE > 2)
            switchCase->unk1E = 0;
        break;
    case 5:
        if(a0->oUnk1AE > 2)
        {
            switchCase->unk1E = 6;
            if(a0->oAngleVelYaw <= 0)
                switchCase->unk1E = 0;
        }
        break;
    case 6:
        if(a0->oAngleVelYaw <= 0)
            switchCase->unk1E = 5;
        break;
    case 3:
        if(a0->oUnk1AE > 2)
        {
            switchCase->unk1E = 4;
            if(a0->oAngleVelYaw >= 0)
                switchCase->unk1E = 0;
        }
        break;
    case 4:
        if(a0->oAngleVelYaw >= 0)
            switchCase->unk1E = 3;
        break;
    default:
        switchCase->unk1E = 0;
    }
    if(switchCase->unk1E != sp1C)
        a0->oUnk1AE = -1;
}

/** Geo switch for controlling the state of bowser's eye direction and open/closed
  * state. Checks whether oEyesShut is TRUE and closes eyes if so and processes
  * direction otherwise.
  */
s32 geo_switch_bowser_eyes(s32 run, struct GraphNode *node, UNUSED Mat4 *mtx)
{
    UNUSED s16 sp36;
    UNUSED s32 unused;
    struct Object *obj = (struct Object *)D_8032CFA0;
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;

    if(run == TRUE)
    {
        if(D_8032CFA4 != NULL)
            obj = (struct Object *)D_8032CFA4->unk1C;
        switch(sp36 = obj->oEyesShut)
        {
        case 0: // eyes open, handle eye looking direction
            func_802B70C8(obj, switchCase);
            break;
        case 1: // eyes closed, blinking
            switchCase->unk1E = 2;
            break;
        }
        obj->oUnk1AE++;
    }
    return 0;
}

Gfx *Geo18_802B7D44(s32 a0, struct GraphNode *node, UNUSED s32 a2)
{
    Gfx* sp2C = NULL;
    Gfx* sp28;
    struct Object* sp24;
    struct GraphNode12A* sp20;

    if(a0 == 1)
    {
        sp24 = (struct Object*)D_8032CFA0;
        sp20 = (struct GraphNode12A *)node;
        if(D_8032CFA4 != 0)
            sp24 = (struct Object*)D_8032CFA4->unk1C;
        if(sp24->oOpacity == 0xFF)
            sp20->fnNode.node.flags = (sp20->fnNode.node.flags & 0xFF) | 0x100;
        else
            sp20->fnNode.node.flags = (sp20->fnNode.node.flags & 0xFF) | (0x100 | 0x400);
        sp28 = sp2C = alloc_display_list(16);

        if(sp24->oUnk1B2 != 0)
        {
            gSPClearGeometryMode(sp28++, G_LIGHTING);
        }
        gSPEndDisplayList(sp28);
    }
    return sp2C;
}

void ActionFallingBowserPlatform0(void)
{
    o->oPlatformUnkF8 = obj_nearest_object_with_behavior(&bhvBowser);
    set_object_collision_data(o,D_8032F698[o->oBehParams2ndByte].unk0);
    if(o->oBowserUnkF8 != 0)
        o->oAction = 1;
}

void ActionFallingBowserPlatform1(void)
{
    UNUSED s32 unused;
    struct Object* sp0 = o->oPlatformUnkF8;
    if(sp0->platform == o)
        if(sp0->oAction == 13 && sp0->oUnknownUnkF4_S32 & 0x10000)
                o->oAction = 2;
    if(sp0->oHealth == 1 && (sp0->oAction == 3 || sp0->oHeldState != HELD_FREE))
        o->oSubAction = 1;
    if(o->oSubAction == 0)
        o->oPlatformUnkFC = 0;
    else
    {
        if((gDebugInfo[4][6] + 20) * (o->oBehParams2ndByte-1) < o->oPlatformUnkFC)
            o->oAction = 2;
        o->oPlatformUnkFC++;
    }
}

void ActionFallingBowserPlatform2(void)
{
    Vec3f sp24;
    s16 sp22;
    f32 sp1C;
    UNUSED struct Object* sp18 = o->oPlatformUnkF8;
    if(o->oTimer == 0 || o->oTimer == 22)
        PlaySound2(SOUND_GENERAL_BOWSERPLATFORM_2);
    if(o->oTimer < 22)
    {
        set_camera_shake_2(SHAKE_2_FALLING_BITS_PLAT);
        o->oVelY = 8.0f;
        o->oGravity = 0.0f;
    }
    else
        o->oGravity = -4.0f;
    if((o->oTimer & 1) == 0 && o->oTimer < 14)
    {
        sp22 = D_8032F698[o->oBehParams2ndByte].unk3 + (gDebugInfo[4][1] << 8);
        sp1C = -(o->oTimer/2) * 290 + 1740;
        func_802B8F7C(sp24,&o->oPosX);
        o->oPosX = D_8032F698[o->oBehParams2ndByte].unk1 + sins(sp22 + 5296) * sp1C;
        o->oPosZ = D_8032F698[o->oBehParams2ndByte].unk2 + coss(sp22 + 5296) * sp1C;
        o->oPosY = 307.0f;
        func_802AA618(4,0,100.0f);
        o->oPosX = D_8032F698[o->oBehParams2ndByte].unk1 + sins(sp22 - 5296) * sp1C;
        o->oPosZ = D_8032F698[o->oBehParams2ndByte].unk2 + coss(sp22 - 5296) * sp1C;
        func_802AA618(4,0,100);
        func_802B8F7C(&o->oPosX,sp24);
    }
    obj_move_using_fvel_and_gravity();
    if(o->oTimer > 300)
        mark_object_for_deletion(o);
}

void (*TableFallingBowserPlatformActions[])(void) = {ActionFallingBowserPlatform0,ActionFallingBowserPlatform1,ActionFallingBowserPlatform2};

struct ObjectHitbox sGrowingBowserFlameHitbox =
{
    /* interactType: */      INTERACT_FLAME,
    /* downOffset: */        20,
    /* damageOrCoinValue: */ 1,
    /* health: */            0,
    /* numLootCoins: */      0,
    /* radius: */            10,
    /* height: */            40,
    /* hurtboxRadius: */     0,
    /* hurtboxHeight: */     0,
};

struct ObjectHitbox sBowserFlameHitbox =
{
    /* interactType: */      INTERACT_FLAME,
    /* downOffset: */        0,
    /* damageOrCoinValue: */ 1,
    /* health: */            0,
    /* numLootCoins: */      0,
    /* radius: */            10,
    /* height: */            40,
    /* hurtboxRadius: */     0,
    /* hurtboxHeight: */     0,
};

f32 D_8032F748[] = {-8.0f,-6.0f,-3.0f};

void bhv_falling_bowser_platform_loop(void)
{
    obj_call_action_function(TableFallingBowserPlatformActions);
}

void func_802B7A58(void)
{
    mark_object_for_deletion(o);
    spawn_object_with_scale(o,MODEL_NONE,&bhvBlackSmokeUpward,1.0f);
    if(RandomFloat() < 0.1)
        spawn_object(o,MODEL_YELLOW_COIN,&bhvTemporaryYellowCoin);
}

s32 func_802B7ADC(s32 a0)
{
    if(a0 < o->oTimer)
        return 1;
    if(o->oFloorType == 1)
        return 1;
    if(o->oFloorType == 10)
        return 1;
    return 0;
}

void bhv_flame_bowser_init(void)
{
    o->oAnimState = (s32)(RandomFloat() * 10.0f);
    o->oMoveAngleYaw = RandomU16();
    if(RandomFloat() < 0.2)
        o->oVelY = 80.0f;
    else
        o->oVelY = 20.0f;
    o->oForwardVel = 10.0f;
    o->oGravity = -1.0f;
    o->oFlameUnkF4 = RandomFloat() + 1.0f;
}

void bhv_flame_large_burning_out_init(void)
{
    o->oAnimState = (s32)(RandomFloat() * 10.0f);
    o->oMoveAngleYaw = RandomU16();
    o->oVelY = 10.0f;
    o->oForwardVel = 0.0f;
    o->oFlameUnkF4 = 7.0f;
}

void func_802B7CFC(void)
{
    s32 sp4;
    sp4 = ((o->oFlameUnkF8 + gGlobalTimer) & 0x3F) << 10;
    o->oPosX += sins(o->oMoveAngleYaw) * sins(sp4) * 4.0f;
    o->oPosZ += coss(o->oMoveAngleYaw) * sins(sp4) * 4.0f;
}

void bhv_flame_bowser_loop(void)
{
    obj_update_floor_and_walls();
    obj_move_standard(78);
    if(o->oVelY < -4.0f)
        o->oVelY = -4.0f;
    if(o->oAction == 0)
    {
        obj_become_intangible();
        func_802B7CFC();
        if(o->oMoveFlags & 1)
        {
            o->oAction++;
            if(obj_has_behavior(bhvFlameLargeBurningOut))
                o->oFlameUnkF4 = 8.0f;
            else
                o->oFlameUnkF4 = RandomFloat() * 2.0f + 6.0f;
            o->oForwardVel = 0.0f;
            o->oVelY = 0.0f;
            o->oGravity = 0.0f;
        }
    }
    else
    {
        obj_become_tangible();
        if(o->oTimer > o->oFlameUnkF4 * 10.0f + 5.0f)
        {
            o->oFlameUnkF4 -= 0.15;
            if(o->oFlameUnkF4 <= 0.0f)
                func_802B7A58();
        }
    }
    obj_scale(o->oFlameUnkF4);
    o->oGraphYOffset = o->header.gfx.scale[1] * 14.0f;
    set_object_hitbox(o,&sBowserFlameHitbox);
}

void bhv_flame_moving_forward_growing_init(void)
{
    o->oForwardVel = 30.0f;
    translate_object_xz_random(o,80.0f);
    o->oAnimState = (s32)(RandomFloat() * 10.0f);
    o->oFlameUnkF4 = 3.0f;
}

void bhv_flame_moving_forward_growing_loop(void)
{
    UNUSED s32 unused;
    UNUSED struct Object* sp18;
    set_object_hitbox(o,&sGrowingBowserFlameHitbox);
    o->oFlameUnkF4 = o->oFlameUnkF4 + 0.5;
    obj_scale(o->oFlameUnkF4);
    if(o->oMoveAnglePitch > 0x800)
        o->oMoveAnglePitch -= 0x200;
    func_802A2A38();
    obj_update_floor_height();
    if(o->oFlameUnkF4 > 30.0f)
        mark_object_for_deletion(o);
    if(o->oPosY < o->oFloorHeight)
    {
        o->oPosY = o->oFloorHeight;
        sp18 = spawn_object(o,MODEL_RED_FLAME,&bhvFlameBowser);
        mark_object_for_deletion(o);
    }
}

void bhv_flame_floating_landing_init(void)
{
    o->oAnimState = (s32)(RandomFloat() * 10.0f);
    o->oMoveAngleYaw = RandomU16();
    if(o->oBehParams2ndByte != 0)
        o->oForwardVel = RandomFloat() * 5.0f;
    else
        o->oForwardVel = RandomFloat() * 70.0f;
    o->oVelY = RandomFloat() * 20.0f;
    o->oGravity = -1.0f;
    o->oFlameUnkF8 = RandomFloat() * 64.0f;
}

void bhv_flame_floating_landing_loop(void)
{
    UNUSED s32 unused;
    obj_update_floor_and_walls();
    obj_move_standard(0x4e);
    func_802B7CFC();
    if(func_802B7ADC(900))
        mark_object_for_deletion(o);
    if(o->oVelY < D_8032F748[o->oBehParams2ndByte])
        o->oVelY = D_8032F748[o->oBehParams2ndByte];
    if(o->oMoveFlags & 1)
    {
        if(o->oBehParams2ndByte == 0)
            spawn_object(o,MODEL_RED_FLAME,&bhvFlameLargeBurningOut);
        else
            spawn_object(o,MODEL_NONE,&bhvBlueFlamesGroup); //? wonder if they meant MODEL_BLUE_FLAME?
        mark_object_for_deletion(o);
    }
    o->oGraphYOffset = o->header.gfx.scale[1] * 14.0f;
}

void bhv_blue_bowser_flame_init(void)
{
    translate_object_xz_random(o,80.0f);
    o->oAnimState = (s32)(RandomFloat() * 10.0f);
    o->oVelY = 7.0f;
    o->oForwardVel = 35.0f;
    o->oFlameUnkF4 = 3.0f;
    o->oFlameUnkFC = RandomFloat() * 0.5;
    o->oGravity = 1.0f;
    o->oFlameUnkF8 = (s32)(RandomFloat() * 64.0f);
}

void bhv_blue_bowser_flame_loop(void)
{
    s32 i;
    set_object_hitbox(o,&sGrowingBowserFlameHitbox);
    if(o->oFlameUnkF4 < 16.0f)
        o->oFlameUnkF4 = o->oFlameUnkF4 + 0.5;
    obj_scale(o->oFlameUnkF4);
    obj_update_floor_and_walls();
    obj_move_standard(0x4e);
    if(o->oTimer > 0x14)
    {
        if(o->oBehParams2ndByte == 0)
            for(i=0;i<3;i++)
                spawn_object_relative_with_scale(0,0,0,0,5.0f,o,MODEL_RED_FLAME,&bhvFlameFloatingLanding);
        else
        {
            spawn_object_relative_with_scale(1,0,0,0,8.0f,o,MODEL_BLUE_FLAME,&bhvFlameFloatingLanding);
            spawn_object_relative_with_scale(2,0,0,0,8.0f,o,MODEL_BLUE_FLAME,&bhvFlameFloatingLanding);
        }
        mark_object_for_deletion(o);
    }
}

void bhv_flame_bouncing_init(void)
{
    o->oAnimState = (s32)(RandomFloat() * 10.0f);
    o->oVelY = 30.0f;
    o->oForwardVel = 20.0f;
    o->oFlameUnkF4 = o->header.gfx.scale[0];
    o->oFlameUnkF8 = (s32)(RandomFloat() * 64.0f);
}

void bhv_flame_bouncing_loop(void)
{
    struct Object* sp1C;
    if(o->oTimer == 0)
        o->oFlameUnk100 = obj_nearest_object_with_behavior(&bhvBowser);
    sp1C = o->oFlameUnk100;
    o->oForwardVel = 15.0f;
    o->oBounce = -1.0f;
    obj_scale(o->oFlameUnkF4);
    set_object_hitbox(o,&sGrowingBowserFlameHitbox);
    obj_update_floor_and_walls();
    obj_move_standard(78);
    if(func_802B7ADC(300))
        mark_object_for_deletion(o);
    if(sp1C != NULL)
        if(sp1C->oHeldState == 0)
            if(lateral_dist_between_objects(o,sp1C) < 300.0f)
                mark_object_for_deletion(o);
}

void bhv_blue_flames_group_loop(void)
{
    struct Object* flame;
    s32 i;
    if(o->oTimer == 0)
    {
        o->oMoveAngleYaw = angle_to_object(o,gMarioObject);
        o->oBlueFlameUnkF8 = 5.0f;
    }
    if(o->oTimer < 16)
    {
        if((o->oTimer & 1) == 0)
        {
            for(i=0;i<3;i++)
            {
                flame = spawn_object(o,MODEL_BLUE_FLAME,bhvFlameBouncing);
                flame->oMoveAngleYaw += i * 0x5555;
                flame->header.gfx.scale[0] = o->oBlueFlameUnkF8;
            }
            o->oBlueFlameUnkF8 -= 0.5;
        }
    }
    else
        mark_object_for_deletion(o);
}

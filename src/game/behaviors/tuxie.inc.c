// tuxie.c.inc

s32 func_802BE2E8(s16 a0,s16 a1,s32 a2)
{
    f32 sp1C;
    if((sp1C = o->header.gfx.unk38.animAccel/(f32)0x10000) == 0)
        sp1C = 1.0f;
    if(obj_check_anim_frame_in_range(a0,sp1C) || obj_check_anim_frame_in_range(a1,sp1C))
    {
        PlaySound2(a2);
        return 1;
    }
    return 0;
}

void play_penguin_walking_sound(s32 a0)
{
    s32 sp1C;
    if(o->oSoundStateID == 0)
    {
        if(a0 == 0)
            sp1C = SOUND_OBJECT_PENGUINWALKING1;
        else
            sp1C = SOUND_OBJECT_PENGUINWALKING2;
        func_802BE2E8(1,11,sp1C);
    }
}

void ActionTuxiesMother2(void)
{
    f32 sp24;
    UNUSED s32 unused;
    struct Object* sp1C = obj_find_nearest_object_with_behavior(bhvSmallPenguin,&sp24);
    UNUSED s32 unused2;
    if(obj_find_nearby_held_actor(bhvUnused20E0,1000.0f) != NULL)
    {
        if(o->oSubAction == 0)
        {
            func_8029ED38(0);
            o->oForwardVel = 10.0f;
            if(800.0f < obj_lateral_dist_from_mario_to_home())
                o->oSubAction = 1;
            obj_rotate_yaw_toward(o->oAngleToMario,0x400);
        }
        else
        {
            o->oForwardVel = 0.0f;
            func_8029ED38(3);
            if(obj_lateral_dist_from_mario_to_home() < 700.0f)
                o->oSubAction = 0;
        }
    }
    else
    {
        o->oForwardVel = 0.0f;
        func_8029ED38(3);
    }
    if(sp1C != NULL && sp24 < 300.0f && sp1C->oHeldState != HELD_FREE)
    {
        o->oAction = 1;
        sp1C->oUnk88 = 1;
        o->prevObj = sp1C;
    }
}

void ActionTuxiesMother1(void)
{
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    switch(o->oSubAction)
    {
    case 0:
        func_8029ED38(3);
        if(!obj_is_mario_on_platform())
        {
            sp2C = (o->oBehParams >> 0x10) & 0xFF;
            sp28 = (o->prevObj->oBehParams >> 0x10) & 0xFF;
            if(sp2C == sp28)
                sp24 = 58;
            else
                sp24 = 59;
            if(obj_update_dialogue_unk2(2,1,162,sp24))
            {
                if(sp24 == 58)
                    o->oSubAction = 1;
                else
                    o->oSubAction = 2;
                o->prevObj->oUnk190 |= 0x40;
            }
        }
        else
            func_8029ED38(0);
        break;
    case 1:
        if(o->prevObj->oHeldState == HELD_FREE)
        {
            ((s32*)o->prevObj)[o->oUnk190+34] &= ~0x40; //FIXME: find something more normal?
            set_object_behavior(o->prevObj,bhvUnused20E0);
#ifndef VERSION_JP
            obj_spawn_star_at_y_offset(3167.0f, -4300.0f, 5108.0f, 200.0f);
#else
            CreateStar(3500.0f,-4300.0f,4650.0f);
#endif
            o->oAction = 2;
        }
        break;
    case 2:
        if(o->prevObj->oHeldState == HELD_FREE)
        {
            ((s32*)o->prevObj)[o->oUnk190+34] &= ~0x40;
            set_object_behavior(o->prevObj,bhvPenguinBaby);
            o->oAction = 2;
        }
        break;
    }
}

void ActionTuxiesMother0(void)
{
    s32 sp2C;
    f32 sp28;
    struct Object* sp24;
    sp2C = 0;
    sp24 = obj_find_nearest_object_with_behavior(bhvSmallPenguin,&sp28);
    obj_scale(4.0f);
    func_8029ED38(3);
    if(sp28 < 500.0f)
        sp2C = 1;
    if(sp24 != NULL && sp28 < 300.0f && sp24->oHeldState != HELD_FREE)
    {
        o->oAction = 1;
        sp24->oUnk88 = 1;
        o->prevObj = sp24;
    }
    else
    {
        switch(o->oSubAction)
        {
        case 0:
            if(obj_is_mario_in_range_and_ready_to_speak(300.0f,100.0f))
                if(sp2C == 0)
                    o->oSubAction++;
            break;
        case 1:
            if(obj_update_dialogue_unk2(2,1,162,57))
                o->oSubAction++;
            break;
        case 2:
            if(o->oDistanceToMario > 450.0f)
                o->oSubAction = 0;
            break;
        }
    }
    if(obj_check_anim_frame(1))
        PlaySound2(SOUND_OBJECT_BIGPENGUIN);
}

void (*TableTuxiesMotherActions[])(void) = {ActionTuxiesMother0,ActionTuxiesMother1,ActionTuxiesMother2};

void bhv_tuxies_mother_loop(void)
{
    o->activeFlags |= 0x400;
    obj_update_floor_and_walls();
    obj_call_action_function(TableTuxiesMotherActions);
    obj_move_standard(-78);
    play_penguin_walking_sound(1);
    o->oInteractStatus = 0;
}

void func_802BEA58(void)
{
    if(mario_is_dive_sliding())
    {
        o->oSmallPenguinUnk100 = o->oAction;
        o->oAction = 3;
    }
}

void ActionSmallPenguin2(void)
{
    s32 sp1C = 0;
    if(o->oTimer == 0)
        if(obj_dist_to_nearest_object_with_behavior(bhvTuxiesMother) < 1000.0f)
            sp1C = 1;
    func_8029ED38(0);
    o->oForwardVel = o->oSmallPenguinUnk104 + 3.0f;
    obj_rotate_yaw_toward(o->oAngleToMario+0x8000,o->oSmallPenguinUnk110+0x600);
    if(o->oDistanceToMario > o->oSmallPenguinUnk108 + 500.0f)
        o->oAction = 0;
    func_802BEA58();
    if(sp1C)
        o->oAction = 5;
}

void ActionSmallPenguin1(void)
{
    func_8029ED38(0);
    o->oForwardVel = o->oSmallPenguinUnk104 + 3.0f;
    obj_rotate_yaw_toward(o->oAngleToMario,o->oSmallPenguinUnk110+0x600);
    if(o->oDistanceToMario < o->oSmallPenguinUnk108 + 300.0f)
        o->oAction = 0;
    if(o->oDistanceToMario > 1100.0f)
        o->oAction = 0;
    func_802BEA58();
}

void ActionSmallPenguin3(void)
{
    if(o->oTimer > 5)
    {
        if(o->oTimer == 6)
            PlaySound2(SOUND_OBJECT_BABYPENGUINTRICK);
        func_8029ED38(1);
        if(o->oTimer > 25)
            if(!mario_is_dive_sliding())
                o->oAction = 4;
    }
}

void ActionSmallPenguin4(void)
{
    if(o->oTimer > 20)
    {
        o->oForwardVel = 0.0f;
        func_8029ED38(2);
        if(o->oTimer > 40)
            o->oAction = o->oSmallPenguinUnk100;
    }
}

void ActionSmallPenguin0(void)
{
    s32 sp1C;

    sp1C = 0;
    func_8029ED38(3);
    if(o->oTimer == 0)
    {
        o->oSmallPenguinUnk110 = (s32)(RandomFloat() * 0x400);
        o->oSmallPenguinUnk108 = RandomFloat() * 100.0f;
        o->oSmallPenguinUnk104 = RandomFloat();
        o->oForwardVel = 0.0f;
        if(obj_dist_to_nearest_object_with_behavior(bhvTuxiesMother) < 1000.0f)
            sp1C = 1;
    }
    if(o->oDistanceToMario < 1000.0f && o->oSmallPenguinUnk108 + 600.0f < o->oDistanceToMario)
        o->oAction = 1;
    else if(o->oDistanceToMario < o->oSmallPenguinUnk108 + 300.0f)
        o->oAction = 2;
    if(sp1C)
        o->oAction = 5;
    if(obj_mario_far_away())
        obj_set_pos_to_home();
}

void ActionSmallPenguin5(void)
{
    f32 sp24;
    s16 sp22;
    struct Object* sp1C = obj_nearest_object_with_behavior(bhvTuxiesMother);
    if(sp1C != NULL)
    {
        if(o->oDistanceToMario < 1000.0f)
            o->oForwardVel = 2.0f;
        else
            o->oForwardVel = 0.0f;
        sp24 = dist_between_objects(o,sp1C);
        sp22 = angle_to_object(o,sp1C);
        if(sp24 > 200.0f)
            obj_rotate_yaw_toward(sp22,0x400);
        else
            obj_rotate_yaw_toward(sp22+0x8000,0x400);
        func_8029ED38(0);
    }
    func_802BEA58();
}

void (*TableSmallPenguinActions[])(void) = {ActionSmallPenguin0,ActionSmallPenguin1,ActionSmallPenguin2,ActionSmallPenguin3,ActionSmallPenguin4,ActionSmallPenguin5};

void func_802BF048(void)
{
    if(o->oUnk88 != 0)
    {
        o->oAction = 5;
        o->oUnk88 = 0;
    }
    obj_update_floor_and_walls();
    obj_call_action_function(TableSmallPenguinActions);
    obj_move_standard(-78);
    play_penguin_walking_sound(0);
}

void bhv_small_penguin_loop(void)
{
    switch(o->oHeldState)
    {
    case 0:
        func_802BF048();
        break;
    case 1:
        func_8029FA5C(0,0);
        if(obj_has_behavior(bhvPenguinBaby))
            set_object_behavior(o,bhvSmallPenguin);
        copy_object_pos(o,gMarioObject);
        if(gGlobalTimer % 30 == 0)
#ifndef VERSION_JP
            SetSound(SOUND_CH9_UNK45,gMarioObject->header.gfx.cameraToObject);
#else
            SetSound(SOUND_CH9_UNK45,o->header.gfx.cameraToObject);
#endif
        break;
    case 2:
        obj_get_thrown_or_placed(0, 0, 0);
        break;
    case 3:
        obj_get_dropped();
        break;
    }
}

/** Geo switch logic for Tuxie's mother's eyes. Cases 0-4. Interestingly, case
  * 4 is unused, and is the eye state seen in Shoshinkai 1995 footage.
  */
s32 geo_switch_tuxie_mother_eyes(s32 run, struct GraphNode *node, UNUSED Mat4 *mtx)
{
    struct Object *obj;
    struct GraphNodeSwitchCase *switchCase;
    s32 timer;

    if(run == TRUE)
    {
        obj = (struct Object *)D_8032CFA0;
        switchCase = (struct GraphNodeSwitchCase *)node;
        switchCase->unk1E = 0;

        // timer logic for blinking. uses cases 0-2.
        timer = gGlobalTimer % 50;
        if(timer < 43)
            switchCase->unk1E = 0;
        else if(timer < 45)
            switchCase->unk1E = 1;
        else if(timer < 47)
            switchCase->unk1E = 2;
        else
            switchCase->unk1E = 1;

        /** make Tuxie's Mother have angry eyes if Mario takes the correct baby
          * after giving it back. The easiest way to check this is to see if she's
          * moving, since she only does when she's chasing Mario.
          */
        if(segmented_to_virtual(bhvTuxiesMother) == obj->behavior)
            if(obj->oForwardVel > 5.0f)
                switchCase->unk1E = 3;
    }
    return 0;
}

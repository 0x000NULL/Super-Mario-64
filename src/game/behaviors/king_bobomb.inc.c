// king_bobomb.c.inc

void bhv_bobomb_anchor_mario_loop(void)
{
    func_802A8D18(50.0f,50.0f,64);
}

void ActionKingBobomb0(void)
{
#ifdef VERSION_US
    o->oForwardVel = 0;
    o->oVelY = 0;
#endif
    if(o->oSubAction == 0)
    {
        obj_become_intangible();
        gSecondCameraFocus = o;
        func_8029ED38(5);
        obj_set_pos_to_home();
        o->oHealth = 3;
        if(obj_is_mario_in_range_and_ready_to_speak(500.0f,100.0f))
        {
            o->oSubAction++;
            func_8031FFB4(0,60,40);
        }
    }
    else if(obj_update_dialogue_unk2(2,1,162,17))
    {
        o->oAction = 2;
        o->oFlags |= OBJ_FLAG_HOLDABLE;
    }
}

int func_802A6AF8(f32 arg0)
{
    if(arg0 < o->oPosY - gMarioObject->oPosY)
        return 1;
    else
        return 0;
}

void ActionKingBobomb2(void)
{
    obj_become_tangible();
    if(o->oPosY - o->oHomeY < -100.0f) // Thrown off hill
    {
        o->oAction =    5;
        obj_become_intangible();
    }
    if(o->OBJECT_FIELD_S32(0x1E) == 0)
    {
        if(obj_check_anim_frame(15))
            ShakeScreen(1);
        if(func_802A4AB0(4))
            o->OBJECT_FIELD_S32(0x1E)++;
    }
    else
    {
        if(o->OBJECT_FIELD_S32(0x1E) == 1)
        {
            func_802A4A70(11,7);
            o->OBJECT_FIELD_S32(0x1E) = 2;
        }
        else
            func_8029ED38(11);
        if(o->OBJECT_FIELD_S32(0x20) == 0)
        {
            o->oForwardVel = 3.0f;
            obj_rotate_yaw_toward(o->oAngleToMario,0x100);
        }
        else
        {
            o->oForwardVel = 0.0f;
            o->OBJECT_FIELD_S32(0x20)--;
        }
    }
    if(obj_check_grabbed_mario())
        o->oAction = 3;
    if(func_802A6AF8(1200.0f))
    {
        o->oAction = 0;
        func_80320CE8(1046);
    }
}


void ActionKingBobomb3(void)
{
    if(o->oSubAction == 0)
    {
        o->oForwardVel = 0;
        o->OBJECT_FIELD_S32(0x1F) = 0;
        o->OBJECT_FIELD_S32(0x1D) = 0;
        if(o->oTimer == 0)
            PlaySound2(SOUND_OBJECT_UNKNOWN3);
        if(func_802A4AB0(0))
        {
            o->oSubAction++;
            func_802A4A70(1,0);
        }
    }
    else
    {
        if(o->oSubAction == 1)
        {
            func_8029ED38(1);
            o->OBJECT_FIELD_S32(0x1D) += player_performed_grab_escape_action();
            print_debug_bottom_up("%d",o->OBJECT_FIELD_S32(0x1D));
            if(o->OBJECT_FIELD_S32(0x1D) > 10)
            {
                o->oUnk88 = 3;
                o->oAction = 2;
                o->OBJECT_FIELD_S32(0x20) = 35;
                o->oInteractStatus &= ~(INT_STATUS_GRABBED_MARIO);
            }
            else
            {
                o->oForwardVel = 3.0f;
                if(o->OBJECT_FIELD_S32(0x1F) > 20 && obj_rotate_yaw_toward(0,0x400))
                {
                    o->oSubAction++;
                    func_802A4A70(9,22);
                }
            }
            o->OBJECT_FIELD_S32(0x1F)++;
        }
        else
        {
            func_8029ED38(9);
            if(obj_check_anim_frame(31))
            {
                o->oUnk88 = 2;
                PlaySound2(SOUND_OBJECT_UNKNOWN4);
            }
            else if(func_8029F788())
            {
                o->oAction = 1;
                o->oInteractStatus &= ~(INT_STATUS_GRABBED_MARIO);
            }
        }
    }
}

void ActionKingBobomb1(void)
{
    o->oForwardVel = 0;
    o->oVelY = 0;
    func_8029ED38(11);
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw,o->oAngleToMario,512);
    if(o->oDistanceToMario < 2500.0f)
        o->oAction = 2;
    if(func_802A6AF8(1200.0f)){
        o->oAction = 0;
        func_80320CE8(1046);
    }
}

void ActionKingBobomb6(void)
{
    if(o->oSubAction == 0)
    {
        if(o->oTimer == 0)
        {
            o->OBJECT_FIELD_S32(0x1F) = 0;
            PlaySound2(SOUND_OBJECT_BULLYTHWOMP);
            PlaySound2(SOUND_CH9_UNK42);
            ShakeScreen(1);
            func_802AA618(0,0,100.0f);
            o->oInteractType = 8;
            obj_become_tangible();
        }
        if(func_802A4AB0(2))
            o->OBJECT_FIELD_S32(0x1F)++;
        if(o->OBJECT_FIELD_S32(0x1F) > 3)
        {
            o->oSubAction++;
            ; // Needed to match
        }
    }
    else
    {
        if(o->oSubAction == 1)
        {
            if(func_802A4AB0(10))
            {
                o->oSubAction++;
                o->oInteractType = 2;
                obj_become_intangible();
            }
        }
        else
        {
            func_8029ED38(11);
            if(obj_rotate_yaw_toward(o->oAngleToMario,0x800) == 1)
                o->oAction = 2;
        }
    }
}


void ActionKingBobomb7(void)
{
    func_8029ED38(2);
    if(obj_update_dialogue_unk2(2,2,162,116))
    {
        create_sound_spawner(SOUND_OBJECT_KINGWHOMPDEATH);
        obj_hide();
        obj_become_intangible();
        func_802AA618(0,0,200.0f);
        spawn_triangle_break_particles(20,138,3.0f,4);
        ShakeScreen(1);
#ifndef VERSION_JP
        obj_spawn_star_at_y_offset(2000.0f, 4500.0f, -4500.0f, 200.0f);
#else
        o->oPosY += 100.0f;
        CreateStar(2000.0f,4500.0f,-4500.0f);
#endif
        o->oAction = 8;
    }
}

void ActionKingBobomb8(void)
{
    if(o->oTimer == 60)
        func_80320CE8(1046);
}

void ActionKingBobomb4() // bobomb been thrown
{
    if(o->oPosY - o->oHomeY > -100.0f) // not thrown off hill
    {
        if(o->oMoveFlags & 1)
        {
            o->oHealth--;
            o->oForwardVel = 0;
            o->oVelY = 0;
            PlaySound2(SOUND_OBJECT_BULLYTHWOMP);
            if(o->oHealth)
                o->oAction = 6;
            else
                o->oAction = 7;
        }
    }
    else
    {
        if(o->oSubAction == 0)
        {
            if(o->oMoveFlags & 2)
            {
                o->oForwardVel = 0;
                o->oVelY = 0;
                o->oSubAction++;
            }
            else if(o->oMoveFlags & 1)
                PlaySound2(SOUND_OBJECT_BULLYTHWOMP);
        }
        else
        {
            if(func_802A4AB0(10))
                o->oAction = 5; // Go back to top of hill
            o->oSubAction++;
        }
    }
}

void ActionKingBobomb5() // bobomb returns home
{
    switch(o->oSubAction)
    {
    case 0:
        if(o->oTimer == 0)
            PlaySound2(SOUND_OBJECT_KINGBOBOMBJUMP);
        o->OBJECT_FIELD_S32(0x1C) = 1;
        func_802A4AEC(8);
        o->oMoveAngleYaw = obj_angle_to_home();
        if(o->oPosY < o->oHomeY)
            o->oVelY = 100.0f;
        else
        {
            func_802B2894(&o->oHomeX,&o->oPosX,100.0f,-4.0f); // the Vec3fs oHome and oPos are being passed by reference
            o->oSubAction++;
        }
        break;
    case 1:
        func_802A4AEC(8);
        if(o->oVelY < 0 && o->oPosY < o->oHomeY)
        {
            o->oPosY = o->oHomeY;
            o->oVelY = 0;
            o->oForwardVel = 0;
            o->oGravity = -4.0f;
            o->OBJECT_FIELD_S32(0x1C) = 0;
            func_8029ED38(7);
            PlaySound2(SOUND_OBJECT_BULLYTHWOMP);
            ShakeScreen(1);
            o->oSubAction++;
        }
        break;
    case 2:
        if(func_802A4AB0(7))
            o->oSubAction++;
        break;
    case 3:
        if(func_802A6AF8(1200.0f))
        {
            o->oAction = 0;
            func_80320CE8(1046);
        }
        if(obj_is_mario_in_range_and_ready_to_speak(500.0f,100.0f))
            o->oSubAction++;
        break;
    case 4:
        if(obj_update_dialogue_unk2(2,1,162,128))
            o->oAction = 2;
        break;
    }
}

void (*TableKingBobombActions[])(void) = {ActionKingBobomb0,ActionKingBobomb1,ActionKingBobomb2,ActionKingBobomb3,ActionKingBobomb4,ActionKingBobomb5,ActionKingBobomb6,ActionKingBobomb7,ActionKingBobomb8};
struct SoundState TableKingBobombSoundStates[] = {
    {0, 0,  0,  NO_SOUND},
    {1, 1,  20, SOUND_OBJECT_POUNDING1_HIGHPRIO},
    {0, 0,  0,  NO_SOUND},
    {0, 0,  0,  NO_SOUND},
    {1, 15, -1, SOUND_OBJECT_POUNDING1_HIGHPRIO},
    {0, 0,  0,  NO_SOUND},
    {0, 0,  0,  NO_SOUND},
    {0, 0,  0,  NO_SOUND},
    {0, 0,  0,  NO_SOUND},
    {1, 33, -1, SOUND_OBJECT_POUNDING1_HIGHPRIO},
    {0, 0,  0,  NO_SOUND},
    {1, 1,  15, SOUND_OBJECT_POUNDING1_HIGHPRIO}
};

void func_802A7748(void)
{
    obj_update_floor_and_walls();
    if(o->OBJECT_FIELD_S32(0x1C) == 0)
        obj_move_standard(-78);
    else
        obj_move_using_fvel_and_gravity();
    obj_call_action_function(TableKingBobombActions);
    exec_anim_sound_state(TableKingBobombSoundStates);
    if(o->oDistanceToMario < 5000.0f)
        obj_enable_rendering();
    else
        obj_disable_rendering();
}

void bhv_king_bobomb_loop(void)
{
    f32 sp34 = 20.0f;
    f32 sp30 = 50.0f;
    UNUSED u8 pad[8];
    o->oUnk190 |= 4;
    switch(o->oHeldState)
    {
    case HELD_FREE:
        func_802A7748();
        break;
    case HELD_HELD:
        func_8029FA5C(6,1);
        break;
    case HELD_THROWN:
    case HELD_DROPPED:
        obj_get_thrown_or_placed(sp34,sp30,4);
        obj_become_intangible();
        o->oPosY += 20.0f;
        break;
    }
    o->oInteractStatus = 0;
}

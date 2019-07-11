// piranha_plant.c.inc

void ActionPiranhaPlant20(void)
{
    obj_become_intangible();
    func_8029ED38(8);
#ifdef VERSION_US
    obj_scale(1);
#endif
    if(o->oDistanceToMario < 1200.0f)
        o->oAction = 1;
}

s32 func_802BD4A8(void)
{
    s32 i;
    s32 sp18 = 1;
    if(o->oInteractStatus & INT_STATUS_INTERACTED)
    {
        func_80321080(50);
        if(o->oInteractStatus & INT_STATUS_WAS_ATTACKED)
        {
            PlaySound2(SOUND_CH9_UNK11);
            for(i=0;i<20;i++)
                spawn_object(o,MODEL_PURPLE_MARBLE,bhvGiantPiranhaParticle);
            o->oAction = 5;
        }
        else
            o->oAction = 3;
        o->oInteractStatus = 0;
    }
    else
        sp18 = 0;
    return sp18;
}

void ActionPiranhaPlant21(void)
{
    obj_become_tangible();
    o->oInteractType = 0x8000;
    func_8029ED38(8);
    obj_set_hitbox_radius_and_height(250.0f,200.0f);
    obj_set_hurtbox_radius_and_height(150.0f,100.0f);
#ifdef VERSION_US
    o->oDamageOrCoinValue = 0;
#endif
    if(o->oDistanceToMario < 400.0f)
    {
        if(func_802BDAD4())
            o->oAction = 3;
    }
    else if(o->oDistanceToMario < 1000.0f)
    {
        func_80320F84(11,0,255,1000);
        o->oPiranhaPlant2UnkF4 = 0;
    }
    else if(o->oPiranhaPlant2UnkF4 == 0)
    {
        o->oPiranhaPlant2UnkF4++;
        func_80321080(0x32);
    }
    func_802BD4A8();
}

void ActionPiranhaPlant23(void)
{
#ifdef VERSION_US
    o->oDamageOrCoinValue = 3;
#endif
    if(o->oTimer == 0)
        func_80321080(50);
    if(func_802BD4A8() == 0)
        if(o->oTimer > 10)
            o->oAction = 2;
}

#ifdef VERSION_US
void func_u_802BE0B8(void)
{
    if(o->activeFlags & 2)
        o->oAction = 0;
}
#endif

void ActionPiranhaPlant25(void)
{
    obj_become_intangible();
    func_8029ED38(2);
    o->oInteractStatus = 0;
    if(func_8029F788())
        o->oAction = 6;
#ifdef VERSION_US
    func_u_802BE0B8();
#endif
}

void ActionPiranhaPlant26(void)
{
    if(o->oTimer == 0)
    {
        PlaySound2(SOUND_OBJECT_ENEMYDEFEATSHRINK);
        o->oPiranhaPlant2UnkF8 = 1.0f;
    }
    if(o->oPiranhaPlant2UnkF8 > 0.0f)
        o->oPiranhaPlant2UnkF8 = o->oPiranhaPlant2UnkF8 - 0.04;
    else
    {
        o->oPiranhaPlant2UnkF8 = 0.0f;
        obj_spawn_loot_blue_coin();
        o->oAction = 7;
    }
    obj_scale(o->oPiranhaPlant2UnkF8);
#ifdef VERSION_US
    func_u_802BE0B8();
#endif
}

void ActionPiranhaPlant27(void)
{
    if(o->oDistanceToMario > 1200.0f)
        o->oAction = 8;
}

void ActionPiranhaPlant28(void)
{
    func_8029ED38(8);
    if(o->oTimer == 0)
        o->oPiranhaPlant2UnkF8 = 0.3f;
    if(o->oPiranhaPlant2UnkF8 < 1.0)
        o->oPiranhaPlant2UnkF8 += 0.02;
    else
    {
        o->oPiranhaPlant2UnkF8 = 1.0f;
        o->oAction = 0;
    }
    obj_scale(o->oPiranhaPlant2UnkF8);
}

void ActionPiranhaPlant22(void)
{

    s32 frame = o->header.gfx.unk38.animFrame;
    obj_become_tangible();
    o->oInteractType = 8;
    func_8029ED38(0);
    obj_set_hitbox_radius_and_height(150.0f,100.0f);
    obj_set_hurtbox_radius_and_height(150.0f,100.0f);
    if(item_in_array(frame,D_8032F998))
        PlaySound2(SOUND_CH9_UNK10);
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x400);
    if(o->oDistanceToMario > 500.0f)
        if(func_8029F788())
            o->oAction = 4;
    if(o->oInteractStatus & INT_STATUS_INTERACTED)
        if(gMarioState->flags & 4)
            o->oAction = 5;
}
s32 func_802BDAD4(void)
{
    if(gMarioStates->vel[1] > 10.0f)
        return 1;
    if(gMarioStates->forwardVel > 10.0f)
        return 1;
    return 0;
}

void ActionPiranhaPlant24(void)
{
    obj_become_intangible();
    func_8029ED38(6);
    if(func_8029F788())
        o->oAction = 1;
    if(o->oDistanceToMario < 400.0f)
        if(func_802BDAD4())
            o->oAction = 2;
}

void (*TablePiranhaPlantActions[])(void) = {ActionPiranhaPlant20,ActionPiranhaPlant21,ActionPiranhaPlant22,ActionPiranhaPlant23,ActionPiranhaPlant24,ActionPiranhaPlant25,ActionPiranhaPlant26,ActionPiranhaPlant27,ActionPiranhaPlant28};
s8 D_8032F9C4[] = {2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,-1};
s8 D_8032F9E0[] = {2,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,-1};
s8 D_8032F9FC[] = {2,2,2,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6,2,2,2,-1};
s8 D_8032FA18[] = {2,2,2,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,-1};
s8 D_8032FA34[] = {2,2,2,2,2,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6,2,2,2,2,2,-1};
s8 D_8032FA50[] = {2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,-1};
s8 D_8032FA6C[] = {2,2,2,2,2,2,2,5,2,2,2,2,2,2,2,2,2,2,6,2,2,2,2,2,2,2,-1};
s8 D_8032FA88[] = {2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,2,-1};
s8 D_8032FAA4[] = {2,2,2,2,2,2,2,2,2,6,2,2,2,2,2,2,5,2,2,2,2,2,2,2,2,2,-1};
s8 D_8032FAC0[] = {2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,3,2,2,2,2,2,2,2,2,2,2,-1};
s8 D_8032FADC[] = {2,2,2,2,2,2,2,2,2,2,2,6,2,2,5,2,2,2,2,2,2,2,2,2,2,2,-1};
s8 D_8032FAF8[] = {2,2,2,2,2,2,2,2,2,2,2,2,3,4,2,2,2,2,2,2,2,2,2,2,2,2,-1};
s8 D_8032FB14[] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-1};
s8 D_8032FB30[] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-1};

struct Struct8032FB4C D_8032FB4C[] = {{MODEL_LLL_BOWSER_PIECE_1,251,241,1,D_8032F9C4},{MODEL_LLL_BOWSER_PIECE_2,5,241,0,D_8032F9E0},{MODEL_LLL_BOWSER_PIECE_3,241,251,0,D_8032FADC},{MODEL_LLL_BOWSER_PIECE_4,251,251,0,D_8032FAF8},{MODEL_LLL_BOWSER_PIECE_5,5,251,0,D_8032F9FC},{MODEL_LLL_BOWSER_PIECE_6,15,251,0,D_8032FA18},{MODEL_LLL_BOWSER_PIECE_7,241,5,0,D_8032FAC0},{MODEL_LLL_BOWSER_PIECE_8,251,5,0,D_8032FAA4},{MODEL_LLL_BOWSER_PIECE_9,5,5,0,D_8032FA50},{MODEL_LLL_BOWSER_PIECE_10,15,5,0,D_8032FA34},{MODEL_LLL_BOWSER_PIECE_11,241,15,0,D_8032FB14},{MODEL_LLL_BOWSER_PIECE_12,251,15,0,D_8032FA88},{MODEL_LLL_BOWSER_PIECE_13,5,15,0,D_8032FA6C},{MODEL_LLL_BOWSER_PIECE_14,15,15,0,D_8032FB30}};

void bhv_piranha_plant_2_loop(void)
{
    obj_call_action_function(TablePiranhaPlantActions);
    if(gCurrLevelNum == LEVEL_WF)
    {
        if(gMarioObject->oPosY > 3400.0f)
            obj_hide();
        else
            obj_unhide();
    }
    o->oInteractStatus = 0;
}

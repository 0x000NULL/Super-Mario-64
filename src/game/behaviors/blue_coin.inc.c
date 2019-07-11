// blue_coin.c.inc

void bhv_hidden_blue_coins_loop(void)
{
    struct Object* coinSwitch;
    switch(o->oAction)
    {
    case 0:
        obj_disable_rendering();
        obj_become_intangible();
        o->oHiddenBlueCoinsUnkF8 = obj_nearest_object_with_behavior(bhvBlueCoinSwitch);
        if(o->oHiddenBlueCoinsUnkF8 != NULL)
            o->oAction++;
        break;
    case 1:
        coinSwitch = o->oHiddenBlueCoinsUnkF8;
        if(coinSwitch->oAction == 2)
            o->oAction++;
        break;
    case 2:
        obj_enable_rendering();
        obj_become_tangible();
        if(o->oInteractStatus & INT_STATUS_INTERACTED)
        {
            spawn_object(o,MODEL_SPARKLES,bhvGoldenCoinSparkles);
            mark_object_for_deletion(o);
        }
        if(obj_wait_then_blink(200,20))
            mark_object_for_deletion(o);
        break;
    }
    o->oInteractStatus = 0;
}

void bhv_blue_coin_switch_loop(void)
{
    obj_scale(3.0f);
    switch(o->oAction)
    {
    case 0:
        if(gMarioObject->platform == o)
            if(gMarioStates->action == ACT_GROUND_POUND_LAND)
            {
                o->oAction++;
                o->oVelY = -20.0f;
                o->oGravity = 0.0f;
                PlaySound2(SOUND_GENERAL_SWITCHDOOROPEN);
            }
        load_object_collision_model();
        break;
    case 1:
        if(o->oTimer > 5)
        {
            obj_hide();
            o->oAction++;
            o->oPosY = gMarioObject->oPosY - 40.0f;
            func_802AA618(0,0,46.0f);
        }
        else
        {
            load_object_collision_model();
            obj_move_using_fvel_and_gravity();
        }
        break;
    case 2:
        if(o->oTimer < 200)
            SetSound(SOUND_CH8_UNK54,D_803320E0);
        else
            SetSound(SOUND_CH8_UNK55,D_803320E0);
        if(obj_nearest_object_with_behavior(bhvHiddenBlueCoin) == 0 || o->oTimer > 240)
            mark_object_for_deletion(o);
        break;
    }
}

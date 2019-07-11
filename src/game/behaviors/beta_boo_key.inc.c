// beta_boo_key.c.inc

void bhv_beta_boo_key_outside_loop(void)
{
    o->oFaceAngleRoll += 0x200;
    o->oFaceAngleYaw    += 0x200;
    if(are_objects_collided(o,gMarioObject))
    {
        o->parentObj->oUnk88 = 1;
        mark_object_for_deletion(o);
        spawn_object(o,MODEL_SPARKLES,bhvGoldenCoinSparkles);
    }
}

void ActionBetaBooKeyInside2(void)
{
    obj_update_floor_and_walls();
    obj_move_standard(78);
    if(o->oGraphYOffset < 26.0f)
        o->oGraphYOffset += 2.0f;
    if(o->oFaceAngleRoll & 0xFFFF) // is it not a full rotation
    {
        o->oFaceAngleRoll &= 0xF800;
        o->oFaceAngleRoll += 0x800;
    }
    if(o->oMoveFlags & 2)
    {
        o->oVelX = 0.0f;
        o->oVelZ = 0.0f;
    }
    o->oFaceAngleYaw += 0x800;
    if(o->oTimer > 90 || o->oMoveFlags & 1)
    {
        obj_become_tangible();
        if(are_objects_collided(o,gMarioObject))
        {
            o->parentObj->oInteractStatus = INTERACT_HOOT; //! This inteaction could had an early action in the beta
            mark_object_for_deletion(o);
            spawn_object(o,MODEL_SPARKLES,bhvGoldenCoinSparkles);
        }
    }
}

void ActionBetaBooKeyInside1(void)
{
    s16 sp26;
    f32 sp20;
    struct Object* parent = o->parentObj;
    copy_object_pos(o,parent);
    if(o->oTimer == 0)
    {
        o->parentObj = parent->parentObj;
        o->oAction = 2;
        sp26 = gMarioObject->oMoveAngleYaw;
        sp20 = 3.0f;
        o->oVelX = sins(sp26) * sp20;
        o->oVelZ = coss(sp26) * sp20;
        o->oVelY = 40.0f;
    }
    o->oFaceAngleYaw    += 0x200;
    o->oFaceAngleRoll += 0x200;
}

void ActionBetaBooKeyInside0(void)
{
    struct Object* parent = o->parentObj;
    copy_object_pos(o,parent);
    o->oPosY += 40.0f;
    if(parent->oUnk88 != 0)
        o->oAction = 1;
    o->oFaceAngleRoll += 0x200;
    o->oFaceAngleYaw    += 0x200;
}

void (*TableBetaBooKeyInsideActions[])(void) = {ActionBetaBooKeyInside0,ActionBetaBooKeyInside1,ActionBetaBooKeyInside2};

void bhv_beta_boo_key_inside_loop(void)
{
    obj_call_action_function(TableBetaBooKeyInsideActions);
}

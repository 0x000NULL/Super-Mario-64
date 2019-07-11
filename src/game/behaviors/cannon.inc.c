// cannon.c.inc

void bhv_cannon_base_unused_loop(void)
{
    o->oPosY += o->oVelY;
}

void ActionOpenedCannon0(void)
{
    if(o->oTimer == 0)
    {
        o->oInteractStatus = 0;
        o->oPosX = o->oHomeX;
        o->oPosY = o->oHomeY;
        o->oPosZ = o->oHomeZ;
        o->oMoveAnglePitch = 0;
        o->oMoveAngleYaw = (s16)(o->oBehParams2ndByte << 8);
        o->OBJECT_FIELD_S32(0x1B) = 0;
        o->OBJECT_FIELD_S32(0x21) = 0;
        obj_enable_rendering();
        obj_become_tangible();
    }
    if(o->oDistanceToMario < 500.0f)
    {
        obj_become_tangible();
        obj_enable_rendering();
        if(o->oInteractStatus & INT_STATUS_INTERACTED && (!(o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB))) // bob-omb explodes when it gets into a cannon
        {
            o->oAction = 4;
            o->OBJECT_FIELD_S32(0x21) = 1;
            o->OBJECT_FIELD_S32(0x1C) = 1;
        }
        else
            o->oInteractStatus = 0;
    }
    else
    {
        obj_become_intangible();
        obj_disable_rendering();
        o->OBJECT_FIELD_S32(0x21) = 0;
    }
}

void ActionOpenedCannon4(void)
{
    if(o->oTimer == 0)
        PlaySound2(SOUND_OBJECT_CANNON1);
    o->oPosY += 5.0f;
    o->oPosX += (f32)((o->oTimer/2 & 1) - 0.5) * 2.0f;
    o->oPosZ += (f32)((o->oTimer/2 & 1) - 0.5) * 2.0f;
    if(o->oTimer > 67)
    {
        o->oPosX += (f32)((o->oTimer/2 & 1) - 0.5) * 4.0f;
        o->oPosZ += (f32)((o->oTimer/2 & 1) - 0.5) * 4.0f;
        o->oAction = 6;
    }
}

void ActionOpenedCannon6(void)
{
    if(o->oTimer == 0)
        PlaySound2(SOUND_OBJECT_CANNON2);
    if(o->oTimer < 4)
    {
        o->oPosX += (f32)((o->oTimer/2 & 1) - 0.5) * 4.0f;
        o->oPosZ += (f32)((o->oTimer/2 & 1) - 0.5) * 4.0f;
    }
    else
    {
        if(o->oTimer < 6) {}
        else
        {
            if(o->oTimer < 22)
            {
                o->oMoveAngleYaw = sins(o->OBJECT_FIELD_S32(0x1B)) * 0x4000 + ((s16)(o->oBehParams2ndByte << 8));
                o->OBJECT_FIELD_S32(0x1B) += 0x400;
            }
            else if(o->oTimer < 26) {}
            else
            {
                o->OBJECT_FIELD_S32(0x1B) = 0;
                o->oAction = 5;
            }
        }
    }
}

void ActionOpenedCannon5(void)
{
    if(o->oTimer == 0)
        PlaySound2(SOUND_OBJECT_CANNON3);
    if(o->oTimer < 4) {}
    else
    {
        if(o->oTimer < 20)
        {
            o->OBJECT_FIELD_S32(0x1B) += 0x400;
            o->oMoveAnglePitch = sins(o->OBJECT_FIELD_S32(0x1B)) * 0x2000;
        }
        else if(o->oTimer < 25) {}
        else
            o->oAction = 1;
    }
}

void ActionOpenedCannon1(void)
{
    UNUSED s32 unused;
    obj_become_intangible();
    obj_disable_rendering();
    o->OBJECT_FIELD_S32(0x21) = 0;
    gMarioShotFromCannon = 1;
}

void ActionOpenedCannon2(void)
{
    o->oAction = 3;
}

void ActionOpenedCannon3(void)
{
    UNUSED s32 unused;
    if(o->oTimer > 3)
        o->oAction = 0;
}

void (*TableOpenedCannonActions[])(void) =    {ActionOpenedCannon0,ActionOpenedCannon1,ActionOpenedCannon2,ActionOpenedCannon3,ActionOpenedCannon4,ActionOpenedCannon5,ActionOpenedCannon6};

u8 unused0EA1FC[] = {2,0,0,0,0,0,0,0,63,128,0,0,2,0,0,0,65,32,0,0,63,128,0,0,2,0,0,0,65,160,0,0,63,128,0,0,2,0,0,0,65,160,0,0,63,128,0,0,8,0,0,0,65,32,0,0,63,128,0,0};

void bhv_cannon_base_loop(void)
{
    obj_call_action_function(TableOpenedCannonActions);
    if(o->OBJECT_FIELD_S32(0x1C))
        o->OBJECT_FIELD_S32(0x1C)++;
    o->oInteractStatus = 0;
}

void bhv_cannon_barrel_loop(void)
{
    struct Object* parent = o->parentObj;
    if(parent->header.gfx.node.flags & GRAPH_RENDER_01)
    {
        obj_enable_rendering();
        copy_object_pos(o,o->parentObj);
        o->oMoveAngleYaw = o->parentObj->oMoveAngleYaw;
        o->oFaceAnglePitch = o->parentObj->oMoveAnglePitch;
    }
    else
        obj_disable_rendering();
}

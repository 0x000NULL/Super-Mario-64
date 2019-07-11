// piranha_bubbles.c.inc

void bhv_piranha_plant_waking_bubbles_loop(void)
{
    if(o->oTimer == 0)
    {
        o->oVelY = RandomFloat() * 10.0f + 5.0f;
        o->oForwardVel = RandomFloat() * 10.0f + 5.0f;
        o->oMoveAngleYaw = RandomU16();
    }
    obj_move_using_fvel_and_gravity();
}

void bhv_piranha_plant_bubble_loop(void)
{
    struct Object* parent = o->parentObj;
    f32 scale = 0;
    s32 i;
    s32 sp40 = parent->header.gfx.unk38.animFrame;
    s32 sp3C = parent->header.gfx.unk38.curAnim->unk08 - 2;
    s32 UNUSED unused;
    f32 sp34;
    f32 sp30;
    obj_set_pos_relative(parent,0,72.0f,180.0f);
    switch(o->oAction)
    {
    case 0:
        obj_disable_rendering();
        scale = 0;
        if(parent->oAction == 1)
            o->oAction++;
        break;
    case 1:
        if(parent->oDistanceToMario < parent->oDrawingDistance)
        {
            obj_enable_rendering();
            if(parent->oAction == 1)
            {
                sp34 = sp3C/2.0f - 4.0f;
                sp30 = sp3C/2.0f + 4.0f;
                if(sp40 < sp34)
                    scale = coss(sp40/sp34 * 0x4000) * 4.0f + 1.0;
                else if(sp40 > sp30)
                    scale = sins((sp40 - (sp3C/2.0f + 4.0f))/sp30 * 0x4000) * 4.0f + 1.0;
                else
                    scale = 1.0f;
            }
            else
                o->oAction++;
        }
        else
            obj_disable_rendering();
        break;
    case 2:
        obj_disable_rendering();
        scale = 0;
        for(i=0;i<15;i++)
            try_to_spawn_object(0,1.0f,o,MODEL_BUBBLE,bhvPiranhaPlantWakingBubbles);
        o->oAction = 0;
        scale = 1.0f;
        break;
    }
    obj_scale(scale);
}

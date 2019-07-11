// flame_mario.c.inc

void bhv_black_smoke_upward_loop(void)
{
    spawn_object_with_scale(o,MODEL_BURN_SMOKE,bhvBlackSmokeBowser,o->header.gfx.scale[0]);
}

void bhv_black_smoke_bowser_loop(void)
{
    if(o->oTimer == 0)
    {
        o->oForwardVel = RandomFloat() * 2.0f + 0.5;
        o->oMoveAngleYaw = RandomU16();
        o->oVelY = 8.0f;
        o->oBlackSmokeBowserUnkF4 = o->header.gfx.scale[0];
    }
    o->oMoveAngleYaw += o->oAngleVelYaw;
    o->oPosY += o->oVelY;
}

void bhv_black_smoke_mario_loop(void)
{
    if(o->oTimer == 0)
    {
        obj_set_pos_relative(gMarioObject,0,0,-30.0f);
        o->oForwardVel = RandomFloat() * 2.0f + 0.5;
        o->oMoveAngleYaw = (gMarioObject->oMoveAngleYaw + 0x7000) + RandomFloat() * 8192.0f;
        o->oVelY = 8.0f;
    }
    o->oMoveAngleYaw += o->oAngleVelYaw;
    o->oPosY += o->oVelY;
}

void bhv_flame_mario_loop(void)
{
    obj_scale(2.0f);
    if(o->oTimer != 0)
        if(o->oTimer&1)
            spawn_object(o,MODEL_BURN_SMOKE,bhvBlackSmokeMario);
    gMarioObject->prevObj = o; // weird?
    set_object_parent_relative_pos(o,40,-120,0);
    if(!(gMarioObject->oMarioParticleFlags & 0x800))
    {
        o->parentObj->oActiveParticleFlags &= ~0x800;
        mark_object_for_deletion(o);
        gMarioObject->prevObj = NULL;
    }
}

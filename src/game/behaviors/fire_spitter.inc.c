
static void fire_spitter_act_idle(void)
{
    approach_f32_ptr(&o->header.gfx.scale[0], 0.2f, 0.002f);
    if (o->oTimer > 150 && o->oDistanceToMario < 800.0f && !(o->oMoveFlags & 0x00000078))
    {
        o->oAction = FIRE_SPITTER_ACT_SPIT_FIRE;
        o->oFireSpitterScaleVel = 0.05f;
    }
}

static void fire_spitter_act_spit_fire(void)
{
    s32 scaleStatus;

    o->oMoveAngleYaw = o->oAngleToMario;

    // Increase scale by 0.05, 0.04, ..., -0.03. Then wait ~8 frames, then
    // starting moving scale by 0.05 each frame toward 0.1. The first time
    // it becomes below 0.15 during this latter portion, shoot fire.
    scaleStatus = obj_grow_then_shrink(&o->oFireSpitterScaleVel, 0.15f, 0.1f);
    if (scaleStatus != 0)
    {
        if (scaleStatus < 0)
        {
            o->oAction = FIRE_SPITTER_ACT_IDLE;
        }
        else
        {
            PlaySound2(SOUND_OBJECT_FLAMEBLOWN);
            obj_spit_fire(0, 0, 0, 5.0f, MODEL_RED_FLAME_SHADOW, 20.0f, 15.0f, 0x1000);
        }
    }
}

void bhv_fire_spitter_update(void)
{
    obj_scale(o->header.gfx.scale[0]);
    o->oGraphYOffset = 40.0f;
    obj_update_floor_and_walls();

    switch (o->oAction)
    {
    case FIRE_SPITTER_ACT_IDLE:      fire_spitter_act_idle();      break;
    case FIRE_SPITTER_ACT_SPIT_FIRE: fire_spitter_act_spit_fire(); break;
    }

    obj_move_standard(78);
}

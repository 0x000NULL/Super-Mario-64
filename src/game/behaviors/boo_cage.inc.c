// boo_cage.c.inc

struct ObjectHitbox sBooCageHitbox =
{
    /* interactType: */      INTERACT_BBH_ENTRANCE,
    /* downOffset: */        0,
    /* damageOrCoinValue: */ 0,
    /* health: */            0,
    /* numLootCoins: */      0,
    /* radius: */            120,
    /* height: */            300,
    /* hurtboxRadius: */     0,
    /* hurtboxHeight: */     0,
};

void bhv_boo_cage_loop(void)
{
    UNUSED s32 unused;
    set_object_hitbox(o,&sBooCageHitbox);
    switch(o->oAction)
    {
    case 0:
        obj_become_intangible();
        obj_scale(1.0f);
        if(o->parentObj->oUnk88 != 0)
        {
            o->oAction++;
            o->oVelY = 60.0f;
            play_puzzle_jingle();
        }
        else
            copy_object_pos_and_angle(o,o->parentObj);
        break;
    case 1:
        o->oFaceAnglePitch = 0;
        o->oFaceAngleRoll = 0;
        obj_update_floor_and_walls();
        obj_move_standard(-78);
        spawn_object(o,MODEL_NONE,bhvPowerupSparkles2);
        if(o->oMoveFlags & 1)
            PlaySound2(SOUND_GENERAL_SOFTLANDING);
        if(o->oMoveFlags & (0x40 | 0x10 | 0x2))
            o->oAction++;
        break;
    case 2:
        obj_become_tangible();
        obj_scale(1.0f);
        if(are_objects_collided(o,gMarioObject))
            o->oAction++;
        break;
    case 3:
        if(o->oTimer > 100)
            o->oAction++;
        break;
    case 4:
        break;
    }
}

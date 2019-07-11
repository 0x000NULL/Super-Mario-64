// pyramid_top.c.inc

void bhv_pyramid_top_init(void) {
    spawn_object_abs_with_rot(o, 0, MODEL_NONE, bhvCollisionBoxSubbehavior, 1789, 1024, 764, 0, 0, 0);
    spawn_object_abs_with_rot(o, 0, MODEL_NONE, bhvCollisionBoxSubbehavior, 1789, 896, -2579, 0, 0, 0);
    spawn_object_abs_with_rot(o, 0, MODEL_NONE, bhvCollisionBoxSubbehavior, -5883, 1024, -2579, 0, 0, 0);
    spawn_object_abs_with_rot(o, 0, MODEL_NONE, bhvCollisionBoxSubbehavior, -5883, 1024, 764, 0, 0, 0);
}

void func_802EF238(void) {
    struct Object *sp1C;

    o->oPosX = o->oHomeX + sins(o->oTimer * 0x4000) * 40.0f;
    if (o->oTimer < 60) {
        o->oPosY = o->oHomeY + absf_2(sins(o->oTimer * 0x2000) * 10.0f);
    }
    else {
        o->oAngleVelYaw += 0x100;
        if (o->oAngleVelYaw > 0x1800) {
            o->oAngleVelYaw = 0x1800;
            o->oVelY = 5.0f;
        }

        o->oFaceAngleYaw += o->oAngleVelYaw;
        o->oPosY += o->oVelY;
    }

    if (o->oTimer < 90) {
        sp1C = spawn_object(o, MODEL_DIRT_ANIMATION, bhvPyramidTopExplosion);
        sp1C->oForwardVel = RandomFloat() * 10.0f + 20.0f;
        sp1C->oMoveAngleYaw = RandomU16();
        sp1C->oUnknownUnkF4_F32 = 0.8f;
        sp1C->oGravity = RandomFloat() + 2.0f;
    }

    if (o->oTimer == 150)
        o->oAction = 2;
}

void func_802EF450(void) {
    struct Object *sp1C;
    s16 sp1A;

    func_802AA618(0, 0, 690.0f);

    for (sp1A = 0; sp1A < 30; sp1A++) {
        sp1C = spawn_object(o, MODEL_DIRT_ANIMATION, bhvPyramidTopExplosion);
        sp1C->oForwardVel = RandomFloat() * 50.0f + 80.0f;
        sp1C->oVelY = RandomFloat() * 80.0f + 20.0f;
        sp1C->oMoveAngleYaw = RandomU16();
        sp1C->oUnknownUnkF4_F32 = 3.0f;
        sp1C->oGravity = RandomFloat() * 2.0f + 5.0f;
    }

    o->activeFlags = 0;
}

void bhv_pyramid_top_loop(void) {
    switch(o->oAction) {
        case 0:
            if (o->oPyramidTopUnkF4 == 4) {
                play_puzzle_jingle();
                o->oAction = 1;
            }
            break;

        case 1:
            if (o->oTimer == 0) 
                PlaySound2(SOUND_CH8_UNK4B);
            
            func_802EF238();
            break;

        case 2:
            if (o->oTimer == 0) 
                create_sound_spawner(SOUND_CH8_UNK4C);

            func_802EF450();
            break;
    }
}

void bhv_pyramid_top_explosion_init(void) {
    o->oFriction = 0.999f;
    o->oBuoyancy = 2.0f;
    o->oAnimState = 3;
    obj_scale(o->oPyramidTopExplosionUnkF4);
}

void bhv_pyramid_top_explosion_loop(void) {
    ObjectStep();
    o->oFaceAngleYaw += 0x1000;
    o->oFaceAnglePitch += 0x1000;

    if (o->oTimer == 60) 
        o->activeFlags = 0;
} 

void bhv_collision_box_sub_bhvavior_loop(void) {
    obj_become_tangible();
    if (are_objects_collided(o, gMarioObject) == 1) {
        o->parentObj->oUnknownUnkF4_S32++;
        o->activeFlags = 0;
    }
}

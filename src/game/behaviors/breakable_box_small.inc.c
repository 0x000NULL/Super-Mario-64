// breakable_box.c.inc


struct ObjectHitbox sBreakableBoxSmallHitbox =
{
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 20,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 250,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 250,
};

void bhv_breakable_box_small_init(void) {
    o->oGravity = 2.5f;
    o->oFriction = 0.99f;
    o->oBuoyancy = 1.4f;
    obj_scale(0.4f);
    set_object_hitbox(o, &sBreakableBoxSmallHitbox);
    o->oAnimState = 1;
    o->activeFlags |= 0x200;
}

void func_802F4CE8(void) {
    struct Object *sp24 = spawn_object(o, MODEL_SMOKE, bhvSmoke);
    sp24->oPosX += (s32)(RandomFloat() * 80.0f) - 40;
    sp24->oPosZ += (s32)(RandomFloat() * 80.0f) - 40;
}

void func_802F4DB4(void) {
    s16 sp1E = ObjectStep();

    attack_collided_non_mario_object(o);
    if (sp1E == 1)
        PlaySound2(SOUND_GENERAL_BOXLANDING_2);
    if (sp1E & 1) {
        if (o->oForwardVel > 20.0f) {
            PlaySound2(SOUND_ENVIRONMENT_SLIDING);
            func_802F4CE8();
        }
    }

    if (sp1E & 2) {
        func_802A3004();
        spawn_triangle_break_particles(20, 138, 0.7f, 3);
        ObjSpawnYellowCoins(o, 3);
        create_sound_spawner(SOUND_GENERAL_BREAKBOX);
        o->activeFlags = 0;
    }
    
    ObjCheckFloorDeath(sp1E, D_803600E0);
}

void breakable_box_small_released_loop(void) {
    o->oBreakableBoxSmallFramesSinceReleased++;
    
    // Begin flashing
    if (o->oBreakableBoxSmallFramesSinceReleased > 810) {
        if (o->oBreakableBoxSmallFramesSinceReleased & 1)
            o->header.gfx.node.flags |= 0x10;
        else
            o->header.gfx.node.flags &= ~0x10;
    }

    // Despawn, and create a corkbox respawner
    if (o->oBreakableBoxSmallFramesSinceReleased > 900) {
        create_respawner(MODEL_BREAKABLE_BOX_SMALL, bhvBreakableBoxSmall, 3000);
        o->activeFlags = 0;
    }
}

void breakable_box_small_idle_loop(void) {
    switch(o->oAction) {
        case 0: 
            func_802F4DB4();
            break;

        case 100:
            ObjLavaDeath();
            break;

        case 101:
            o->activeFlags = 0;
            create_respawner(MODEL_BREAKABLE_BOX_SMALL, bhvBreakableBoxSmall, 3000);
            break;
    }

    if (o->oBreakableBoxSmallReleased == 1)
        breakable_box_small_released_loop();
}

void breakable_box_small_get_dropped(void) {
    obj_become_tangible();
    obj_enable_rendering();
    obj_get_dropped();
    o->header.gfx.node.flags &= ~0x10;
    o->oHeldState = 0;
    o->oBreakableBoxSmallReleased = 1;
    o->oBreakableBoxSmallFramesSinceReleased = 0;
}

void breakable_box_small_get_thrown(void) {
    obj_become_tangible();
    obj_enable_rendering_2();
    obj_enable_rendering();
    o->header.gfx.node.flags &= ~0x10;
    o->oHeldState = 0;
    o->oFlags &= ~0x08;
    o->oForwardVel = 40.0f;
    o->oVelY = 20.0f;
    o->oBreakableBoxSmallReleased = 1;
    o->oBreakableBoxSmallFramesSinceReleased = 0;
    o->activeFlags &= ~0x200;
}

void bhv_breakable_box_small_loop(void) {
    switch (o->oHeldState) {
        case 0:
            breakable_box_small_idle_loop();
            break;

        case 1:
            obj_disable_rendering();
            obj_become_intangible();
            break;

        case 2:
            breakable_box_small_get_thrown();
            break;

        case 3:
            breakable_box_small_get_dropped();
            break;
    }

    o->oInteractStatus = 0;
}

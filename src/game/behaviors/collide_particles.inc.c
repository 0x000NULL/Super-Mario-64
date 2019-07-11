// collide_particles.c.inc

s16 D_8032F2CC[] = {0xD000,0,0x3000,0,0xDE67,0x2199,0x2199,0x2199,0xDE67,0xDE67,0x2199,0xDE67};

s16 D_8032F2E4[] = {0xE000,0,0,0,0x2000,0,0xE99A,0x1666,0x1666,0x1666,0xE99A,0xE99A,0x1666,0xE99A};

void bhv_punch_tiny_triangle_loop(void)
{
    s16 sp1E;
    if(o->oTimer == 0)
    {
        sp1E = o->oMoveAngleYaw;
        o->OBJECT_FIELD_F32(0x1B) = 1.28f;
        obj_set_pos_relative(gMarioObject,0.0f,60.0f,100.0f);
        o->oMoveAngleYaw = sp1E; // does obj_set_pos_relative modify currentObject?
    }
    obj_move_using_fvel_and_gravity();
    o->oAnimState = 5;
    obj_scale(o->OBJECT_FIELD_F32(0x1B));
    o->OBJECT_FIELD_F32(0x1B) -= 0.2f;
    if(gDebugInfo[4][0] + 6 < o->oTimer)
        mark_object_for_deletion(o);
}

void bhv_punch_tiny_triangle_init(void)
{
    s32 i;
    UNUSED s32 unused;
    struct Object* triangle;
    for(i=0;i<6;i++)
    {
        triangle = spawn_object(o,MODEL_DIRT_ANIMATION,bhvPunchTinyTriangle);
        triangle->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + D_8032F2CC[2*i] + 0x8000;
        triangle->oVelY = sins(D_8032F2CC[2*i+1]) * 25.0f;
        triangle->oForwardVel = coss(D_8032F2CC[2*i+1]) * 25.0f;
    }
}

void bhv_wall_tiny_star_particle_loop(void)
{
    s16 sp1E;
    if(o->oTimer == 0)
    {
        sp1E = o->oMoveAngleYaw;
        o->OBJECT_FIELD_F32(0x1B) = 0.28f;
        obj_set_pos_relative(gMarioObject,0.0f,30.0f,110.0f);
        o->oMoveAngleYaw = sp1E;
    }
    obj_move_using_fvel_and_gravity();
    o->oAnimState = 4;
    obj_scale(o->OBJECT_FIELD_F32(0x1B));
    o->OBJECT_FIELD_F32(0x1B) -= 0.015f;
}

void bhv_tiny_star_particles_init(void)
{
    s32 i;
    UNUSED s32 unused;
    struct Object* particle;
    for(i=0;i<7;i++)
    {
        particle = spawn_object(o,MODEL_CARTOON_STAR,bhvWallTinyStarParticle);
        particle->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + D_8032F2E4[2*i] + 0x8000;
        particle->oVelY = sins(D_8032F2E4[2*i+1]) * 25.0f;
        particle->oForwardVel = coss(D_8032F2E4[2*i+1]) * 25.0f;
    }
}

void bhv_pound_tiny_star_particle_loop(void)
{
    if(o->oTimer == 0)
    {
        o->OBJECT_FIELD_F32(0x1B) = 0.28f;
        o->oForwardVel = 25.0f;
        o->oPosY -= 20.0f;
        o->oVelY = 14.0f;
    }
    obj_move_using_fvel_and_gravity();
    o->oAnimState = 4;
    obj_scale(o->OBJECT_FIELD_F32(0x1B));
    o->OBJECT_FIELD_F32(0x1B) -= 0.015f;
}

void bhv_pound_tiny_star_particle_init(void)
{
    s32 sp24;
    s32 sp20 = 8;
    struct Object* particle;
    for(sp24=0;sp24<sp20;sp24++)
    {
        particle = spawn_object(o,MODEL_CARTOON_STAR,bhvPoundTinyStarParticle);
        particle->oMoveAngleYaw = (sp24 << 16)/sp20;
    }
}

// piranha_particles.c.inc

void bhv_piranha_particles_spawn_loop(void)
{
    s32 i;
    obj_update_floor_and_walls();
    obj_move_standard(78);
    if(o->oMoveFlags & 2)
        mark_object_for_deletion(o);
    if(are_objects_collided(o,gMarioObject))
    {
        mark_object_for_deletion(o);
        for(i=0;i<10;i++)
            spawn_object(o,MODEL_PURPLE_MARBLE,bhvGiantPiranhaParticle);
    }
}

// ddd_warp.c.inc

void bhv_ddd_warp_loop(void)
{
    if(D_8035FFAC & 2)
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp_2);
    else
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp);
}

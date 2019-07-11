// beta_following_circling_obj.c.inc

// "TODO: This is not inside cannon. This is the bait coin from moneybag.
// I got baited."
// Double baited! This seems to be a beta bowser object. It continually
// updates to be about the same distance away from Mario as Bowser is
// and it is destructive.
void bhv_beta_following_circling_obj_loop(void)
{
    obj_set_pos_relative(gMarioObject, 0, 30.0f, 300.0f);
    o->hitboxRadius = gDebugInfo[4][0] + 100;
    o->hitboxHeight = gDebugInfo[4][1] + 300;
    attack_collided_non_mario_object(o);
}

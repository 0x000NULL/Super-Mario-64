// thi_top.c.inc

struct SpawnParticlesInfo D_8032F134 = {0,30,MODEL_SMALL_SNOW_BALL,0,40,0,20,40,252,30,20.0f,0.0f};

UNUSED u8 unused8032F134[] = {10,11,12};

void bhv_thi_huge_island_top_loop(void)
{
    if(gTHIWaterDrained & 1)
    {
        if(o->oTimer == 0)
            gWaterRegions[18] = 3000;
        obj_hide();
    }
    else
        load_object_collision_model();
}

void bhv_thi_tiny_island_top_loop(void)
{
    if(!(gTHIWaterDrained & 1)) // last bit not set
    {
        if(o->oAction == 0)
        {
            if(o->oDistanceToMario < 500.0f)
                if(gMarioStates->action == ACT_GROUND_POUND_LAND)
                {
                    o->oAction++;
                    obj_spawn_particles(&D_8032F134);
                    spawn_triangle_break_particles(20,138,0.3f,3);
                    PlaySound2(SOUND_GENERAL_EXPLOSION);
                    obj_hide();
                }
        }
        else
        {
            if(o->oTimer < 50)
            {
                gWaterRegions[18]--;
                PlaySound(SOUND_ENVIRONMENT_WATERDRAIN);
            }
            else
            {
                gTHIWaterDrained |= 1; // set last bit
                play_puzzle_jingle();
                o->oAction += 1;
            }
        }
    }
    else
    {
        if(o->oTimer == 0)
            gWaterRegions[18] = 700;
        obj_hide();
    }
}

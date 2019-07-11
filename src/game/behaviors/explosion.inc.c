// explosion.c.inc

void bhv_explosion_init(void)
{
    create_sound_spawner(SOUND_CH8_UNK2E);
    set_camera_shake_2(SHAKE_2_UNKNOWN_1);
    
    o->oOpacity = 255;
}

void bhv_explosion_loop(void)
{
    s32 i;
    
    if (o->oTimer == 9)
    {
        if (find_water_level(o->oPosX, o->oPosZ) > o->oPosY)
        {
            for (i = 0; i < 40; i++)
                spawn_object(o, MODEL_SMALL_SNOW_BALL, bhvBobombExplosionBubble);
        }
        else spawn_object(o, MODEL_SMOKE, bhvBobombBullyDeathSmoke);
        
        o->activeFlags = 0;
    }
    
    o->oOpacity -= 14;
    
    obj_scale((f32)o->oTimer / 9.0f + 1.0);
}

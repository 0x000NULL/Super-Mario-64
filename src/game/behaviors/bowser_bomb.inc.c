// bowser_bomb.c.inc

void bhv_bowser_bomb_loop(void)
{
    if (are_objects_collided(o, gMarioObject) == 1)
    {
        o->oInteractStatus &= ~INT_STATUS_INTERACTED; /* bit 15 */
        spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        o->activeFlags = 0;
    }
    
    if (o->oInteractStatus & INTERACT_MR_BLIZZARD) /* bit 21 */
    {
        spawn_object(o, MODEL_BOWSER_FLAMES, bhvBowserBombExplosion);
        create_sound_spawner(SOUND_GENERAL_EXPLOSION5);
        func_8027F440(3, o->oPosX, o->oPosY, o->oPosZ);
        o->activeFlags = 0;
    }
    
    SetObjectVisibility(o, 7000);
}

void bhv_bowser_bomb_explosion_loop(void)
{
    struct Object *mineSmoke;
    
    obj_scale((f32)o->oTimer / 14.0f * 9.0 + 1.0);
    if ((o->oTimer % 4 == 0) && (o->oTimer < 20))
    {
        mineSmoke = spawn_object(o, MODEL_BOWSER_SMOKE, bhvBowserBombSmoke);
        mineSmoke->oPosX += RandomFloat() * 600.0f - 400.0f;
        mineSmoke->oPosZ += RandomFloat() * 600.0f - 400.0f;
        mineSmoke->oVelY += RandomFloat() * 10.0f;
    }
    
    if (o->oTimer % 2 == 0) o->oAnimState++;
    if (o->oTimer == 28) o->activeFlags = 0;
}

void bhv_bowser_bomb_smoke_loop(void)
{
    obj_scale((f32)o->oTimer / 14.0f * 9.0 + 1.0);
    if (o->oTimer % 2 == 0) o->oAnimState++;
    
    o->oOpacity -= 10;
    if (o->oOpacity < 10) o->oOpacity = 0;
    
    o->oPosY += o->oVelY;
    
    if (o->oTimer == 28) o->activeFlags = 0;
}

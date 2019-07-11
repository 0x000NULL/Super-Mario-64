// wdw_water_level.c.inc

// called when WDW is loaded.
void bhv_init_changing_water_level_loop(void)
{
    if(gCurrentObject->oAction == 0)
    {
        if(gWaterRegions != NULL)
            gCurrentObject->oAction++;
    }
    else if(gCurrentObject->oTimer < 10)
        *gWaterLevels = gWaterRegions[6];
    else
    {
        gWaterRegions[6] = *gWaterLevels + sins(o->oWaterLevelTriggerUnkF4) * 20.0f;
        gCurrentObject->oWaterLevelTriggerUnkF4 += 0x200;
    }
}

void bhv_water_level_diamond_loop(void)
{
    if(gWaterRegions != NULL)
    {
        switch(o->oAction)
        {
        case WATER_LEVEL_DIAMOND_ACT_INIT:
            o->oFaceAngleYaw = 0;
            o->oWaterLevelTriggerTargetWaterLevel = (s32)o->oPosY;
            if(o->oTimer > 10)
                o->oAction++; // Sets to WATER_LEVEL_DIAMOND_ACT_IDLE
            break;
        case WATER_LEVEL_DIAMOND_ACT_IDLE:
            if(are_objects_collided(o,gMarioObject))
            {
                if(gWDWWaterLevelChanging == 0)
                {
                    o->oAction++; // Sets to WATER_LEVEL_DIAMOND_ACT_CHANGE_WATER_LEVEL
                    gWDWWaterLevelChanging = 1;
                }
            }
            break;
        case WATER_LEVEL_DIAMOND_ACT_CHANGE_WATER_LEVEL:
            o->oAngleVelYaw = 0;
            *gWaterLevels = (s32)approach_f32_symmetric((f32)*gWaterLevels,(f32)o->oWaterLevelTriggerTargetWaterLevel,10.0f);
            if(*gWaterLevels == o->oWaterLevelTriggerTargetWaterLevel)
            {
                if((s16)o->oFaceAngleYaw == 0)
                    o->oAction++; // Sets to WATER_LEVEL_DIAMOND_ACT_IDLE_SPINNING
                else
                    o->oAngleVelYaw = 0x800;
            }
            else
            {
                if(o->oTimer == 0)
                    PlaySound2(SOUND_GENERAL_WATERLEVELTRIG);
                else
                {
                    if(*gWaterLevels > o->oWaterLevelTriggerTargetWaterLevel)
                        PlaySound(SOUND_ENVIRONMENT_WATERDRAIN);
                    else
                        PlaySound(SOUND_ENVIRONMENT_WATERDRAIN); // same as above
                }
                o->oAngleVelYaw = 0x800;
            }
            break;
        case WATER_LEVEL_DIAMOND_ACT_IDLE_SPINNING:
            if(!are_objects_collided(o,gMarioObject))
            {
                gWDWWaterLevelChanging = 0;
                o->oAction = WATER_LEVEL_DIAMOND_ACT_IDLE;
                o->oAngleVelYaw = 0;
            }
            break;
        }
        o->oFaceAngleYaw += o->oAngleVelYaw;
    }
}

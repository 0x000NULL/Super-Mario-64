// hoot.c.inc

void bhv_hoot_init(void)
{
    SetObjAnimation(0);

    o->oHomeX = o->oPosX + 800.0f;
    o->oHomeY = o->oPosY - 150.0f;
    o->oHomeZ = o->oPosZ + 300.0f;
    o->header.gfx.node.flags |= 0x10; /* bit 4 */

    obj_become_intangible();
}

//sp28 = arg0
//sp2c = arg1

f32 HootFindNextFloor(struct FloorGeometry **arg0, f32 arg1)
{
    f32 sp24 = arg1 * sins(o->oMoveAngleYaw) + o->oPosX;
    UNUSED f32 sp20 = o->oPosY;
    f32 sp1c = arg1 * coss(o->oMoveAngleYaw) + o->oPosZ;
    f32 floorY = find_floor_height_and_data(sp24, 10000.0f, sp1c, arg0);
    
    return floorY;
}

void HootFloorBounce(void)
{
    struct FloorGeometry *sp1c;
    f32 floorY;
    
    floorY = HootFindNextFloor(&sp1c, 375.0f);
    if (floorY + 75.0f > o->oPosY) o->oMoveAnglePitch -= 3640.8888;
    
    floorY = HootFindNextFloor(&sp1c, 200.0f);
    if (floorY + 125.0f > o->oPosY) o->oMoveAnglePitch -= 7281.7776;
    
    floorY = HootFindNextFloor(&sp1c, 0);
    if (floorY + 125.0f > o->oPosY) o->oPosY = floorY + 125.0f;
    if (o->oMoveAnglePitch < -21845.3328) o->oMoveAnglePitch = -21845;
}

//sp30 = fastOscY
//sp34 = speed

void HootFreeStep(s16 fastOscY, s32 speed)
{
    struct FloorGeometry *sp2c;
    s16 yaw = o->oMoveAngleYaw;
    s16 pitch = o->oMoveAnglePitch;
    s16 sp26 = o->header.gfx.unk38.animFrame;
    f32 xPrev = o->oPosX;
    f32 zPrev = o->oPosZ;
    f32 hSpeed;
    
    o->oVelY = sins(pitch) * speed;
    hSpeed = coss(pitch) * speed;
    o->oVelX = sins(yaw) * hSpeed;
    o->oVelZ = coss(yaw) * hSpeed;
    
    o->oPosX += o->oVelX;
    if (fastOscY == 0)
        o->oPosY -= o->oVelY + coss((s32)(sp26 * 3276.8)) * 50.0f / 4.0f;
    else
        o->oPosY -= o->oVelY + coss((s32)(sp26 * 6553.6)) * 50.0f / 4.0f;
    o->oPosZ += o->oVelZ;
    
    find_floor_height_and_data(o->oPosX, o->oPosY, o->oPosZ, &sp2c);
    if (sp2c == NULL)
    {
        o->oPosX = xPrev;
        o->oPosZ = zPrev;
    }
    
    if (sp26 == 0) PlaySound2(SOUND_GENERAL_SWISHWATER);
}

void PlayerSetHootYaw(void)
{
    s16 stickX = gPlayer3Controller->rawStickX;
    s16 stickY = gPlayer3Controller->rawStickY;
    UNUSED s16 pitch = o->oMoveAnglePitch;
    if (stickX < 10 && stickX >= -9) stickX = 0;
    if (stickY < 10 && stickY >= -9) stickY = 0;
    
    o->oMoveAngleYaw -= 5 * stickX;
}

//sp28 = speed
//sp2c = xPrev
//sp30 = zPrev

void HootCarryStep(s32 speed, UNUSED f32 xPrev, UNUSED f32 zPrev)
{
    s16 yaw = o->oMoveAngleYaw;
    s16 pitch = o->oMoveAnglePitch;
    s16 sp22 = o->header.gfx.unk38.animFrame;
    f32 hSpeed;
    
    o->oVelY = sins(pitch) * speed;
    hSpeed = coss(pitch) * speed;
    o->oVelX = sins(yaw) * hSpeed;
    o->oVelZ = coss(yaw) * hSpeed;
    
    o->oPosX += o->oVelX;
    o->oPosY -= o->oVelY + coss((s32)(sp22 * 6553.6)) * 50.0f / 4.0f;
    o->oPosZ += o->oVelZ;
    
    if (sp22 == 0) PlaySound2(SOUND_GENERAL_SWISHWATER);
}

//sp48 = xPrev
//sp4c = yPrev
//sp50 = zPrev

void HootSurfaceCollision(f32 xPrev, UNUSED f32 yPrev, f32 zPrev)
{
    struct FloorGeometry *sp44;
    struct WallCollisionData hitbox;
    f32 floorY;
    
    hitbox.x = o->oPosX;
    hitbox.y = o->oPosY;
    hitbox.z = o->oPosZ;
    hitbox.offsetY = 10.0;
    hitbox.radius = 50.0;
    
    if (find_wall_collisions(&hitbox) != 0)
    {
        o->oPosX = hitbox.x;
        o->oPosY = hitbox.y;
        o->oPosZ = hitbox.z;
        gMarioObject->oInteractStatus |= INT_STATUS_MARIO_UNK7; /* bit 7 */
    }
    
    floorY = find_floor_height_and_data(o->oPosX, o->oPosY, o->oPosZ, &sp44);
    if (sp44 == NULL)
    {
        o->oPosX = xPrev;
        o->oPosZ = zPrev;
        return;
    }
    
    
    if (absf_2(o->oPosX) > 8000.0f) o->oPosX = xPrev;
    if (absf_2(o->oPosZ) > 8000.0f) o->oPosZ = zPrev;
    if (floorY + 125.0f > o->oPosY) o->oPosY = floorY + 125.0f;
}

//sp28 = xPrev
//sp2c = zPrev

void HootAscentLoop(f32 xPrev, f32 zPrev)
{
    f32 negX = 0 - o->oPosX;
    f32 negZ = 0 - o->oPosZ;
    s16 angleToOrigin = atan2s(negZ, negX);
    
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, angleToOrigin, 0x500);
    o->oMoveAnglePitch = 0xCE38;
    
    if (o->oTimer >= 29)
    {
        PlaySound(SOUND_ENVIRONMENT_WIND2);
        o->header.gfx.unk38.animFrame = 1;
    }
    
    if (o->oPosY > 6500.0f) o->oAction = HOOT_ACT_CARRY;
    
    HootCarryStep(60, xPrev, zPrev);
}

void HootActionLoop(void)
{
    f32 xPrev = o->oPosX;
    f32 yPrev = o->oPosY;
    f32 zPrev = o->oPosZ;
    
    switch (o->oAction)
    {
        case HOOT_ACT_ASCENT:
            HootAscentLoop(xPrev, zPrev);
            break;
            
        case HOOT_ACT_CARRY:
            PlayerSetHootYaw();
            
            o->oMoveAnglePitch = 0x71C;
            
            if (o->oPosY < 2700.0f)
            {
                set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                
                if (func_8028F8E0(162, o, 45))
                {
                    clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                    
                    o->oAction = HOOT_ACT_TIRED;
                }
            }
            
            HootCarryStep(20, xPrev, zPrev);
            break;
            
        case HOOT_ACT_TIRED:
            PlayerSetHootYaw();
            
            o->oMoveAnglePitch = 0;
            
            HootCarryStep(20, xPrev, zPrev);
            
            if (o->oTimer >= 61) gMarioObject->oInteractStatus |= INT_STATUS_MARIO_UNK7; /* bit 7 */
            break;
    }
    
    HootSurfaceCollision(xPrev, yPrev, zPrev);
}

void HootTurnToHome(void)
{
    f32 homeDistX = o->oHomeX - o->oPosX;
    f32 homeDistY = o->oHomeY - o->oPosY;
    f32 homeDistZ = o->oHomeZ - o->oPosZ;
    s16 hAngleToHome = atan2s(homeDistZ, homeDistX);
    s16 vAngleToHome = atan2s(sqrtf(homeDistX * homeDistX + homeDistZ * homeDistZ), -homeDistY);
    
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, hAngleToHome, 0x140);
    o->oMoveAnglePitch = approach_s16_symmetric(o->oMoveAnglePitch, vAngleToHome, 0x140);
}

void HootAwakeLoop(void)
{
    if (o->oInteractStatus == INTERACT_HOOT)
    {
        HootActionLoop();
        
        SetObjAnimation(1);
    }
    else
    {
        SetObjAnimation(0);
        
        HootTurnToHome();
        HootFloorBounce();
        HootFreeStep(0, 10);
        
        o->oAction = 0;
        o->oTimer = 0;
    }
    
    SetObjectVisibility(o, 2000);
}

void bhv_hoot_loop(void)
{
    switch (o->oHootAvailability)
    {
        case HOOT_AVAIL_ASLEEP_IN_TREE:
            if (IsPointCloseToMario(o->oPosX, o->oPosY, o->oPosZ, 50))
            {
                o->header.gfx.node.flags &= ~0x10; /* bit 4 */
                o->oHootAvailability = HOOT_AVAIL_WANTS_TO_TALK;
            }
            break;
            
        case HOOT_AVAIL_WANTS_TO_TALK:
            HootAwakeLoop();
            
            if (set_mario_npc_dialogue(2) == 2 && func_8028F8E0(162, o, 44))
            {
                set_mario_npc_dialogue(0);
                
                obj_become_tangible();
                
                o->oHootAvailability = HOOT_AVAIL_READY_TO_FLY;
            }
            break;
            
        case HOOT_AVAIL_READY_TO_FLY:
            HootAwakeLoop();
            break;
    }
}
